/*
 * Copyright 2016 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <memory>
#include <vector>

#include "cairo.h"
#include "fontdiff/line.h"
#include "fontdiff/line_differ.h"
#include "dtl/dtl.hpp"

namespace fontdiff {

class LineDiffer {
 public:
  LineDiffer(const Line* before, const Line* after);
  ~LineDiffer();
  void FindDiffs(std::vector<DeltaRange>* removals,
                 std::vector<DeltaRange>* additions) const;

 private:
  void ComputeHash(unsigned const char* data, std::vector<uint32_t>* hash);
  void MergeRange(size_t start_idx, size_t end_idx,
                  std::vector<DeltaRange>* ranges) const;

  static const int scale_;
  static const int render_scale_;
  static const int merge_threshold_;
  FT_F26Dot6 width_, height_;
  cairo_surface_t* beforeSurface_;
  cairo_surface_t* afterSurface_;
  unsigned const char* beforeData_;
  unsigned const char* afterData_;
  size_t imageWidth_, imageHeight_, imageStride_;
  std::vector<uint32_t> beforeHash_, afterHash_;
};

const int LineDiffer::scale_ = 8;
const int LineDiffer::render_scale_ = 64;
const int LineDiffer::merge_threshold_ = 2 * render_scale_ / scale_;

LineDiffer::LineDiffer(const Line* before, const Line* after) {
  width_ = std::max(before->GetWidth(), after->GetWidth());
  height_ = std::max(before->GetHeight(), after->GetHeight());
  beforeSurface_ =
      cairo_image_surface_create(CAIRO_FORMAT_A1,
                                 (width_ * scale_) / render_scale_,
                                 (height_ * scale_) / render_scale_);
  afterSurface_ =
      cairo_image_surface_create(CAIRO_FORMAT_A1,
                                 (width_ * scale_) / render_scale_,
                                 (height_ * scale_) / render_scale_);

  cairo_t* beforeGC = cairo_create(beforeSurface_);
  cairo_scale(beforeGC, scale_, scale_);
  before->Render(beforeGC, 0, 0);
  cairo_surface_flush(beforeSurface_);
  cairo_destroy(beforeGC);
  beforeData_ = cairo_image_surface_get_data(beforeSurface_);

  cairo_t* afterGC = cairo_create(afterSurface_);
  cairo_scale(afterGC, scale_, scale_);
  after->Render(afterGC, 0, 0);
  cairo_surface_flush(afterSurface_);
  afterData_ = cairo_image_surface_get_data(afterSurface_);
  cairo_destroy(afterGC);

  imageWidth_ =
      static_cast<size_t>(cairo_image_surface_get_width(beforeSurface_));
  imageHeight_ =
      static_cast<size_t>(cairo_image_surface_get_height(beforeSurface_));
  imageStride_ =
      static_cast<size_t>(cairo_image_surface_get_stride(beforeSurface_));

  // Only compute hashes if image data differs.
  if (memcmp(beforeData_, afterData_, imageStride_ * imageHeight_)) {
    ComputeHash(beforeData_, &beforeHash_);
    ComputeHash(afterData_, &afterHash_);
  }

  // Hack: For some reason (cairo internals?) cairo scaling does not work
  // correctly without this additional rendering.
  // TODO: Remove this workaround once better solution found.
  cairo_t* tempGC = cairo_create(beforeSurface_);
  before->Render(tempGC, 0, 0);
  after->Render(tempGC, 0, 0);
  cairo_destroy(tempGC);
}

void LineDiffer::ComputeHash(unsigned const char* data,
			     std::vector<uint32_t>* hash) {
  // https://en.wikipedia.org/wiki/Jenkins_hash_function
  hash->resize(imageWidth_, 0);
  for (size_t x = 0; x < imageWidth_; ++x) {
    uint32_t h = (*hash)[x];
    for (size_t y = 0; y < imageHeight_; ++y) {
      int byte_offset = x / 8;
      int bit_offset = x % 8;
      uint8_t val = (static_cast<int8_t>(data[y * imageStride_ + byte_offset]) >> bit_offset) & 1;
      h += val;
      h += (h << 10);
      h ^= (h >> 6);
    }
    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);
    (*hash)[x] = h;
  }
}

LineDiffer::~LineDiffer() {
  cairo_surface_destroy(beforeSurface_);
  cairo_surface_destroy(afterSurface_);
}

void LineDiffer::MergeRange(size_t start_idx, size_t end_idx,
                            std::vector<DeltaRange>* ranges) const {
  DeltaRange range;
  range.x = start_idx * render_scale_ / scale_;
  range.width = (end_idx - start_idx + 1) * render_scale_ / scale_;
  if (ranges->size()) {
    DeltaRange& prev = ranges->back();
    if (range.x - prev.x - prev.width < merge_threshold_) {
      prev.width = range.x + range.width - prev.x;
      return;
    }
  }
  ranges->push_back(range);
}

void LineDiffer::FindDiffs(std::vector<DeltaRange>* removals,
                           std::vector<DeltaRange>* additions) const {
  size_t position = -1, start_idx = -1, end_idx = -1;
  removals->clear();
  additions->clear();
  // Build shortest edit sequence and go through each element.
  dtl::Diff<uint32_t> d(beforeHash_, afterHash_);
  d.compose();
  dtl::edit_t last_op = dtl::SES_COMMON;
  for (const auto& elem : d.getSes().getSequence()) {
    if (elem.second.type != dtl::SES_COMMON) {
      position = (elem.second.type == dtl::SES_ADD)
          ? elem.second.afterIdx : elem.second.beforeIdx;
    }
    // Edit operation type changed. Merge ranges of Add or Remove
    // operations if common chunk is reached.
    if (elem.second.type != last_op) {
      if (last_op != dtl::SES_COMMON) {
        MergeRange(start_idx, end_idx,
                   last_op == dtl::SES_ADD ? additions : removals);
      } //else {
        start_idx = position;
      //}
      last_op = elem.second.type;
    }
    end_idx = position;
  }

  // If sequence ends with either Add or Remove, merge it.
  if (last_op != dtl::SES_COMMON){
    MergeRange(start_idx, end_idx,
               last_op == dtl::SES_ADD ? additions : removals);
  }
}

bool FindDeltas(const Line* before, const Line* after,
                std::vector<DeltaRange>* removals,
                std::vector<DeltaRange>* additions) {
  LineDiffer differ(before, after);
  differ.FindDiffs(removals, additions);
  return !removals->empty() || !additions->empty();
}

}  // namespace fontdiff

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

namespace fontdiff {

class LineDiffer {
 public:
  LineDiffer(const Line* before, const Line* after);
  ~LineDiffer();
  void FindDiffs(std::vector<DeltaRange>* removals,
                 std::vector<DeltaRange>* additions) const;

 private:
  bool IsImageColumnEqual(int x1, int x2) const;
  void ComputeHash(unsigned const char* data, std::vector<uint32_t>* hash);

  static const int scale_;
  FT_F26Dot6 width_, height_;
  cairo_surface_t* beforeSurface_;
  cairo_surface_t* afterSurface_;
  unsigned const char* beforeData_;
  unsigned const char* afterData_;
  size_t imageWidth_, imageHeight_, imageStride_;
  std::vector<uint32_t> beforeHash_, afterHash_;
};

const int LineDiffer::scale_ = 8;

LineDiffer::LineDiffer(const Line* before, const Line* after) {
  width_ = std::max(before->GetWidth(), after->GetWidth());
  height_ = std::max(before->GetHeight(), after->GetHeight());
  beforeSurface_ =
      cairo_image_surface_create(CAIRO_FORMAT_A1,
                                 (width_ * scale_) / 64,
                                 (height_ * scale_) / 64);
  afterSurface_ =
      cairo_image_surface_create(CAIRO_FORMAT_A1,
                                 (width_ * scale_) / 64,
                                 (height_ * scale_) / 64);
  cairo_t* beforeGC = cairo_create(beforeSurface_);
  cairo_t* afterGC = cairo_create(afterSurface_);
  cairo_scale(beforeGC, scale_, scale_);
  cairo_scale(afterGC, scale_, scale_);
  before->Render(beforeGC, 0, 0);
  after->Render(afterGC, 0, 0);
  cairo_destroy(beforeGC);
  cairo_destroy(afterGC);
  cairo_surface_flush(beforeSurface_);
  cairo_surface_flush(afterSurface_);

  beforeData_ = cairo_image_surface_get_data(beforeSurface_);
  afterData_ = cairo_image_surface_get_data(afterSurface_);

  imageWidth_ =
      static_cast<size_t>(cairo_image_surface_get_width(beforeSurface_));
  imageHeight_ =
      static_cast<size_t>(cairo_image_surface_get_height(beforeSurface_));
  imageStride_ =
      static_cast<size_t>(cairo_image_surface_get_stride(beforeSurface_));

  ComputeHash(beforeData_, &beforeHash_);
  ComputeHash(afterData_, &afterHash_);
}

void LineDiffer::ComputeHash(unsigned const char* data,
			     std::vector<uint32_t>* hash) {
  // https://en.wikipedia.org/wiki/Jenkins_hash_function
  hash->resize(imageWidth_, 0);
  for (size_t x = 0; x < imageWidth_; ++x) {
    uint32_t h = (*hash)[x];
    for (size_t y = 0; y < imageHeight_; ++y) {
      h += static_cast<int8_t>(data[y * imageStride_ + x]);
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

void LineDiffer::FindDiffs(std::vector<DeltaRange>* removals,
                           std::vector<DeltaRange>* additions) const {
  removals->clear();
  additions->clear();
  if (memcmp(beforeData_, afterData_, imageStride_ * imageHeight_) == 0) {
    return;
  }

  int x = 0;
  for (x = 0; x < imageWidth_ && IsImageColumnEqual(x, x); ++x) {
  }

  DeltaRange r;
  r.x = x * 512 / scale_;
  r.width = width_ - r.x;
  additions->push_back(r);
  removals->push_back(r);
}

bool LineDiffer::IsImageColumnEqual(int x1, int x2) const {
  if (x1 < 0 || x1 >= imageWidth_ || x2 < 0 || x2 >= imageWidth_) {
    return true;
  }

  if (beforeHash_[x1] != afterHash_[x2]) {
    return false;
  }

  unsigned const char* a = beforeData_ + x1;
  unsigned const char* b = afterData_ + x2;
  unsigned const char* stop = a + (imageHeight_ * imageStride_);
  while (a < stop) {
    if (*a != *b) {
      return false;
    }
    a += imageStride_;
    b += imageStride_;
  }
  return true;
}

bool FindDeltas(const Line* before, const Line* after,
                std::vector<DeltaRange>* removals,
                std::vector<DeltaRange>* additions) {
  LineDiffer differ(before, after);
  differ.FindDiffs(removals, additions);
  return !removals->empty() || !additions->empty();
}

}  // namespace fontdiff

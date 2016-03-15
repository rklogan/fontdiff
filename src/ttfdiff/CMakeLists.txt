cmake_minimum_required(VERSION 3.4.1)
project(ttfdiff)

find_package(PkgConfig REQUIRED)
pkg_search_module(CAIRO REQUIRED cairo)
pkg_search_module(FREETYPE REQUIRED freetype2)
pkg_search_module(HARFBUZZ REQUIRED harfbuzz)
pkg_search_module(HARFBUZZ_ICU REQUIRED harfbuzz-icu)
pkg_search_module(EXPAT REQUIRED expat)
pkg_search_module(ICU REQUIRED icu-i18n)
pkg_search_module(OPENSSL REQUIRED openssl)

add_executable(ttfdiff
  argument_parser.cc
  diff_job.cc
  font.cc
  font_loader.cc
  icu_helper.cc
  language.cc
  main.cc
  paragraph.cc
  shaped_text.cc
  style.cc
  style_key.cc)

target_compile_features(ttfdiff PRIVATE cxx_constexpr)

target_include_directories(ttfdiff
  PRIVATE ..
  ${CAIRO_INCLUDE_DIRS}
  ${EXPAT_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${HARFBUZZ_INCLUDE_DIRS}
  ${HARFBUZZ_ICU_INCLUDE_DIRS}
  ${ICU_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIRS})

target_link_libraries(ttfdiff
  ${CAIRO_LIBRARIES}
  ${EXPAT_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${HARFBUZZ_LIBRARIES}
  ${HARFBUZZ_ICU_LIBRARIES}
  ${ICU_LIBRARIES}
  ${OPENSSL_LIBRARIES})

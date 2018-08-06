#pragma once

#include <cstdint>
#include <vector>

#include "errors.h"

struct hsv_t {
  uint8_t h;
  uint8_t s;
  uint8_t v;

  template <typename T>
  void from_vec(const std::vector<T>& vec);

  template <typename T>
  void from_arr(const T arr[]);

  template <typename T>
  void to_vec(std::vector<T>& vec);

  template <typename T>
  void to_arr(T* arr);
};

typedef std::vector<hsv_t> hsv_vec_t;


template <typename T>
void hsv_t::from_arr(const T* arr)
{
#ifndef NDEBUG
  VALIDATE_IN_RANGE(arr[0], 0, 255);
  VALIDATE_IN_RANGE(arr[1], 0, 255);
  VALIDATE_IN_RANGE(arr[2], 0, 255);
#endif

  h = static_cast<uint8_t>(arr[0]);
  s = static_cast<uint8_t>(arr[1]);
  v = static_cast<uint8_t>(arr[2]);
}

template <typename T>
void hsv_t::from_vec(const std::vector<T>& vec) 
{
  if (vec.size() != 3) {
    RUNTIME_ERROR("invalid hsv vector size of %d", (int) vec.size());
  }
  this->from_arr<T>(&vec[0]);
}

template <typename T>
void hsv_t::to_arr(T* arr)
{
  arr[0] = static_cast<T>(h);
  arr[1] = static_cast<T>(s);
  arr[2] = static_cast<T>(v);
}

template <typename T>
void hsv_t::to_vec(std::vector<T>& vec)
{
  if (vec.size() != 3) {
    vec.resize(3);
  }
  this->to_arr<T>(&vec[0]);
}



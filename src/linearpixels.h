#pragma once

#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <vector>

#include "errors.h"

static const int MAX_NUM_OF_STRIPS = 48;

struct hsv_t {
  uint8_t h;
  uint8_t s;
  uint8_t v;

  void from_vec(const std::vector<int>& vec) 
  {
    if (vec.size() != 3) {
      RUNTIME_ERROR("invalid hsv vector size of %d", (int) vec.size());
    }
    VALIDATE_IN_RANGE(vec[0], 0, 255);      
    VALIDATE_IN_RANGE(vec[1], 0, 255);      
    VALIDATE_IN_RANGE(vec[2], 0, 255);      
    h = static_cast<uint8_t>(vec[0]);
    s = static_cast<uint8_t>(vec[1]);
    v = static_cast<uint8_t>(vec[2]);
  }

  template <typename T>
  void to_vec(std::vector<T>& vec)
  {
    if (vec.size() != 3) {
      vec.resize(3);
    }
    vec[0] = static_cast<T>(h);
    vec[1] = static_cast<T>(s);
    vec[2] = static_cast<T>(v);
  }
};

typedef std::vector<hsv_t> hsv_vec_t;

// Linear Pixels maps 2D (strip, pixel) to one linear addressing scheme.
// addresses linearily from strip 0 to MAX_NUM_OF_STRIPS
class LinearPixels
{
  public:
    
    LinearPixels(size_t number_of_strips_, size_t max_leds_per_strip_);
    
    // returns the flat address offset
    size_t get_linaddr(size_t strip_id, size_t offset) const 
    {
      return strip_id * max_leds_per_strip + offset;
    }

    void disable_pixel(size_t linear_addr)
    {
      active_mask[linear_addr] = 0;
    }

    void disable_strip(size_t strip_id)
    {
      for (size_t i = 0; i < max_leds_per_strip; i++) {
        disable_pixel(get_linaddr(strip_id, i));
      }
    }

    bool is_active(int linear_addr) const
    {
      return active_mask[linear_addr] == 1;
    }
  
    size_t size() const { return data.size(); }

    // access operators
    const hsv_t& operator[] (size_t i) const { return data[i]; }
          hsv_t& operator[] (size_t i)       { return data[i]; }

    void print() const;

    // returns a vector containing all the values under the indices v_indices.
    // for instance, invoking this method with v_indices of { 0, 14, 901, 1003 } 
    // returns a new vector of type hsv_t, size of 4 containing the values of
    // data[0], data[14], data[901], data[1003] in that order.
    hsv_vec_t get_pixel_group(const std::vector<uint16_t>& v_indices) const;

    // sets data to a given pixel group and indices
    void set_pixel_group(const std::vector<uint16_t>& v_indices, 
                         const hsv_vec_t& hsv_vec);


  private:
    size_t number_of_strips;
    size_t max_leds_per_strip;
    std::vector<hsv_t> data;
    std::vector<uint8_t> active_mask;
};


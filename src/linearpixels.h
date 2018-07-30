#pragma once

#include "stdint.h"
#include <iostream>
#include <iomanip>
#include <vector>

static const int MAX_NUM_OF_STRIPS = 48;

struct hsv_t {
  uint8_t h;
  uint8_t s;
  uint8_t v;
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
    void get_pixel_group(const std::vector<uint16_t>& v_indices, 
                         const hsv_vec_t& hsv_vec);


  private:
    size_t number_of_strips;
    size_t max_leds_per_strip;
    std::vector<hsv_t> data;
    std::vector<uint8_t> active_mask;
};


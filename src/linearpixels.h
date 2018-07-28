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


  private:
    size_t number_of_strips;
    size_t max_leds_per_strip;
    std::vector<hsv_t> data;
    std::vector<uint8_t> active_mask;
};


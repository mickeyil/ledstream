#pragma once

#include "json_helpers.h"

// for hsv_t
#include "linearpixels.h"

/*
 1
        peak
         ^\
        /  -\
       /     -\
      /        -\
 0  -*-----------*----> t
    t_s         t_e
*/

static const int DEFAULT_HSV_MASK[] = { 1, 1, 1 };

class Blender
{
  public:

		static const float DEFAULT_START_VALUE = 0.0;
		static const float DEFAULT_PEAK_VALUE = 1.0;
		static const float DEFAULT_END_VALUE = 0.0;
	  static const float DEFAULT_PEAK_TIME = 0.0;	

    Blender(const Json::Value& root);
    ~Blender() { }

    // *** NOTE: these might belong to a future "father" class ***
    bool is_initialized() const { return _is_initialized; }

    bool is_enabled() const { return _is_enabled; }

    // NOTE: this might be a virtual method in a future abstract class
    void render(float t, hsv_vec_t& pixels);
    // *** end

  private:
    // *** NOTE: these might belong to a future "father" class ***
    bool _is_initialized;

    bool _is_enabled;
    // *** end

    // duration of animation
    float duration;

    // peak time. values should be in range: [0, 1]
    float peak_time; 
  
    // intensity values. values are in range: [0, 1]
    float peak_value;
    float start_value;
    float end_value;

    // fg and bg color. blend is calculated:
    // val = a(t)*fg + (1 - a(t))*bg
    // where a(t) is the piecewise linear built from duration, peak value/time etc. 
    hsv_t fg_color;
    hsv_t bg_color; 

    // activation mask per color channel
    int hsv_mask[3];
};


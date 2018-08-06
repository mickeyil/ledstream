#pragma once

#include "colorspace.h"
#include "json_helpers.h"


class Animation
{

  public:
    Animation(const Json::Value& params);
    virtual ~Animation() noexcept { }

    // indicates if an animation is successfully initialized. false value indicates
    // that an error has occured during initialization, perhaps due to invalid
    // or missing values.
    bool is_initialized() const { return _is_initialized; }

    // indicates if an animation is enabled. disabled animation is a NO-OP in
    // regard to the data.
    bool is_enabled() const { return _is_enabled; }

    // set enable state
    void set_enable(bool is_enabled) { _is_enabled = is_enabled; }

    // render function takes a relative time to activation time, and pixels
    // vector. It modifies the pixel vector with the updated values.
    virtual void render(float t_relative, hsv_vec_t& pixels) = 0;

    // set animation parameters from JSON input
    virtual void set_parameters(const Json::Value& params) = 0;

  private:
    bool _is_initialized;
    bool _is_enabled;
};



// Blender animation does Alpha-blend with the background:
// out = a(t)*fg + (1-a(t))*bg 
// a(t) is a piecewise linear function of the form below. Can be configured
// via JSON to the start/end values and peak value/location.
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


class Blender : public Animation
{
  public:

		static constexpr float DEFAULT_START_VALUE = 0.0;
		static constexpr float DEFAULT_PEAK_VALUE = 1.0;
		static constexpr float DEFAULT_END_VALUE = 0.0;
	  static constexpr float DEFAULT_PEAK_PERCENT = 0.0;	

    Blender(const Json::Value& params);
    virtual ~Blender() noexcept { }

    virtual void render(float t_relative, hsv_vec_t& pixels);

    virtual void set_parameters(const Json::Value& params);

  private:
    // duration of animation
    float duration;

    // peak time. values should be in range: [0, 1]
    float peak_percent; 
  
    // intensity values. values are in range: [0, 1]
    float peak_value;
    float start_value;
    float end_value;

    // fg and bg color. blend is calculated:
    // val = a(t)*fg + (1 - a(t))*bg
    // where a(t) is the piecewise linear built from duration, peak value/time etc. 
    std::vector<float> fg_hsv;

    // activation mask per color channel
    std::vector<int> hsv_mask;

    // piecewise linear function details. calculated when parameters are set
    bool is_ready;      // ready to render if set_parameters() was called on the object
    float m1, m2, t_peak;
};


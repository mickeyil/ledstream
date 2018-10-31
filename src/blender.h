#include "animation.h"


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


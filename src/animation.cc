#include <cassert>
#include <sstream>
#include <string>
#include "animation.h"
#include "errors.h"
#include "slog.h"


Animation::Animation(const Json::Value& params) :
  _is_initialized(false), _is_enabled(true)
{
}

void Blender::set_parameters(const Json::Value& params)
{
  json_read_v_required<float>(params, "fg_color_hsv", fg_hsv);
  VALIDATE_TRUE(fg_hsv.size() == 3, "hsv vector size must be 3");

  // default HSV mask is all 1's
  if (!json_read_v<int>(params, "hsv_mask", hsv_mask)) {
    hsv_mask.resize(3);
    hsv_mask[0] = hsv_mask[1] = hsv_mask[2] = 1;
  }
    
  json_read_required<float>(params, "duration", duration);
  VALIDATE_TRUE(duration > 0.0, "duration must be positive");

  peak_percent = params.get("peak_percent", DEFAULT_PEAK_PERCENT).asDouble();
  VALIDATE_IN_RANGE(peak_percent, 0.0, 1.0);

  start_value = params.get("start_value", DEFAULT_START_VALUE).asDouble();
  VALIDATE_IN_RANGE(start_value, 0.0, 1.0);

  peak_value = params.get("peak_value", DEFAULT_PEAK_VALUE).asDouble();
  VALIDATE_IN_RANGE(peak_value, 0.0, 1.0);

  end_value = params.get("end_value", DEFAULT_END_VALUE).asDouble();
  VALIDATE_IN_RANGE(end_value, 0.0, 1.0);

  // calculate slopes for piecewise linear function
  // NOTE: those doesn't change each time a render is made and are cached
  // to instance variables.

  // t_peak is the time at which the alpha function gets its peak value.
  t_peak = peak_percent * duration;
  m1 = 0.0; 
  m2 = 0.0;
  if (t_peak > 0.0)
    m1 = (peak_value - start_value) / t_peak;
  if (duration - t_peak > 0.0)
    m2 = (end_value - peak_value) / (duration - t_peak);
}


Blender::Blender(const Json::Value& params) :
  Animation(params)
{
  set_parameters(params);
}


void Blender::render(float t, hsv_vec_t& pixels)
{
  // disabled animation doesn't render
  if (!is_enabled())
    return;

  // time t is relative to animation start time.
  // if outside of its [t_start,t_end] time, there's nothing to render.
  if (t < 0.0 || t > duration)
    return;

  assert(t >= 0.0);
  assert(t <= duration);
  float alpha;
  if (t < t_peak) {
    alpha = m1*t + start_value;
  } else {
    alpha = m2*t - m2*t_peak + peak_value;
  }

  float bg_hsv[3], res[3];
  // iterate on every pixels in the pixel group
  for (size_t pidx = 0; pidx < pixels.size(); pidx++) {

    pixels[pidx].to_arr<float>(bg_hsv);
    for (size_t i = 0; i < 3; i++) {
      
      // if mask is not active, take existing pixel color (bg)
      if (hsv_mask[i]) {
        res[i] = alpha*fg_hsv[i] + (1.0 - alpha)*bg_hsv[i];
      } else {
        res[i] = bg_hsv[i];
      }
    }
    // write updated value to pixel group
    pixels[pidx].from_arr<float>(res);
  }
}

#include "animation.h"
#include "errors.h"
#include "slog.h"


Animation::Animation(const Json::Value& params) :
  _is_initialized(false), _is_enabled(true)
{
}


void Blender::set_parameters(const Json::Value& params)
{
  const Json::Value ap = params["animation_parameters"];
  std::vector<int> hsv_vec;
  json_read_v_required<int>(params, "fg_color_hsv", hsv_vec);
  fg_color_hsv.set(hsv_vec);
  
  // default HSV mask is all 1's
  if (!json_read_v<int>(params, "hsv_mask", hsv_mask)) {
    hsv_mask.resize(3);
    hsv_mask[0] = hsv_mask[1] = hsv_mask[2] = 1;
  }
    
  duration = ap["duration"].asDouble();
  VALIDATE_TRUE(duration > 0.0, "duration must be positive");

  peak_time = ap.get("peak_time", DEFAULT_PEAK_TIME).asDouble();
  VALIDATE_IN_RANGE(peak_time, 0.0, 1.0);

  start_value = ap.get("start_value", DEFAULT_START_VALUE).asDouble();
  VALIDATE_IN_RANGE(start_value, 0.0, 1.0);

  peak_value = ap.get("peak_value", DEFAULT_PEAK_VALUE).asDouble();
  VALIDATE_IN_RANGE(peak_value, 0.0, 1.0);

  end_value = ap.get("end_value", DEFAULT_END_VALUE).asDouble();
  VALIDATE_IN_RANGE(end_value, 0.0, 1.0);
}


Blender::Blender(const Json::Value& params) :
  Animation(params)
{
  set_parameters(params);
}

void Blender::render(float t, hsv_vec_t& pixels)
{

}

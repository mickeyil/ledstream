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


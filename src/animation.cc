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


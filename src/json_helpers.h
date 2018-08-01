#pragma once

#include <cstdarg>
#include <cstring>
#include <string>
#include "json/json.h"
#include "errors.h"
#include "slog.h"



static
bool json_has_field(const Json::Value& root, const char *fieldname)
{
  return root.isMember(fieldname);
}



static 
bool json_read_string(const Json::Value& root, const char* fieldname, std::string& val, std::string& err)
{
  if (!json_has_field(root, fieldname)) {
    return false;
  }
  
  if (!root[fieldname].isString()) {
    printf("field %s isn't a string.\n", fieldname);
    return false;
  }
  
  val = root[fieldname].asString();
  return true;  
}



static 
bool json_read_int(const Json::Value& root, const char* fieldname, int& val)
{
  if (!json_has_field(root, fieldname)) {
    return false;
  }
  
  if (!root[fieldname].isInt()) {
    printf("field %s isn't an int.\n", fieldname);
    return false;
  }
  
  val = root[fieldname].asInt();
  return true;  
}



static
bool json_read_double(const Json::Value& root, const char* fieldname, double& val)
{
  if (!json_has_field(root, fieldname)) {
    return false;
  }
  
  if (!root[fieldname].isDouble()) {
    printf("field %s isn't a double.\n", fieldname);
    return false;
  }
  
  val = root[fieldname].asDouble();
  return true;  
}



template <typename T>
static
bool json_read_(const Json::Value& root, const char *fieldname, T& val)
{
  LOGF("json_read(): unimplemented type.");
  return false;
}



template <>
bool json_read_<bool>(const Json::Value& root, const char *fieldname, bool& val)
{
  if (!root[fieldname].isBool()) return false;
  val = root[fieldname].asBool();
  return true;
}



template <>
bool json_read_<int>(const Json::Value& root, const char *fieldname, int& val)
{
  if (!root[fieldname].isInt()) return false;
  val = root[fieldname].asInt();
  return true;
}



template <>
bool json_read_<double>(const Json::Value& root, const char *fieldname, double& val)
{
  if (!root[fieldname].isDouble()) return false;
  val = root[fieldname].asDouble();
  return true;
}



template <>
bool json_read_<float>(const Json::Value& root, const char *fieldname, float& val)
{
  if (!root[fieldname].isDouble()) return false;
  val = static_cast<float>(root[fieldname].asDouble());
  return true;
}



template <>
bool json_read_<std::string>(const Json::Value& root, const char *fieldname, std::string& val)
{
  if (!root[fieldname].isString()) return false;
  val = root[fieldname].asString();
  return true;
}


template<typename T>
static
bool json_element_as(const Json::Value& root, T& val)
{
  throw "unsopported type";  // unsupported type
}


template<>
bool json_element_as<std::string>(const Json::Value& root, std::string& val)
{
  if (!root.isString()) return false;
  val = root.asString();
  return true;
}


template<>
bool json_element_as<double>(const Json::Value& root, double& val)
{
  if (!root.isDouble()) return false;
  val = root.asDouble();
  return true;
}


template<>
bool json_element_as<float>(const Json::Value& root, float& val)
{
  if (!root.isDouble()) return false;
  val = static_cast<float>(root.asDouble());
  return true;
}


template<>
bool json_element_as<int>(const Json::Value& root, int& val)
{
  if (!root.isInt()) return false;
  val = root.asInt();
  return true;
}



template <typename T>
static
bool json_read(const Json::Value& root, const char* fieldname, T& val)
{
  if (!json_has_field(root, fieldname)) {
    return false;
  }
  if (!json_read_<T>(root, fieldname, val)) {
    LOGF("type mismatch for field: %s", fieldname);
    return false;
  }
  return true;
}

template <typename T>
static
void json_read_required(const Json::Value& root, const char* fieldname, T& val)
{
  if (!json_read<T>(root, fieldname, val)) {
    RUNTIME_ERROR("missing required field: %s", fieldname);
  }
}

template <typename T>
bool json_read_v(const Json::Value& root, const char* fieldname, std::vector<T>& vec)
{
  if (!json_has_field(root, fieldname)) return false;
  const Json::Value& arr = root[fieldname];
  
  int sz = arr.size();
  if (sz == 0) return false;

  vec.resize(sz);
  for (int i = 0; i < sz; i++) {
    if (!json_element_as<T>(arr[i], vec[i])) 
      return false;  
  }

  return true;
}

template <typename T>
void json_read_v_required(const Json::Value& root, const char* fieldname, std::vector<T>& vec)
{
  if (!json_read_v<T>(root, fieldname, vec)) {
    RUNTIME_ERROR("missing required field: %s", fieldname);
  } 
}

static
bool json_read_bool_string_value(const Json::Value& root, const char *fieldname, bool& val)
{
  std::string str_val;
  if(!json_read<std::string>(root, fieldname, str_val))
    return false;

  val = (str_val == "true");
  return true;
}


// JSON to std::string helper
inline std::string json2str(const Json::Value& root)
{
  // create reply string in JSON format
  Json::FastWriter writer;
  return writer.write(root); 
}



// helper that returns a json data structure with: 
//  'status'        : 'error', 
//  'error_details' : < function argument: details >
static
std::string json_error_string(const char *format, ...)
{
  char error_str[512];
  
  // write log message in buffer
  va_list args;
  va_start (args, format);
  vsnprintf(error_str, sizeof(error_str), format, args);
  va_end(args);

  LOGF("json error: %s", error_str);
  
  Json::Value root;
  root["status"]        = "error";
  root["error_details"] = error_str;

  Json::FastWriter writer;
  return writer.write(root); 
}



static
void vjson_error(Json::Value& root, const char *format, va_list args)
{
  char errmsg[512];

  vsnprintf(errmsg, sizeof(errmsg), format, args);

  std::string errmsg_str(errmsg);
  root["status"] = "error";
  root["error_details"] = errmsg_str;
}



static
void json_error(Json::Value& root, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vjson_error(root, format, args);
  va_end(args);
}



static
Json::Value json_error(const char *format, ...)
{
  Json::Value root;

  va_list args;
  va_start(args, format);
  vjson_error(root, format, args);
  va_end(args);
  
  return root;
}




#pragma once

#include <cstdlib>
#include <cstdarg>
#include <stdexcept>
#include <string>


#define EXIT_ERROR(...) do {    \
		printf(__VA_ARGS__);        \
		exit(1);                    \
} while(0);

static
std::string stringf(const char *format, ...)
{
  char msg[256];
  
  // write log message in buffer
  va_list args;
  va_start (args, format);
  vsnprintf(msg, sizeof(msg), format, args);
  va_end(args);

  return std::string(msg); 
}

#define RUNTIME_ERROR(Format, ...) do { \
  	throw std::runtime_error(stringf((Format), # __VA_ARGS__).c_str()); } \
	while(0);

#define ERROR(Msg) do { \
    throw std::runtime_error(Msg); \
  } while(0)

#define VALIDATE_TRUE(Expr, Msg) do { \
	if (!(Expr)) \
		ERROR(Msg);	\
	} while(0)

#define VALIDATE_IN_RANGE(Expr, Low, High) do { \
	if ((Expr) < (Low) || (Expr) > (High)) \
		RUNTIME_ERROR("out of range for: " #Expr); \
	} while(0)

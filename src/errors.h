#pragma once

#include <stdexcept>

#define ERROR(Msg) do {            \
    throw std::runtime_error(Msg); \
  } while(0)


#define VALIDATE_TRUE(Expr, Msg) do { \
	if (!(Expr)) { 											\
		Error(Msg);												\
	} while(0)

#define VALIDATE_IN_RANGE(Expr, Low, High) do { \
	if ((Expr) < (Low) || (Expr) > (High)) { 	\
		Error("out of range for: " #Expr); 			\
	} while(0)



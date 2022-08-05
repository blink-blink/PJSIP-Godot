#pragma once

#include <Godot.hpp>

using namespace godot;

/**
* macros
*/

#define ERROR_CODE_0 "Account Exists"
#define ERROR_CODE_1 "Account Failed"

#define CHECK(op)   do { \
			status = op; \
			if (status != PJ_SUCCESS) { \
			    err(#op, status); \
			    return status; \
			} \
		    } \
		    while (0)
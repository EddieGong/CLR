#pragma once

// TODO: when there is no reference to the assert.h in the code, this file can be renamed to ASSERT
#define CLR_ASSERT_MSG(expr, msg) assert(expr && msg)

#define CLR_ASSERT(expr) assert(expr)
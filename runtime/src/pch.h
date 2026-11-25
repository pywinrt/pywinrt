// What the runtime precompiles: the C++/WinRT and Python headers that
// prelude.h brings, plus the small runtime headers most files use. Only
// target_precompile_headers() in runtime/CMakeLists.txt names this file, and
// nothing includes it: each file includes what it uses itself.

#pragma once

#include "errors.h"
#include "handles.h"
#include "prelude.h"
#include "wrappers.h"

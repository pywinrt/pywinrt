// The umbrella header for winrt-runtime's own C++ headers. A source file of
// the runtime includes this one and gets everything; each header below says
// what it is for, and they are listed in the order they depend on each other.
//
// It stays one include rather than one per header because the py::converter<T>
// specializations are spread over convert.h and array.h, and every
// one of them has to be declared before a source file instantiates any of
// them. Including this header is what guarantees that.

#pragma once

#include "prelude.h"

#include "handles.h"
#include "traits.h"
#include "wrappers.h"

#include "abi.h"
#include "errors.h"

#include "convert.h"
#include "buffer.h"
#include "array.h"

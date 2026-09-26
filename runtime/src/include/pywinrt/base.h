// The umbrella header for the PyWinRT runtime API. A module that compiles
// against winrt-runtime includes this one and gets everything; each header
// below says what it is for, and they are listed in the order they depend on
// each other.
//
// It stays one include rather than ten because the py::converter<T>
// specializations are spread over convert.h and array.h, and every
// one of them has to be declared before a module instantiates any of them.
// Including this header is what guarantees that.

#pragma once

#include <pywinrt/prelude.h>

#include <pywinrt/handles.h>
#include <pywinrt/traits.h>
#include <pywinrt/wrappers.h>

#include <pywinrt/abi.h>
#include <pywinrt/errors.h>

#include <pywinrt/async.h>
#include <pywinrt/convert.h>
#include <pywinrt/buffer.h>
#include <pywinrt/array.h>
#include <pywinrt/compose.h>

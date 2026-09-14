# winrt-runtime

This package provides the PyWinRT runtime including the `winrt.system` module.

It also ships the C++ headers that a projection module compiles against. Use
`winrt._include.get_include()` to locate them, together with the generated
headers from the `winrt-sdk` package:

```python
from setuptools import setup
from winrt._include import get_include
from winrt_sdk import get_include_dirs

setup(..., include_dirs=[get_include()] + get_include_dirs())
```

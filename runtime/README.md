# winrt-runtime

This package provides the PyWinRT runtime including the `winrt.system` module.

It also ships the C++ headers that a compiled module uses to hand WinRT values
to Python and take them back. A projection package is a table this runtime
interprets, so such a module needs nothing generated: it names the type it is
handing over and the runtime resolves the name. Use
`winrt._include.get_include()` to locate the headers, alongside the C++/WinRT
headers that the module brings itself:

```python
from setuptools import setup
from winrt._include import get_include

setup(..., include_dirs=[get_include(), "path/to/cppwinrt"])
```

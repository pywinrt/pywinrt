# winrt-sdk

This package provides PyWinRT header files for the Windows SDK.

This package is a build time dependency, not a runtime dependency.

Example use in a `pyproject.toml` file:

```toml
[build-system]
requires = ["setuptools", "setuptools-scm", "winrt-sdk"]
build-backend = "setuptools.build_meta"
```

Then in your `setup.py`:

```python
from setuptools import setup
from winrt_sdk import get_include_dirs

setup(
    ...
    include_dirs=get_include_dirs()
)
```

For the runtime package, use `winrt-runtime` instead.

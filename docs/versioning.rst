==========
Versioning
==========

PyWinRT attempts to use semantic versioning, however, due to the fact that there
are 100s of automatically generated packages, this is not always possible.

The PyWinRT code generation tool itself has a *major.minor.patch* version number.
This version is also used for the generated packages. In cases when we need to
make a bug fix for an individual package, an extra number is added, so we end
up with 4 segments: *major.minor.patch.package*.

If you are creating Python projections of your own WinRT components, you will
have to use the same versioning scheme since it is baked into the code generator.
In the future, we hope to relax the versioning requirements even more so that
custom components don't have to build a new release each time the PyWinRT code
generation tool is updated.

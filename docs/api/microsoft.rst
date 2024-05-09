================================
:mod:`winrt.microsoft` namespace
================================

The :mod:`winrt.microsoft` namespace package contains the automatically generated
bindings for the Windows App SDK. Each namespace is distributed as a separate package
on PyPI. For example, the ``winrt-Microsoft.Windows.UI`` package contains the
bindings for the ``Microsoft.Windows.UI`` namespace and can be imported in Python
as ``import winrt.microsoft.windows.ui``.

Since most of the code is generated, there currently aren't Python API docs.
You can use https://learn.microsoft.com/en-us/windows/windows-app-sdk/api/winrt/
instead, keeping in mind the Python naming conventions and other rules mentioned
in :doc:`../types` section.

.. tip:: The packages include type hints so you can use those if
    you are unsure of a return type or a parameter type.

------------
Boostrapping
------------

To use the Windows App SDK, you need to start the runtime at the beginning of
your program:

.. code-block:: python

    from winrt.microsoft.windows.applicationmodel.dynamicdependency import bootstrap

    # initialize Windows App runtime and prompt user if it is not installed
    with bootstrap.initialize(options=bootstrap.InitializeOptions.ON_NO_MATCH_SHOW_UI):
        # your main entry point code here
        ...

---------------
Interop modules
---------------

There are also special modules that provide extra functionality
to bridge between WinRT and other interfaces.

.. toctree::
   :maxdepth: 2

   microsoft/windows/applicationmodel/dynamicdepedency/bootstrap

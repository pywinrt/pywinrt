================================
:mod:`winrt.microsoft` namespace
================================

The :mod:`winrt.microsoft` namespace package contains the automatically
generated bindings for the Windows App SDK — also known as WinUI 3 — and for
the WebView2 control. Each is published on PyPI as one distribution per NuGet
package it comes from: ``winrt-Microsoft.WindowsAppSDK.WinUI`` carries every
namespace the App SDK's WinUI component defines, including
``Microsoft.UI.Xaml``, which is imported in Python as
``winrt.microsoft.ui.xaml``.

Since most of the code is generated, there currently aren't Python API docs.
You can use https://learn.microsoft.com/en-us/windows/windows-app-sdk/api/winrt/
instead, keeping in mind the Python naming conventions and other rules mentioned
in :doc:`../types` section.

.. tip:: The packages include type hints so you can use those if
    you are unsure of a return type or a parameter type.

.. versionchanged:: 3.0
    The top-level package name has been changed from ``winrt`` to ``winui3``.

.. versionchanged:: 4.0
    The top-level package name has been changed back from ``winui3`` to
    ``winrt``, and the Windows App SDK packages are published one per NuGet
    component instead of one per WinRT namespace.

---------------
Windows App SDK
---------------

.. danger:: None of the Windows App SDK packages can be used if the Windows
    App Runtime is not installed! You must manually install this from `here <download>`_.
    Also read the `Boostrapping`_ section below for how to initialize the runtime.

.. _download: https://learn.microsoft.com/en-us/windows/apps/windows-app-sdk/downloads

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

.. warning:: If you are using a Python runtime installed rom the Microsoft Store,
    this method will fail with ``OSError(winerror=-2147009196)`` (``ERROR_NOT_SUPPORTED = -2147009196``).
    This happens because the Microsoft Store version of Python is a "packaged" app
    and expects the dependency to be included in the package manifest. This should
    be able to be worked around using other dynamic dependency APIs, but this has
    not been extensively tested yet. See `Microsoft's Docs`_ for more information.

.. _Microsoft's Docs: https://learn.microsoft.com/en-us/windows/apps/desktop/modernize/framework-packages/use-the-dynamic-dependency-api

---------------
Interop modules
---------------

There are also special modules that provide extra functionality
to bridge between WinRT and other interfaces.

.. toctree::
   :maxdepth: 2

   microsoft/ui/interop
   microsoft/windows/applicationmodel/dynamicdepedency/bootstrap

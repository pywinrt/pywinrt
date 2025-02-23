=======================
:mod:`winui3` namespace
=======================

The :mod:`winui3` namespace package contains the automatically generated
bindings for the Windows App SDK, also known as WinUI 3. Each API namespace is
distributed as a separate package on PyPI. For example, the ``winui3-Microsoft.UI.Xaml``
package contains the bindings for the APIs in the ``Microsoft.UI.Xaml`` namespace
and can be imported in Python as ``import winui3.microsoft.windows.ui.xaml``.

Since most of the code is generated, there currently aren't Python API docs.
You can use https://learn.microsoft.com/en-us/windows/windows-app-sdk/api/winrt/
instead, keeping in mind the Python naming conventions and other rules mentioned
in :doc:`../types` section.

.. tip:: The packages include type hints so you can use those if
    you are unsure of a return type or a parameter type.

.. versionchanged:: unreleased
    The top-level package name has been changed from ``winrt`` to ``winui3``.

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

   microsoft/ui/interop
   microsoft/windows/applicationmodel/dynamicdepedency/bootstrap

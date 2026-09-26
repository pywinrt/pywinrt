=========================
Freezing an application
=========================

A frozen application needs two things from PyWinRT that a freezer cannot work
out by reading your source.

**A projected namespace is a data file.** ``winrt.windows.storage`` is a small
``__init__.py`` beside a ``_table.pywinrt`` that describes the namespace, and
importing the module asks the runtime to read that table. A freezer that
collects the module and not the table produces an executable that fails on the
first import.

**A projected type is found by importing its module by name while the program
runs.** A call that hands back an ``IAsyncOperation`` imports
``winrt.windows.foundation`` whether or not your code ever mentions it. Nothing
in the source says so, so a freezer that follows imports alone will leave it
out.

There is also one rule that applies whatever tool you use: **the table has to
stay a real file.** The runtime opens it and maps it into memory, so a package
that has been packed into a zip cannot be read, however complete the archive
is.


-----------
PyInstaller
-----------

Nothing to configure. ``winrt-runtime`` ships a hook and registers it in the
``pyinstaller40`` entry point group, so PyInstaller finds it on its own:

.. code-block:: text

   pyinstaller app.py

The hook collects every ``_table.pywinrt`` of every installed projection
package, adds each of those packages as a hidden import, and collects the
component ``.dll`` that a redistribution package carries — that one is loaded
by bare name after its directory is added to the DLL search path, so no import
table mentions it.

It collects every installed projection package, not only the ones your program
imports, because of the second point above: what an application imports says
little about which namespaces its calls return types from. If that is more than
you want in your executable, install fewer projection packages in the
environment you freeze from — the hook collects what is installed, so a virtual
environment with only what your application needs produces the smallest result.


---------
cx_Freeze
---------

cx_Freeze takes no hook of this shape, so the two points above become
configuration. Name each projection package your application uses so that the
ones imported at run time are included, keep them out of the zip so their
tables stay readable, and copy the tables in:

.. code-block:: python

   from cx_Freeze import setup, Executable

   setup(
       executables=[Executable("app.py")],
       options={
           "build_exe": {
               "packages": ["winrt.windows.foundation", "winrt.windows.storage"],
               "zip_exclude_packages": ["winrt"],
           }
       },
   )

``zip_exclude_packages`` is the part that is easy to miss: without it the
package is put in ``library.zip``, the table goes with it, and the import
fails even though the file is there.


------
Nuitka
------

Nuitka needs the same two things said in its own options — the packages, so
that the ones imported by name are compiled in, and their data files, so the
tables travel with them:

.. code-block:: text

   nuitka --standalone --include-package=winrt --include-package-data=winrt app.py

``--include-package-data`` is what carries the ``_table.pywinrt`` files.
Nuitka writes a standalone distribution as ordinary files rather than an
archive, so the rule about the table staying a real file takes care of itself.


---------------------
Checking what you got
---------------------

Whichever tool you used, the check is the same: run the executable on a
machine that has no PyWinRT installed, rather than in the environment you
built it from. A frozen application that imports a projection package usually
works in the build environment either way, because the interpreter there can
still find the installed copy.

A module whose table did not travel with it fails at the import of that
module::

   File "winrt\windows\foundation\__init__.py", line 8, in <module>
   File "winrt\runtime\_internals.py", line 56, in load_projection
   FileNotFoundError: [WinError -2147024894] The system cannot find the file specified.

The traceback names the module; the file it could not find is the
``_table.pywinrt`` that belongs beside it.

A namespace that was not collected at all fails later and looks like
something else entirely - at the call that would have returned a type from
it, and often as a missing attribute with a private name in it::

   AttributeError: module 'winrt.windows.foundation.collections' has no attribute '_IIterator'

That second one is why it is worth exercising the paths your application
actually takes, rather than only checking that it starts.

.. seealso:: :doc:`versioning`

"""
What a frozen application needs from PyWinRT that PyInstaller cannot see.

Two things are invisible to an import graph.

A projected namespace is a data file. ``winrt.windows.storage`` is an
``__init__.py`` that asks the runtime to read the ``_table.pywinrt`` sitting
beside it, so freezing the module without the table produces an executable
that fails at import. PyInstaller collects the module and not the table.

A projected type is resolved by importing the module that owns it, by name,
while the program runs (``py::get_python_type()`` in the runtime). A call
that hands back an ``IAsyncOperation`` imports ``winrt.windows.foundation``
whether or not the application ever names it, and nothing in the source says
so. So every projection package that is installed is collected rather than
only the ones the graph reached: what an application imports says little
about which namespaces its calls return types from.

Nothing here names a Windows SDK, a Windows App SDK or any other family. What
is collected is what the installed distributions say they ship, so a
projection somebody else generated is collected on the same terms as the ones
published from this repository.
"""

from importlib.metadata import distributions

# the data file a projection package carries, which
# winrt.runtime._internals.load_projection() reads from beside the module
TABLE_NAME = "_table.pywinrt"

binaries: list[tuple[str, str]] = []
datas: list[tuple[str, str]] = []
hiddenimports: list[str] = []


def _installed_files() -> list[tuple[str, str, str]]:
    """
    Every file of every installed distribution, as (name, package, source).

    ``package`` is the dotted package the file sits in and ``source`` is where
    it is now. A distribution installed from a wheel records what it put where;
    one that did not record it is something this can say nothing about.
    """
    files = []

    for distribution in distributions():
        for file in distribution.files or []:
            directory = str(file).replace("\\", "/").rpartition("/")[0]

            if directory:
                files.append(
                    (file.name, directory, str(distribution.locate_file(file)))
                )

    return files


def _collect() -> None:
    installed = _installed_files()

    # The namespace roots PyWinRT projects into are whichever ones hold a
    # table, rather than a list of names kept here: the runtime has no
    # business knowing which families exist.
    roots = {
        package.partition("/")[0]
        for name, package, _ in installed
        if name == TABLE_NAME
    }

    for name, package, source in installed:
        if package.partition("/")[0] not in roots:
            continue

        if name == TABLE_NAME:
            datas.append((source, package))
            hiddenimports.append(package.replace("/", "."))

        # A package that redistributes a component .dll loads it by bare name
        # after putting its own directory on the DLL search path, so no import
        # table mentions it and nothing else would collect it.
        if name.lower().endswith(".dll"):
            binaries.append((source, package))


_collect()

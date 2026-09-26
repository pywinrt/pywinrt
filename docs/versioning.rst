==========
Versioning
==========

A PyWinRT version says which metadata a package was generated from, not which
version of the code generator read that metadata. There are two schemes,
because there are two kinds of package, and one number that both of them
carry.

.. note::

   This describes PyWinRT 4.0 and later. Versions up to 3.2.1 gave every
   package the code generator's own version, so upgrading the generator meant
   republishing everything. See `Moving from 3.x`_.


------------------------
What a version tells you
------------------------

A **projection package** is generated from a WinRT metadata release, so it is
versioned after that release:

.. code-block:: text

   winrt-Windows.Storage                 4!10.0.28000.2705   Windows SDK
   winui2-Microsoft.UI.Xaml              4!2.8.7             Microsoft.UI.Xaml
   winrt-Microsoft.WindowsAppSDK.WinUI   4!2.5.1             Windows App SDK
   winrt-Microsoft.Web.WebView2          4!1.0.4191.47       WebView2

Reading one of these tells you exactly which upstream release the API surface
came from, which is otherwise invisible from Python. Packages generated from
one upstream release are published together and carry the same version.

A **package written by hand** has no upstream release to follow, so it uses
semantic versioning and is released on its own when it changes:

.. code-block:: text

   winrt-runtime                         4.0.0
   winrt-table-compiler                  4.0.0
   winrt-Windows.System.Interop          4.0.0

These all start at ``4.0.0``, and from there each moves only when that
package changes: a fix to ``winrt-runtime`` does not renumber the interop
modules, and moving the Windows SDK pin does not renumber any of them.

The ``4!`` on the first group is a :pep:`440` *epoch*, and the leading ``4``
on the second is the major. They are the same number.


----------------------------
The compatibility generation
----------------------------

That number is the compatibility generation. It appears in three places that
must agree — the epoch of every projection package, the C ABI the compiled
packages are built against, and the format of the table a projection package
carries — because a change to any of them breaks every projection package at
once. Rather than three numbers that have to be reasoned about together, it
is one number that says: *these packages work with each other, and with no
runtime of another generation.*

An epoch sorts above everything published before it, whatever the release
segments say, and that is what makes these numbers usable at all. An upstream
line is numbered for its own reasons, and several of them are below the
``3.2.1`` that PyWinRT itself last published: the Windows App SDK is on 2.x
and WebView2 on 1.x. ``2.5.1`` would be an upgrade to nothing, while
``4!2.5.1`` is unambiguously newer than ``3.2.1``. The epoch is what lets a
package be numbered after its upstream without its history running
backwards.

The generation changes rarely, and when it does every projection package is
republished.


----------------------------------
What has to be new enough, and why
----------------------------------

Two things are checked at run time, and both come out the same way:
**winrt-runtime has to be at least as new as what it is running.**

- A compiled package — ``winrt-runtime`` itself and the interop modules —
  reaches the runtime through a C ABI. A module built against ABI *4.m* needs
  a runtime whose ABI minor is at least *m*; entry points are only ever
  appended, so an older module keeps working against a newer runtime.
- A projection package carries a table describing its namespace. The runtime
  refuses a table whose format minor is higher than the one it reads, and
  reads an older table happily.

In both cases the newer half may be the runtime and never the other side. The
``>=`` floors in every package's metadata say so, which is why pip normally
gets this right on its own: installing a newer projection package pulls a
runtime new enough to read it.

If the two are forced apart anyway — with ``--no-deps``, a stale lock file, or
a vendored copy — the failure is an exception at import that names both
versions, not a crash:

.. code-block:: text

   RuntimeError: winrt._winrt._C_API ABI minor version mismatch: expected >= 8, got 3
   ImportError: table format version 4.3 is newer than this runtime, which reads 4.0

The fix for both is to upgrade ``winrt-runtime``.


------------
How to pin
------------

**Libraries** should use floors and no upper bounds:

.. code-block:: text

   winrt-Windows.Foundation>=4!10.0.28000
   winrt-runtime>=4.0

A cap on a dependency you have not seen break is a guess, and in a library it
becomes everyone else's problem to resolve. PyWinRT's own metadata contains
exactly one upper bound — ``winrt-runtime<5`` — because that incompatibility
is known rather than guessed, and everything else leans on it. A projection
package of the next generation cannot be installed beside a runtime of this
one, so the cap is doing the work a cap on the projection package would.

**Applications** should pin exactly, with a lock file if the tooling has one.
An application controls its whole environment, so reproducibility is worth
more than resolver freedom.

**An application tied to a Windows App SDK line** can pin to that line, since
the version says which one it is:

.. code-block:: text

   winrt-Microsoft.WindowsAppSDK.WinUI==4!2.5.*

Pinning a Windows SDK package to a line is rarely worth it, because Windows
APIs are almost always additive: a newer projection has what an older one had.
The exception is a namespace Microsoft withdraws, whose package stops being
published — there is no newer version of it to move to, so a pin would not
have helped either.


-------------------
Re-releases: .postN
-------------------

``.postN`` on a projection package means PyWinRT republished the same upstream
release — to fix something in the packaging, not in the API surface.
``4!10.0.28000.2705.post1`` and ``4!10.0.28000.2705`` project the same Windows
SDK. Nothing else uses the segment; a prerelease of an upstream release is
spelled the way :pep:`440` spells one, so ``1.8.0-preview2`` upstream is
published as ``4!1.8.0b2``.


-------------------
Deprecation policy
-------------------

Open floors are only safe if upgrading does not break code without warning, so
an API that is removed or renamed goes through stages rather than
disappearing:

1. it keeps working and raises ``DeprecationWarning``, for at least a year;
2. the warning becomes ``FutureWarning``, which is shown to end users rather
   than only to developers;
3. it is removed.

A compatibility alias is carried across at least one major version, and is
marked with ``@warnings.deprecated`` in the type stubs, so a type checker
reports it before anything is run.


--------------------------------------
Projections of your own components
--------------------------------------

If you generate a projection of your own WinRT component, its version is
yours — nothing about PyWinRT's scheme is baked into what you publish, which
was not true before 4.0.

What you do have to track is the generation. Your package carries a table, so
it depends on ``winrt-runtime`` of the generation it was built for, and it is
built by ``winrt-table-compiler``, which writes that generation's table
format. Both are published packages, so a build of your own needs no copy of
the PyWinRT sources:

.. code-block:: toml

   [build-system]
   requires = ["hatchling", "winrt-table-compiler>=4.0.0,<5"]

   [project]
   dependencies = ["winrt-runtime>=4.0.0,<5"]

Generating the projection in the first place needs ``PyWinRT.exe`` and the
shape census it ships beside, which records the ABI call shapes the runtime
has trampolines for. A component whose members need a shape the census does
not have cannot be projected until that shape is added to the runtime.


----------------
Moving from 3.x
----------------

Up to 3.2.1 every package carried the code generator's version, with a fourth
segment added to fix one package on its own, and a projection of your own
components had to use the same scheme. None of that applies now.

The two generations never mix: a 3.x projection package requires
``winrt-runtime~=3.2.1.0``, so pip cannot pair one with a 4.x runtime. The
forward promise above — that a runtime runs anything of its generation that is
no newer than itself — begins at 4.0 and says nothing about 3.x.

.. seealso:: :doc:`types`

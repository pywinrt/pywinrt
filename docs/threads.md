# Threads and apartments

WinRT objects can be used from more than one Python thread, but WinRT ties some
things to a thread. This page says what that means for a program.

## Apartments

WinRT runs every thread in a COM apartment. A program without a user interface
needs to do nothing about it. A thread that has not chosen an apartment uses
WinRT in the process's multithreaded apartment, which the runtime joins the
first time it activates a class.

A thread that runs a user interface is a single-threaded apartment instead. Call
[`init_apartment()`](api/runtime.md#init_apartment) on it before it creates any
of the interface:

```python
from winrt.runtime import ApartmentType, init_apartment

init_apartment(ApartmentType.SINGLE_THREADED)
```

The WinUI 2 and WebView2 samples do this before they create their window.

## What a single-threaded apartment changes

The blocking [`get()`](types.md#get) and [`wait()`](types.md#wait) of an async
operation raise `RuntimeError` on such a thread, since blocking it would freeze
the interface. Await the operation instead, or give it a `completed` handler.

A XAML element belongs to the thread that created it and is used on that
thread. A call to it from any other thread fails with `OSError` and
`RPC_E_WRONG_THREAD`.

## Packages that choose the apartment on import

Some Python packages for COM make the thread that first imports them a
single-threaded apartment. pywin32's `pythoncom`, which `win32com` imports, and
`comtypes` both do. A program that imports one of them, even through another
package, gets the changes above on that thread, so a `get()` that worked before
raises `RuntimeError`.

Both packages read `sys.coinit_flags` when they are imported. Setting it to
`0`, which is `COINIT_MULTITHREADED`, before the first import leaves the thread
in the multithreaded apartment:

```python
import sys

sys.coinit_flags = 0  # COINIT_MULTITHREADED

import win32com.client
```

## Which thread runs your code

A handler that WinRT calls, for an event or for an operation's `completed` or
`progress`, runs on whatever thread WinRT calls it from, which is often not one
of the program's own.
[Which thread calls the handler](types.md#which-thread-calls-the-handler) says
how to hand the work to the thread or event loop that should do it.

## Free-threaded Python

PyWinRT runs on the free-threaded build of CPython, where threads run Python
code at the same time. Import the WinRT modules a program uses before it starts
threads, or from one thread, as most programs do at the top of a module anyway.
Two threads importing WinRT modules that need each other for the first time at
once can fail with an import deadlock error, as with any Python package whose
modules import each other. [`Array`](api/system.md#array) says what it
guarantees when several threads use one.

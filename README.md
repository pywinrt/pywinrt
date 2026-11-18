<img src="PyWinRT/nuget/icon.png" alt="logo" align="right" />

# PyWinRT

[![winrt-runtime](https://img.shields.io/pypi/v/winrt-runtime?label=winrt-runtime)](https://pypi.org/project/winrt-runtime/)
[![test](https://github.com/pywinrt/pywinrt/actions/workflows/test.yaml/badge.svg?branch=main)](https://github.com/pywinrt/pywinrt/actions/workflows/test.yaml)
[![docs](https://readthedocs.org/projects/pywinrt/badge/?version=latest)](https://pywinrt.readthedocs.io)

PyWinRT lets Python call Windows Runtime (WinRT) APIs: Bluetooth, USB and
serial devices, cameras, notifications, screen capture, speech and many other
APIs that Windows offers apps.

- **A projection:** it is generated from the WinRT metadata, so every API of
  the Windows SDK is there, and the Windows App SDK's too.
- **Pythonic:** `snake_case` names, collections that are sequences and
  mappings, async operations you `await` and any bytes-like object where WinRT
  takes a buffer, as [the WinRT type system](https://pywinrt.readthedocs.io/en/latest/types/)
  describes.
- **Fully typed:** every package ships type hints, so editors complete your
  code and type checkers check it.

## Installing

Each part of the WinRT API is a package of its own on PyPI. Install the ones
your code uses, and each brings the runtime with it:

```console
pip install winrt-Windows.Media.SpeechSynthesis
```

PyWinRT needs CPython 3.11 or later on Windows.
[Installing](https://pywinrt.readthedocs.io/en/latest/installing/) says which
packages to choose.

## Example

```python
from winrt.windows.media.speechsynthesis import SpeechSynthesizer

for voice in SpeechSynthesizer.all_voices:
    print(voice.display_name, voice.language)
```

To hear the default voice, install [uv](https://docs.astral.sh/uv/) and run
the [text-to-speech sample](samples/text_to_speech.py):

```console
uv run --script https://raw.githubusercontent.com/pywinrt/pywinrt/refs/heads/main/samples/text_to_speech.py
```

The [documentation](https://pywinrt.readthedocs.io) covers the rest. You can
also write [WinUI 3 apps](samples/winui3/hello_app.py). The
[PyWinRT tool](https://www.nuget.org/packages/PyWinRT) projects WinRT
components of your own, and
[Versioning](https://pywinrt.readthedocs.io/en/latest/versioning/#projections-of-your-own-components)
says how to package them.

## More

- **Moving from 3.x:** read the
  [4.0 release notes](https://pywinrt.readthedocs.io/en/latest/release-notes/4.0/)
  and the [migration guide](scripts/3to4/README.md).
- **Built on PyWinRT:** [Bleak](https://github.com/hbldh/bleak) and
  [Windows-Toasts](https://github.com/DatGuy1/Windows-Toasts).
- **Support:** ask questions in
  [Discussions](https://github.com/pywinrt/pywinrt/discussions) and report bugs
  in [Issues](https://github.com/pywinrt/pywinrt/issues).
- **Contributing:** see [CONTRIBUTING.md](CONTRIBUTING.md).
- **License:** [MIT](LICENSE).
- **History:** PyWinRT began as a fork of the `pywinrt` code generator from
  Microsoft's [xlang](https://github.com/microsoft/xlang/) project and is now an
  independent project.
  [History](https://pywinrt.readthedocs.io/en/latest/history/) tells the rest.

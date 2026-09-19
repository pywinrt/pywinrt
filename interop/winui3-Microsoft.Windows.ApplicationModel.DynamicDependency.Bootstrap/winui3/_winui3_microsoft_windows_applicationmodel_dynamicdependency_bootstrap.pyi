from typing import Self

from winui3.microsoft.windows.applicationmodel.dynamicdependency.bootstrap import (
    InitializeOptions,
)

RELEASE_VERSION: str
RUNTIME_VERSION: str

class Shutdown:
    def __call__(self) -> None: ...
    def __enter__(self) -> Self: ...
    def __exit__(self, *args) -> None: ...

def initialize(
    major_minor_version: int,
    version_tag: str | None,
    min_version: int,
    options: InitializeOptions,
) -> Shutdown: ...

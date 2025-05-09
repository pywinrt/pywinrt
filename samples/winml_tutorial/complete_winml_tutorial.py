"""
A port of the WinML C++/WinRT tutorial to Python

https://docs.microsoft.com/en-us/windows/ai/get-started-desktop
"""

import asyncio
import csv
import functools
import os
import time
from pathlib import Path
from typing import Any, Callable, Coroutine, Iterable, Sequence, TypeVar, cast

import winrt.windows.ai.machinelearning as winml
from winrt.windows.graphics.imaging import BitmapDecoder
from winrt.windows.media import VideoFrame
from winrt.windows.storage import StorageFile

if os.environ.get("MSYSTEM", ""):
    _getfullpathname: Callable[[str], str]
    from nt import _getfullpathname  # type: ignore

    # HACK: MSYS hacks ntpath module to use unix directory separators when the
    # MSYSTEM environment variable is present, so we have to work around that.
    def abspath(path: os.PathLike[str]) -> str:
        return _getfullpathname(os.path.normpath(path))
else:
    abspath = os.path.abspath

TCallable = TypeVar("TCallable", bound=Callable[..., Any])
TCoroutine = TypeVar("TCoroutine", bound=Callable[..., Coroutine[Any, Any, Any]])


def timed_op(fun: TCallable) -> TCallable:
    @functools.wraps(fun)
    def wrapper(*args: Any, **kwds: Any) -> Any:
        print("Starting", fun.__name__)
        start = time.perf_counter()

        ret = fun(*args, **kwds)

        end = time.perf_counter()
        print(f"{fun.__name__} took {end - start:.3f} seconds")

        return ret

    return cast(TCallable, wrapper)


def timed_async_op(fun: TCoroutine) -> TCoroutine:
    @functools.wraps(fun)
    async def wrapper(*args: Any, **kwds: Any) -> Any:
        print("Starting", fun.__name__)
        start = time.perf_counter()

        ret = await fun(*args, **kwds)

        end = time.perf_counter()
        print(f"{fun.__name__} took {end - start:.3f} seconds")

        return ret

    return cast(TCoroutine, wrapper)


@timed_op
def load_model(model_path: Path) -> winml.LearningModel:
    return winml.LearningModel.load_from_file_path(abspath(model_path))


@timed_async_op
async def load_image_file(file_path: Path) -> VideoFrame:
    file = await StorageFile.get_file_from_path_async(abspath(file_path))

    with await file.open_read_async() as stream:
        decoder = await BitmapDecoder.create_async(stream)
        software_bitmap = await decoder.get_software_bitmap_async()

    return VideoFrame.create_with_software_bitmap(software_bitmap)


@timed_op
def bind_model(
    model: winml.LearningModel, image_frame: VideoFrame
) -> tuple[winml.LearningModelSession, winml.LearningModelBinding]:
    device = winml.LearningModelDevice(winml.LearningModelDeviceKind.DEFAULT)
    session = winml.LearningModelSession(model, device)
    binding = winml.LearningModelBinding(session)
    image_feature_value = winml.ImageFeatureValue.create_from_video_frame(image_frame)
    binding.bind("data_0", image_feature_value)
    shape = winml.TensorFloat.create2([1, 1000, 1, 1])
    binding.bind("softmaxout_1", shape)
    return session, binding


@timed_op
def evaluate_model(
    session: winml.LearningModelSession, binding: winml.LearningModelBinding
) -> Sequence[float]:
    results = session.evaluate(binding, "RunId")
    result_tensor = results.outputs["softmaxout_1"].as_(winml.TensorFloat)
    return result_tensor.get_as_vector_view()


def load_labels(labels_path: Path) -> dict[int, str]:
    with open(labels_path) as labels_file:
        return {
            int(index): ", ".join(labels) for index, *labels in csv.reader(labels_file)
        }


def print_results(results: Iterable[float], labels: dict[int, str]):
    for confidence, label in sorted(zip(results, labels), reverse=True)[:3]:
        print(f"{labels[label]} ({confidence * 100:.1f}%)")


@timed_async_op
async def async_main() -> None:
    winml_content_dir = (Path(__file__).parent / "winml_content").resolve()

    model = load_model(winml_content_dir / "SqueezeNet.onnx")
    image_frame = await load_image_file(winml_content_dir / "kitten_224.png")
    session, binding = bind_model(model, image_frame)
    results = evaluate_model(session, binding)
    labels = load_labels(winml_content_dir / "Labels.txt")

    print_results(results, labels)


asyncio.run(async_main())

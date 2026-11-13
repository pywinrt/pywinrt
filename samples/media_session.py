# /// script
# requires-python = ">=3.11"
# dependencies = [
#   "winrt-runtime>=4",
#   "winrt-Windows.Foundation",
#   "winrt-Windows.Foundation.Collections",
#   "winrt-Windows.Media.Control",
#   "winrt-Windows.Storage.Streams",
# ]
# ///

"""
Example of how to see and control what is playing on the PC, whichever app is
playing it, with the System Media Transport Controls (SMTC) session manager.
This is what the media flyout of Windows and the media keys use.

Run this example from the web with:
uv run --script https://raw.githubusercontent.com/pywinrt/pywinrt/main/samples/media_session.py

Without arguments it lists the media sessions and prints what the current one
is playing. Play something in a browser or a music app first.

    --app NAME          use the session whose app id contains NAME
    --thumbnail FILE    save the artwork of the current track to FILE
    --watch             keep running and print every change, until Ctrl+C
    play-pause | next | previous
                        send that command to the session first
"""

import argparse
import asyncio
from datetime import timedelta

from winrt.system import Object
from winrt.windows.media.control import (
    GlobalSystemMediaTransportControlsSession as Session,
    GlobalSystemMediaTransportControlsSessionManager as SessionManager,
)
from winrt.windows.storage.streams import Buffer, InputStreamOptions


def find_session(manager: SessionManager, app: str | None) -> Session | None:
    """
    Lists the sessions and returns the one to use.

    A session is identified by the AppUserModelID of the app that owns it. That
    is a package id such as ``SpotifyAB.SpotifyMusic_zpdnekdrzrea0!Spotify``
    for a packaged app, and can be as little as ``chrome.exe`` for a desktop
    app, so matching on a part of it is the practical way to find an app.
    """
    current = manager.get_current_session()
    current_id = current.source_app_user_model_id if current else None
    found = None

    for session in manager.get_sessions():
        app_id = session.source_app_user_model_id
        print("*" if app_id == current_id else " ", app_id)

        if app and app.lower() in app_id.lower() and found is None:
            found = session

    # Without --app, the one Windows itself considers current: usually the
    # session that started playing last.
    return found if app else current


def clock(time: timedelta) -> str:
    minutes, seconds = divmod(int(time.total_seconds()), 60)
    return f"{minutes}:{seconds:02}"


async def describe(session: Session) -> str:
    properties = await session.try_get_media_properties_async()
    status = session.get_playback_info().playback_status
    timeline = session.get_timeline_properties()

    # The timeline is a snapshot, not a clock: the position was true at
    # timeline.last_updated_time, and an app updates it when it likes, some
    # every second and some only when playback starts or seeks. Not every app
    # reports one at all; a live stream has none to report.
    time = ""
    if timeline.end_time:
        time = f"  {clock(timeline.position)} / {clock(timeline.end_time)}"

    # The properties are None when the app has not supplied any (yet).
    track = f"{properties.artist} - {properties.title}" if properties else "?"

    return f"[{status.name}] {track}{time}"


async def save_thumbnail(session: Session, path: str) -> bool:
    properties = await session.try_get_media_properties_async()

    if properties is None or properties.thumbnail is None:
        return False

    # The stream is closed at the end of the block, like a Python file.
    with await properties.thumbnail.open_read_async() as stream:
        buffer = Buffer(stream.size)
        await stream.read_async(buffer, buffer.capacity, InputStreamOptions.READ_AHEAD)

    # A WinRT buffer supports the Python buffer protocol, so its bytes can be
    # used directly. The format is whatever the app supplied, PNG or JPEG.
    with open(path, "wb") as file:
        file.write(memoryview(buffer)[: buffer.length])

    return True


async def watch(session: Session) -> None:
    loop = asyncio.get_running_loop()
    changed = asyncio.Event()

    # WinRT calls an event handler on a thread of its own, and a handler has
    # to be a plain function, so it only passes the news on to the event loop.
    def on_changed(sender: Session, args: Object) -> None:
        loop.call_soon_threadsafe(changed.set)

    properties_token = session.add_media_properties_changed(on_changed)
    playback_token = session.add_playback_info_changed(on_changed)

    try:
        while True:
            await changed.wait()
            changed.clear()
            print(await describe(session))
    finally:
        session.remove_media_properties_changed(properties_token)
        session.remove_playback_info_changed(playback_token)


async def main() -> None:
    parser = argparse.ArgumentParser(description="Show what is playing on the PC.")
    parser.add_argument(
        "command", nargs="?", choices=["play-pause", "next", "previous"]
    )
    parser.add_argument("--app", help="use the session whose app id contains this")
    parser.add_argument("--thumbnail", metavar="FILE", help="save the artwork here")
    parser.add_argument("--watch", action="store_true", help="print every change")
    args = parser.parse_args()

    manager = await SessionManager.request_async()
    session = find_session(manager, args.app)

    if session is None:
        print("No such media session. Play something first.")
        return

    if args.command:
        # The result says whether the app accepted the command, which it does
        # not when, for example, there is no next track.
        accepted = await {
            "play-pause": session.try_toggle_play_pause_async,
            "next": session.try_skip_next_async,
            "previous": session.try_skip_previous_async,
        }[args.command]()
        print(args.command, "accepted" if accepted else "refused")

    print(await describe(session))

    if args.thumbnail:
        saved = await save_thumbnail(session, args.thumbnail)
        print("Saved" if saved else "No thumbnail to save to", args.thumbnail)

    if args.watch:
        await watch(session)


try:
    asyncio.run(main())
except KeyboardInterrupt:
    pass

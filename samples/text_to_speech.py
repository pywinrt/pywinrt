# /// script
# requires-python = ">=3.10"
# dependencies = [
#   "winrt-runtime",
#   "winrt-Windows.Foundation",
#   "winrt-Windows.Media.SpeechSynthesis",
#   "winrt-Windows.Media.Playback",
#   "winrt-Windows.Storage",
#   "winrt-Windows.Storage.Streams",
# ]
# ///

import asyncio

from winrt.system import Object
from winrt.windows.media.speechsynthesis import SpeechSynthesizer
from winrt.windows.media.playback import MediaPlayer, MediaPlayerAudioCategory


async def main():
    synth = SpeechSynthesizer()
    stream = await synth.synthesize_text_to_stream_async("Hello, World!")

    media_ended_event = asyncio.Event()
    loop = asyncio.get_running_loop()

    def on_media_ended(sender: MediaPlayer, args: Object):
        loop.call_soon_threadsafe(media_ended_event.set)

    player = MediaPlayer()
    player.audio_category = MediaPlayerAudioCategory.SPEECH

    player.set_stream_source(stream)
    player.add_media_ended(on_media_ended)
    player.play()

    await media_ended_event.wait()


asyncio.run(main())

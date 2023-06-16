import asyncio
from pypresence import Presence # The simple rich presence client in pypresence
import time


client_id = "1118822819090940007"  # Put your Client ID in here
RPC = Presence(client_id)  # Initialize the Presence client

from winsdk.windows.media.control import \
    GlobalSystemMediaTransportControlsSessionManager as MediaManager

RPC.connect() # Start the handshake loop

async def get_media_info():
    sessions = await MediaManager.request_async()

    # This source_app_user_model_id check and if statement is optional
    # Use it if you want to only get a certain player/program's media
    # (e.g. only chrome.exe's media not any other program's).

    # To get the ID, use a breakpoint() to run sessions.get_current_session()
    # while the media you want to get is playing.
    # Then set TARGET_ID to the string this call returns.

    current_session = sessions.get_current_session()
    if current_session:  # there needs to be a media session running
        info = await current_session.try_get_media_properties_async()

        # song_attr[0] != '_' ignores system attributes
        info_dict = {song_attr: info.__getattribute__(song_attr) for song_attr in dir(info) if song_attr[0] != '_'}

        # converts winrt vector to list
        info_dict['genres'] = list(info_dict['genres'])

        return info_dict

    # It could be possible to select a program from a list of current
    # available ones. I just haven't implemented this here for my use case.
    # See references for more information.
    raise Exception('TARGET_PROGRAM is not the current media session')

while True:  # The presence will stay on as long as the program is running
    current_media_info = asyncio.run(get_media_info())
    RPC.update(details=current_media_info["title"], state=f"by {current_media_info['artist']}") # Updates our presence
    time.sleep(15) # Can only update rich presence every 15 seconds
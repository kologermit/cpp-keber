# Встроенные модули
from enum import IntEnum
from json import loads

class DownloadType(IntEnum):
    AUDIO = 0
    VIDEO_720P = 1
    VIDEO_BEST = 2
    PLAYLIST_AUDIO = 3
    PLAYLIST_VIDEO_720P = 4
    PLAYLIST_VIDEO_BEST = 5

class Message:
    def __init__(self, payload: str, handle_id: int):
        json_payload = loads(payload)
        self.handle_id = handle_id
        self.text: str = json_payload['text']
        self.data: tuple[str, DownloadType] = json_payload['data']
        self.chat_id: int = json_payload['chat_id']
        self.user_id: int = json_payload['user_id']


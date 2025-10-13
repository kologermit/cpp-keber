from os import path

from config import USE_OAUTH, TMP_DIR
from models import YouTubeAudioSetting
from utils.Python.types import Message

from pytubefix import YouTube
from pytubefix.exceptions import RegexMatchError, VideoUnavailable
from pathvalidate import sanitize_filename
from loguru import logger

def audio_handler(message: Message) -> bool:
    videos: list[YouTube] = []

    for line in message.text.split('\n'):
        try:
            videos.append(YouTube(line.strip(), use_oauth=USE_OAUTH))
        except (RegexMatchError, VideoUnavailable, KeyError):
            continue

    settings: dict[str, YouTubeAudioSetting] = {
        setting.url: setting
        for setting in YouTubeAudioSetting.select(YouTubeAudioSetting.id==message.user_id)
    }

    logger.info(f'settings: {settings}')
    logger.info(f'videos: {videos}')

    # failed_downloads: list[YouTube] = []
    # success_downloads: list[str] = []

    # for video in videos:
    #     try:
    #         if video.watch_url in settings:
    #             setting = settings[video.watch_url]
    #             session = ClientSession(setting.download_url)
    #             response: ClientResponse = await session.get()
    #             if response.status < 200 or response.status >= 300:
    #                 failed_downloads.append(video)
    #                 continue
    #             if setting.file_name != "" and setting.file_name is not None:
    #                 file_name = setting.file_name
    #             else:
    #                 file_name = sanitize_filename(path.basename(setting.download_url))
    #             file_path = path.join(TMP_DIR, file_name)
    #             file = open(file_path, 'wb')
    #             file.write(await response.content.read())
    #             file.close()
    #             success_downloads.append(file_path)
    #         else:
    #             success_downloads.append(video.streams.get_audio_only().download(output_path=TMP_DIR))
    #     except:
    #         pass

    
            
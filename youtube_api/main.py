# Встроенные модули
from json import dumps
from functools import lru_cache

# Внутренниие модули
from config import (
    LOGS_DIR, 
    LISTEN_IP, 
    LISTEN_PORT, 
    PROJECT_NAME,
    TEST_YOUTUBE_VIDEO,
    USE_OAUTH,
    BOT_URL,
    BOT_ADMINS,
    AUTH_KEY,
)
from utils.Python.logger import init as init_logger, logger, log_err_with_code
from utils.Python.bot_api import BotAPI
from utils.Python.check_oauth import is_logined

# Внешние модули
from bottle import route, run, response, request
from pytubefix import YouTube, Channel, Playlist
from pytubefix.exceptions import RegexMatchError, VideoUnavailable

MAX_CACHE_SIZE = 2000

def middleware(f):
    def _(*args, **kwargs) -> str:
        try:
            return f(*args, **kwargs)
        except Exception as err:
            log_err_with_code(err)
            return generate_json_response({'message': 'Internal Error', 'error': str(err)}, 500)
    return _

def generate_json_response(data, status: int=200) -> str:
    response.status = status
    response.set_header('Content-Type', 'application/json')
    response.set_header('Cache-Control', 'no-cache')
    return dumps(data)

@lru_cache(maxsize=MAX_CACHE_SIZE)
def get_video_dict(url: str, use_oauth: bool) -> dict:
    v = YouTube(url, use_oauth=use_oauth)
    return {
        'title': v.title,
        'video_id': v.video_id,
        'video_url': v.watch_url,
        'thumbnail_url': v.thumbnail_url,
        'year': v.publish_date.year if v.publish_date is not None else None,
        'channel_url': v.channel_url,
    }

@lru_cache(maxsize=MAX_CACHE_SIZE)
def get_channel_dict(url: str, use_oauth: bool) -> dict:
    c = Channel(url, use_oauth=use_oauth)
    return {
        'channel_id': c.channel_id,
        'title': c.channel_name,
        'channel_url': c.channel_url,
    }

@lru_cache(maxsize=MAX_CACHE_SIZE)
def get_playlist_dict(url: str, use_oauth: bool) -> dict:
    p = Playlist(url, use_oauth=use_oauth)
    return {
        'playlist_id': p.playlist_id,
        'title': p.title,
        'playlist_url': p.playlist_url,
        'views': p.views,
        'thumbnail_url': p.thumbnail_url,
        'video_urls': p.video_urls
    }

@route('/video')
@middleware
def video():
    try:
        url = YouTube(dict(request.query)["url"]).watch_url # type: ignore
        v = get_video_dict(url, USE_OAUTH)
    except (RegexMatchError, KeyError):
        return generate_json_response({'message': 'Video not found'}, 400)
    except VideoUnavailable:
        return generate_json_response({'message': 'Video Unavailable'}, 400)
    return generate_json_response({
        'video': v,
        'channel': get_channel_dict(v['channel_url'], USE_OAUTH),
    })

@route('/playlist')
@middleware
def playlist():
    try:
        url = Playlist(dict(request.query)["url"]).playlist_url # type: ignore
        p = get_playlist_dict(url, USE_OAUTH)
    except (RegexMatchError, KeyError):
        return generate_json_response({'message': 'Playlist not found'}, 400)
    return generate_json_response({'playlist': p})

@route('/health')
@middleware
def health():
    return generate_json_response({'status': 'active'})

def main():
    init_logger(LOGS_DIR)
    bot = BotAPI(BOT_URL, AUTH_KEY, BOT_ADMINS)
    bot.send_message_to_admins('START SERVICE YOUTUBE API')
    if not is_logined(TEST_YOUTUBE_VIDEO, USE_OAUTH):
        bot.send_message_to_admins('SERVICE YOUTUBE API: oauth not logged in. Logging in and reload program')
        logger.warning({'event': 'OAUTH_NOT_LOGGED_IN', 'message': 'YouTube OAuth is not logged in. Logging in and reload program'})
        try:
            while True:
                pass
        except KeyboardInterrupt:
            return 0
    logger.info({'event': 'START_SERVICE', 'service': {PROJECT_NAME}, 'listen': f'{LISTEN_IP}:{LISTEN_PORT}'})
    run(host=LISTEN_IP, port=LISTEN_PORT)
    return 0

if __name__ == '__main__':
    exit(main())
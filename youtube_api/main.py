# Встроенные модули
from json import dumps
from functools import lru_cache

# Внутренниие модули
from config import (
    LOGS_DIR, 
    LISTEN_IP, 
    LISTEN_PORT, 
    PROJECT_NAME, 
    GOOGLE_PASSWORD, 
    GOOGLE_EMAIL, 
    SELENIUM_PAGE_LOAD_TIME, 
    SELENIUM_HOST, 
    TEST_YOUTUBE_VIDEO,
    USE_OAUTH,
)
from utils.Python.logger import init as init_logger, logger, log_err_with_code
from utils.Python.pythubefix_settings import init as init_pytubefix

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
        'year': v.publish_date.year,
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
        url = YouTube(dict(request.query)["url"]).watch_url
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
        url = Playlist(dict(request.query)["url"]).playlist_url
        p = get_playlist_dict(url, USE_OAUTH)
    except (RegexMatchError, KeyError):
        return generate_json_response({'message': 'Playlist not found'}, 400)
    return generate_json_response({
        'playlist': p
    })

def main():
    init_logger(LOGS_DIR)
    init_pytubefix(
        GOOGLE_EMAIL, 
        GOOGLE_PASSWORD, 
        SELENIUM_HOST, 
        SELENIUM_PAGE_LOAD_TIME,
        TEST_YOUTUBE_VIDEO,
        USE_OAUTH
    )
    logger.info({'event': 'START_SERVICE', 'service': {PROJECT_NAME}, 'listen': f'{LISTEN_IP}:{LISTEN_PORT}'})
    run(host=LISTEN_IP, port=LISTEN_PORT)

if __name__ == '__main__':
    main()
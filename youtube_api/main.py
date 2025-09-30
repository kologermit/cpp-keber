import sys; sys.path.extend(['../'*i for i in range(10)]+['.'])


from config import LOGS_DIR, LISTEN_IP, LISTEN_PORT, PROJECT_NAME, GOOGLE_PASSWORD, GOOGLE_EMAIL, SELENIUM_PAGE_LOAD_TIME, SELENIUM_HOST, TEST_YOUTUBE_VIDEO
from utils.Python.logger import init as init_logger, logger, log_err_with_code
from utils.Python.google_acc import ActiveGoogleAccResult, active_google_acc_by_selenium
from bottle import route, run, response, request
from pytubefix import YouTube, Channel, Playlist
from pytubefix.exceptions import RegexMatchError, VideoUnavailable
from json import dumps
import builtins

USE_OAUTH = True

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

def get_video_dict(v: YouTube) -> dict:
    return {
        'title': v.title,
        'video_id': v.video_id,
        'video_url': v.watch_url,
        'thumbnail_url': v.thumbnail_url,
        'year': v.publish_date.year
    }

def get_channel_dict(c: Channel) -> dict:
    return {
        'channel_id': c.channel_id,
        'title': c.channel_name,
        'channel_url': c.channel_url,
    }

def get_playlist_dict(p: Playlist) -> dict:
    return {
        'playlist_id': p.playlist_id,
        'title': p.title,
        'playlist_url': p.playlist_url,
        'views': p.views,
        'thumbnail_url': p.thumbnail_url,
    }

original_print = builtins.print
original_input = builtins.input
code = ''
active_google_acc_result: tuple[ActiveGoogleAccResult, Exception] = None

def mock_print(*args, **kwargs):
    global code
    original_print(*args, **kwargs)
    code = args[0].split(' ')[-1]
    builtins.print = original_print

def mock_input(_):
    global code, active_google_acc_result
    active_google_acc_result = active_google_acc_by_selenium(code, GOOGLE_EMAIL, GOOGLE_PASSWORD, SELENIUM_HOST, SELENIUM_PAGE_LOAD_TIME)
    builtins.input = original_input


@route('/video')
@middleware
def video():
    try:
        v = YouTube(dict(request.query)["url"], use_oauth=USE_OAUTH)
        _ = v.title
    except (RegexMatchError, KeyError):
        return generate_json_response({'message': 'Video not found'}, 400)
    except VideoUnavailable:
        return generate_json_response({'message': 'Video Unavailable'}, 400)
    c = Channel(v.channel_url, use_oauth=USE_OAUTH)
    return generate_json_response({
        'video': get_video_dict(v),
        'channel': get_channel_dict(c),
    })

@route('/playlist')
@middleware
def playlist():
    try:
        p = Playlist(dict(request.query)["url"], use_oauth=USE_OAUTH)
    except (RegexMatchError, KeyError):
        return generate_json_response({'message': 'Playlist not found'}, 400)
    return generate_json_response({
        'playlist': get_playlist_dict(p),
        'video_urls': [
            v.watch_url
            for v in p.videos
        ]
    })

def main():
    init_logger(LOGS_DIR)
    builtins.print = mock_print
    builtins.input = mock_input
    v = YouTube(TEST_YOUTUBE_VIDEO, use_oauth=USE_OAUTH)
    builtins.print = original_print
    builtins.input = original_input
    logger.info(f'Test video title: {v.title}')
    logger.info(f'Start service {PROJECT_NAME} on {LISTEN_IP}:{LISTEN_PORT}')
    run(host=LISTEN_IP, port=LISTEN_PORT)

if __name__ == '__main__':
    main()
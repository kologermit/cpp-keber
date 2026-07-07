from  copy import deepcopy

from pytubefix import YouTube
from pytubefix.helpers import reset_cache
from pytubefix.exceptions import BotDetection
import pytubefix.innertube
from urllib.error import HTTPError

class CheckOAUTH:
    def __init__(self, test_youtube_video: str, use_oauth: bool=False):
        self.test_youtube_video = deepcopy(test_youtube_video)
        self.use_oauth = deepcopy(use_oauth)
        self.is_logined = True

    def __call__(self,
        verification_url: str,
        code: str):
        self.is_logined = False

def is_logined(
    test_youtube_video: str,
    use_oauth: bool = True
) -> bool:
    check_oauth = CheckOAUTH(
        test_youtube_video,
        use_oauth,
    )
    default_oauth_verifier = pytubefix.innertube._default_oauth_verifier
    if use_oauth:
        pytubefix.innertube._default_oauth_verifier = check_oauth
    try:
        YouTube(test_youtube_video, use_oauth=use_oauth).title
    except HTTPError as exc:
        if exc.code != 428:
            raise exc
    except BotDetection:
        return False
    pytubefix.innertube._default_oauth_verifier = default_oauth_verifier
    return check_oauth.is_logined
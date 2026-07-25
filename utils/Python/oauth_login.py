from .config import PROXY_HOST, PROXY_PORT, TEST_YOUTUBE_VIDEO, default_config, required
from .parse_config import parse_config_to_exec
from pytubefix import YouTube

default_config[TEST_YOUTUBE_VIDEO][required] = True
default_config[PROXY_HOST][required] = True
default_config[PROXY_PORT][required] = True # type: ignore
exec(parse_config_to_exec(default_config))

proxies = {"http": f"http://{PROXY_HOST}:{PROXY_PORT}"} if PROXY_HOST and PROXY_PORT > 0 else None

v = YouTube(TEST_YOUTUBE_VIDEO, use_oauth=True, proxies=proxies)
print(f'Title={v.title}')
print(f'Steams={v.streams}')
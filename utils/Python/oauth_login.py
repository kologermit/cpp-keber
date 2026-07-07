from pytubefix import YouTube

BASE_TEST_VIDEO_URL = 'https://www.youtube.com/watch?v=jiT2Mak9AzI'
test_video_url = input(f'Input test url or skip (default: {BASE_TEST_VIDEO_URL}): ').strip()
test_video_url = BASE_TEST_VIDEO_URL if test_video_url == '' else test_video_url

v = YouTube(test_video_url, use_oauth=True)
print(f'Title={v.title}')
print(f'Steams={v.streams}')
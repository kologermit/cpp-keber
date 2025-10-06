from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
from pytubefix import YouTube
from time import sleep
from enum import Enum, auto
from loguru import logger
from copy import deepcopy
import pytubefix

class GoogleEmailActivatorResult(Enum):
    OK = auto()
    ERROR_INIT_DRIVER = auto()
    ERROR_INPUT_CODE = auto()
    ERROR_INPUT_EMAIL = auto()
    ERROR_INPUT_PASSWORD = auto()
    ERROR_CLICK_CONFIRM = auto()

class GoogleEmailActivator:
    def __init__(
        self,
        email: str, 
        password: str, 
        selenium_host: str = 'http://localhost:4444/wd/hub', 
        page_load_time: int = 10,
    ):
        self.__email__ = deepcopy(email)
        self.__password__ = deepcopy(password)
        self.__selenium_host__ = deepcopy(selenium_host)
        self.__page_load_time__ = deepcopy(page_load_time)

    def __call__(
        self,
        verification_url: str,
        code: str
    ) -> tuple[GoogleEmailActivatorResult, Exception|None]:

        try:
            logger.info(f'Try connect to selenium {self.__selenium_host__}')
            options = Options()
            options.add_argument('--no-sandbox')
            options.add_argument('--disable-dev-shm-usage')
            options.add_argument('--disable-gpu')
            options.add_argument('--window-size=1920,1080')
            options.add_experimental_option("excludeSwitches", ["enable-automation"])
            options.add_experimental_option('useAutomationExtension', False)
            options.add_argument("--disable-blink-features")
            options.add_argument("--disable-blink-features=AutomationControlled")
            options.add_argument("--user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36")

            driver = webdriver.Remote(
                command_executor=self.__selenium_host__,
                options=options,
            )
            logger.info(f'Success connect to selenium')
            logger.info(f'Try open verification page {verification_url}')
            driver.get(verification_url)
            logger.info('Success open verification page')
        except Exception as err:
            return GoogleEmailActivatorResult.ERROR_INIT_DRIVER, err

        # Input Code
        try:
            logger.info(f'Waiting {self.__page_load_time__}s load code page')
            sleep(self.__page_load_time__)
            logger.info(f'Try past code {code}')
            input_code = driver.find_element(By.CSS_SELECTOR, '#code')
            driver.execute_script("arguments[0].focus();", input_code)
            input_code.send_keys(code)
            input_code.send_keys('\n')
            logger.info('Success past code')
        except Exception as err:
            return GoogleEmailActivatorResult.ERROR_INPUT_CODE, err
        

        # Input Email
        try:
            logger.info(f'Waiting {self.__page_load_time__}s load email page')
            sleep(self.__page_load_time__)
            logger.info(f'Try paste email {self.__email__}')
            input_email = driver.find_element(By.CSS_SELECTOR, '#identifierId')
            driver.execute_script("arguments[0].focus();", input_email)
            input_email.send_keys(self.__email__)
            input_email.send_keys('\n')
            logger.info('Success paste email')
        except Exception as err:
            return GoogleEmailActivatorResult.ERROR_INPUT_CODE, err

        # Input Password
        try:
            logger.info(f'Waiting {self.__page_load_time__}s load password page')
            sleep(self.__page_load_time__)
            logger.info('Try paste password')
            input_password = driver.find_element(By.CSS_SELECTOR, '[name=Passwd]')
            driver.execute_script("arguments[0].focus();", input_password)
            input_password.send_keys(self.__password__)
            input_password.send_keys('\n')
            logger.info('Success paste password')
        except Exception as err:
            return GoogleEmailActivatorResult.ERROR_INPUT_PASSWORD, err

        # Click Confirm
        try:
            logger.info(f'Waiting {self.__page_load_time__}s load confirm page')
            sleep(self.__page_load_time__)
            logger.info('Try confirm')
            confirm_button = driver.find_elements(By.CSS_SELECTOR, 'button span')[1]
            confirm_button.click()
            logger.info('Success confirm')
        except Exception as err:
            return GoogleEmailActivatorResult.ERROR_CLICK_CONFIRM, err

        # Wait confirm
        logger.info(f'Waiting {self.__page_load_time__}s to confirm')
        sleep(self.__page_load_time__)
        logger.info(f'Success active {self.__email__}')

        return GoogleEmailActivatorResult.OK, None

def init(
    google_email,
    google_password,
    selenium_host,
    selenium_page_load_time,
    test_youtube_video,
    use_oauth: bool = True
) -> str:
    if use_oauth:
        pytubefix.innertube._default_oauth_verifier = GoogleEmailActivator(
            google_email, 
            google_password, 
            selenium_host, 
            selenium_page_load_time
        )
    v = YouTube(test_youtube_video, use_oauth=use_oauth)
    {'event': 'TEST_GET_VIDEO_TITLE', 'url': test_youtube_video,'title': f'Test video title: {v.title}'}
    return v.title
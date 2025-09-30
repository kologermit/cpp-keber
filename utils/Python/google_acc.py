from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
from time import sleep
from enum import Enum, auto
from loguru import logger

class ActiveGoogleAccResult(Enum):
    OK = auto()
    ERROR_INIT_DRIVER = auto()
    ERROR_INPUT_CODE = auto()
    ERROR_INPUT_EMAIL = auto()
    ERROR_INPUT_PASSWORD = auto()
    ERROR_CLICK_CONFIRM = auto()


def active_google_acc_by_selenium(
        code: str, 
        email: str, 
        password: str, 
        selenium_host: str = 'http://localhost:4444/wd/hub', 
        page_load_time: int = 10
    ) -> tuple[ActiveGoogleAccResult, Exception|None]:

    try:
        logger.info(f'Try connect to selenium {selenium_host}')
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
            command_executor=selenium_host,
            options=options,
        )

        driver.get('https://www.google.com/device')
        logger.info(f'Success connect to selenium')
    except Exception as err:
        return ActiveGoogleAccResult.ERROR_INIT_DRIVER, err

    # Input Code
    try:
        logger.info(f'Waiting {page_load_time}s load code page')
        sleep(page_load_time)
        logger.info(f'Try past code {code}')
        input_code = driver.find_element(By.CSS_SELECTOR, '#code')
        driver.execute_script("arguments[0].focus();", input_code)
        input_code.send_keys(code)
        input_code.send_keys('\n')
        logger.info('Success past code')
    except Exception as err:
        return ActiveGoogleAccResult.ERROR_INPUT_CODE, err
    

    # Input Email
    try:
        logger.info(f'Waiting {page_load_time}s load email page')
        sleep(page_load_time)
        logger.info(f'Try paste email {email}')
        input_email = driver.find_element(By.CSS_SELECTOR, '#identifierId')
        driver.execute_script("arguments[0].focus();", input_email)
        input_email.send_keys(email)
        input_email.send_keys('\n')
        logger.info('Success paste email')
    except Exception as err:
        return ActiveGoogleAccResult.ERROR_INPUT_CODE, err

    # Input Password
    try:
        logger.info(f'Waiting {page_load_time}s load password page')
        sleep(page_load_time)
        logger.info('Try paste password')
        input_password = driver.find_element(By.CSS_SELECTOR, '[name=Passwd]')
        driver.execute_script("arguments[0].focus();", input_password)
        input_password.send_keys(password)
        input_password.send_keys('\n')
        logger.info('Success paste password')
    except Exception as err:
        return ActiveGoogleAccResult.ERROR_INPUT_PASSWORD, err

    # Click Confirm
    try:
        logger.info(f'Waiting {page_load_time}s load confirm page')
        sleep(page_load_time)
        logger.info('Try confirm')
        confirm_button = driver.find_elements(By.CSS_SELECTOR, 'button span')[1]
        confirm_button.click()
        logger.info('Success confirm')
    except Exception as err:
        return ActiveGoogleAccResult.ERROR_CLICK_CONFIRM, err

    # Wait confirm
    logger.info(f'Waiting {page_load_time}s to confirm')
    sleep(page_load_time)

    return ActiveGoogleAccResult.OK, None
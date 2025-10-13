from copy import deepcopy
from requests import post, session

class BotAPI:
    def __init__(self, url: str):
        self.url = deepcopy(url)
        self.session = session()

    def send_message(
            self, 
            chat_id: int, 
            text: str, 
            reply_message_id: int|None=None,
            file_path: str|None=None,
        ) -> int:
        response = self.session.post(
            f'{self.url}/send_text', 
            data={
                'chat_id': chat_id, 
                'text': text,
                **(
                    {'reply_message_id': reply_message_id}
                    if reply_message_id is not None else
                    {}
                )
            },
            files=({'file': open(file_path, 'rb')} if file_path is not None else {})
        )
        if response.status_code != 200:
            return -1
        
        return response.json()
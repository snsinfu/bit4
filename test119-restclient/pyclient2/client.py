import os

import requests
from hmac_auth import HMACAuth

API_SERVER = os.getenv('API_SERVER')
API_KEY = os.getenv('API_KEY')
API_SECRET = os.getenv('API_SECRET').encode('ascii')

post = {
    'title': 'Beautiful is better than ugly',
    'score': 99,
}
res = requests.post(API_SERVER + '/v1/posts',
                    json=post, auth=HMACAuth(API_KEY, API_SECRET)).json()
print(res['message'])

import os
from env import env

print(env.API_KEY)
print(env.API_SECRET)
print(env.TIMEOUT or 10)

del env.API_KEY
print(env.API_KEY)

env.API_KEY = 'abc'
os.system('echo ${API_KEY}')

import asyncio
import time

import aiohttp


async def main():
    async with aiohttp.ClientSession() as session:
        t = ticker(3)
        await asyncio.wait([run(t, session) for _ in range(5)])


async def ticker(interval):
    while True:
        yield
        await asyncio.sleep(interval)


async def run(t, session):
    await anext(t)
    await fetch_text(session, "https://www.example.com/")
    print(time.time())


async def fetch_text(session, url):
    async with session.get(url) as resp:
        return await resp.text()


async def anext(gen):
    async for x in gen:
        return x


asyncio.run(main())

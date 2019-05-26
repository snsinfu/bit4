import asyncio
import json

import aiohttp
import bs4


BIORXIV_BASE = "https://www.biorxiv.org"
ALTMETRIC_BASE = "https://api.altmetric.com/v1"


async def main():
    async with aiohttp.ClientSession() as session:
        article_id = "10.1101/648147"
        article_version = "v1"

        url = f"{BIORXIV_BASE}/content/{article_id}{article_version}"
        article_page = await fetch_text(session, url)

        url = f"{ALTMETRIC_BASE}/doi/{article_id}"
        article_altmetric = await fetch_text(session, url)

    metrics = json.loads(article_altmetric)
    for k, v in metrics.items():
        print("#", k)
        print(v)
        print("")



async def fetch_text(session, url):
    async with session.get(url) as resp:
        return await resp.text()


if __name__ == "__main__":
    asyncio.run(main())

import argparse
import asyncio
import json
import sys

import aiohttp
import techbookfest


TBF_VISIBILITY = "site"
MAX_CONNECTION = 5


async def main():
    args = parse_args()
    event_id = args.event

    conn = aiohttp.TCPConnector(limit_per_host=MAX_CONNECTION)
    async with aiohttp.ClientSession(connector=conn) as session:
        info = await scrape_all_circle_info(session, args.event)

    json.dump(info, sys.stdout)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("event", type=str, help="Event ID (e.g., tbf05)")
    return parser.parse_args()


async def scrape_all_circle_info(session, event_id):
    api = techbookfest.API(session)

    circle_list = await get_circle_list(api, event_id)
    tasks = [consolidate_circle_info(api, circle) for circle in circle_list]
    return await asyncio.gather(*tasks)


async def consolidate_circle_info(api, circle):
    products = await get_circle_products(api, circle["id"])
    circle = circle.copy()
    circle["products"] = products
    return circle


async def get_circle_list(api, event_id):
    params = {"eventID": event_id, "visibility": TBF_VISIBILITY, "onlyAdoption": "true"}
    return await api.get_list("/circle", params)


async def get_circle_products(api, circle_id):
    params = {"circleExhibitInfoID": circle_id}
    return await api.get_list("/product", params)


if __name__ == "__main__":
    asyncio.run(main())

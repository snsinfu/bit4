import argparse
import json
import signal
import sys

import requests


TBF_API_BASE = "https://techbookfest.org/api"
TBF_EVENT = "tbf05"
TBF_VISIBILITY = "site" # "site" or "staff"
TBF_LIMIT = 100


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)

    parser = argparse.ArgumentParser()
    parser.add_argument("id", type=str, help="Circle ID")
    args = parser.parse_args()
    circle_id = args.id

    with requests.Session() as session:
        params = {
            "circleExhibitInfoID": circle_id,
            "limit": TBF_LIMIT,
        }
        url = TBF_API_BASE + "/product"

        circle_products = []
        while True:
            page = session.get(url, params=params).json()

            chunk = page["list"]
            circle_products.extend(chunk)

            if "cursor" in page:
                params["cursor"] = page["cursor"]
            else:
                break

    json.dump(circle_products, sys.stdout)



if __name__ == "__main__":
    main()

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

    with requests.Session() as session:
        params = {
            "eventID": TBF_EVENT,
            "visibility": TBF_VISIBILITY,
            "limit": TBF_LIMIT,
            "onlyAdoption": True,
        }
        url = TBF_API_BASE + "/circle"

        circle_list = []
        while True:
            page = session.get(url, params=params).json()

            chunk = page["list"]
            circle_list.extend(chunk)

            if "cursor" in page:
                params["cursor"] = page["cursor"]
            else:
                break

    json.dump(circle_list, sys.stdout)



if __name__ == "__main__":
    main()

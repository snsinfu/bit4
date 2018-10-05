import aiohttp


TBF_API_BASE = "https://techbookfest.org/api"
TBF_VISIBILITY = "site"
TBF_LIMIT = 100


class API:
    def __init__(self, session):
        self._session = session

    async def get_list(self, endpoint, params={}):
        query_params = {"limit": TBF_LIMIT}
        query_params.update(params)

        result = []

        while True:
            url = TBF_API_BASE + endpoint
            async with self._session.get(url, params=query_params) as response:
                page = await response.json()

                chunk = page.get("list")
                if chunk:
                    result.extend(chunk)

                cursor = page.get("cursor")
                if cursor:
                    query_params["cursor"] = cursor
                else:
                    break

        return result

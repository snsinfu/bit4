import asyncio
import datetime
import random


async def run():
    delay = 1
    while True:
        await asyncio.gather(task(), asyncio.sleep(delay))


async def task():
    size = 2 ** random.randint(1, 15)
    count = 0
    for n in range(2, size):
        prime = True
        for k in range(2, n):
            if n % k == 0:
                prime = False
                break
        if prime:
            count += 1

    now = datetime.datetime.now().strftime("%T")
    print(f"{now} - {count}")


if __name__ == "__main__":
    asyncio.run(run())

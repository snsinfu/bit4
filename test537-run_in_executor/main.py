import asyncio
import time


async def main():
    loop = asyncio.get_event_loop()

    print(f"Start: {time.strftime('%T')}")

    await asyncio.gather(
        loop.run_in_executor(None, synchronous_task_1),
        loop.run_in_executor(None, synchronous_task_2),
        loop.run_in_executor(None, synchronous_task_3),
    )


def synchronous_task_1():
    time.sleep(10)
    print(f"Task 1: {time.strftime('%T')}")


def synchronous_task_2():
    time.sleep(2)
    print(f"Task 2: {time.strftime('%T')}")


def synchronous_task_3():
    time.sleep(5)
    print(f"Task 3: {time.strftime('%T')}")


asyncio.run(main())

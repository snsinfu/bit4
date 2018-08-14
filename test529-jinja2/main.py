import time

from jinja2 import Environment, FileSystemLoader


def sync_bench():
    env = Environment(loader=FileSystemLoader("templates"), line_statement_prefix="#")

    test_count = 16384
    start_time = time.time()

    for _ in range(test_count):
        template = env.get_template("main.html.j2")
        document = template.render(
            {
                "sections": [
                    {
                        "title": "Lorem ipsum",
                        "body": "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
                    },
                    {
                        "title": "Duis vehicula",
                        "body": "Duis vehicula placerat facilisis.",
                    },
                    {
                        "title": "Quisque nisi",
                        "body": "Quisque nisi velit, faucibus quis dictum at, aliquam sed neque.",
                    },
                ]
            }
        )

    end_time = time.time()
    mean_elapsed_time = (end_time - start_time) / test_count

    print("{:.1f} us/render".format(mean_elapsed_time * 1e6))


sync_bench()

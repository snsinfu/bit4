import argparse
import json


def main():
    args = parse_args()

    with open(args.json) as file:
        circle_info = json.load(file)

    for idx, circle in enumerate(circle_info):
        if idx == 0:
            event = circle["event"]
            print("# {event}".format(
                event=event["name"]
            ))
            print("")

        url = "https://techbookfest.org/event/{event_id}/circle/{circle_id}".format(
            event_id=circle["event"]["id"],
            circle_id=circle["id"]
        )
        print("### [{space} - {name}]({url})".format(
            space="/".join(circle["spaces"]),
            name=circle["name"],
            url=url
        ))
        print("")

        print(circle["genreFreeFormat"])
        print("")

        products = circle["products"]
        if not products:
            continue

        for product in products:
            print("- {name}".format(
                name=product.get("name")
            ))
        print("")


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("json", type=str, help="TBF JSON")
    return parser.parse_args()


if __name__ == "__main__":
    main()

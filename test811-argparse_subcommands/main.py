import argparse


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="subcmd")

    auth_subparser = subparsers.add_parser("auth")
    auth_subparser.add_argument("--username")
    auth_subparser.add_argument("--password")

    search_subparser = subparsers.add_parser("search")
    search_subparser.add_argument("--token")
    search_subparser.add_argument("keyword")

    # auth subcommand
    auth = parser.parse_args(
        ["auth", "--username", "user1234", "--password", "qwerty"]
    )
    assert auth.subcmd == "auth"
    assert auth.username == "user1234"
    assert auth.password == "qwerty"

    # auth subcommand
    search = parser.parse_args(
        ["search", "--token", "b4faa57804affc4c", "lorem ipsum"]
    )
    assert search.subcmd == "search"
    assert search.token == "b4faa57804affc4c"
    assert search.keyword == "lorem ipsum"

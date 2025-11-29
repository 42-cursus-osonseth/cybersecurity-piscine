import argparse
from htmlParser import MyHTMLParser


def check_limit(value):
    try:
        v = int(value)
    except ValueError:
        raise argparse.ArgumentTypeError("-l must be an integer between 0 and 5")
    if v < 0 or v > 5:
        raise argparse.ArgumentTypeError("-l must be between 0 and 5")
    return v


def argsParser() -> list:

    try:
        parser = argparse.ArgumentParser()
        parser.add_argument(
            "-l",
            type=check_limit,
            nargs="?",
            const=5,
            default=None,
            help="-l between 0 and 5 (default: 5 if provided without value)",
        )
        parser.add_argument("-r", action="store_true")
        parser.add_argument("-p")
        parser.add_argument("url")
        args = parser.parse_args()
        args = vars(args)
        if args["l"] is not None and not args["r"]:
            parser.error("-l can only be used if -r is present.")
        return args
    except Exception as Error:
        print(Error)
        exit(1)


def main():
    try:
        args = argsParser()
        html_parser = MyHTMLParser(args)
        html_parser.execute()
    except Exception as e:
        print(e)


if __name__ == "__main__":
    main()

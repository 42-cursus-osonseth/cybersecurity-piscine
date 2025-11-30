from bs4 import BeautifulSoup
import sys
import argparse


def argsParser() -> list:

    try:
        parser = argparse.ArgumentParser()
        parser.add_argument("-l", nargs= "?", const= 5, type=int)
        parser.add_argument("-r", action="store_true")
        parser.add_argument("-p")
        parser.add_argument("url")
        args = parser.parse_args()
        args = vars(args)
        if args["l"] and not args["r"]:
            parser.error("-l can only be used if -r is present.")
        if not args["p"]:
           args["p"] = "./data"
        return args
    except Exception as Error:
        print(Error)
        exit(1)


def main():
    args = argsParser()


if __name__ == "__main__":
    main()

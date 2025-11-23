import argparse
import os
from htmlParser import MyHTMLParser
from request import get_html_page


def create_download_folder(path: str) -> str:
    if path:
        os.makedirs(path, exist_ok=True)
        return path
    else:
        os.makedirs("data", exist_ok=True)
        return "data"


def argsParser() -> list:
    parser = argparse.ArgumentParser()
    parser.add_argument("-l")
    parser.add_argument("-r", action="store_true")
    parser.add_argument("-p")
    parser.add_argument("url")
    args = parser.parse_args()
    args = vars(args)
    if args["l"] is not None and not args["r"]:
        parser.error("-l can only be used if -r is present.")
    return args


def main():
    args = argsParser()
    print(args)
    level = 0
    if (args["r"]):
        print("prout lvl")
        level = int(args["l"])
        print (level)
    page = get_html_page(args["url"])
    html_parser = MyHTMLParser(args["p"], args["url"], level)
    html_parser.feed(page.text)
    html_parser.Filter_images_by_extensions()
    html_parser.download_the_images_from_the_current_page()


if __name__ == "__main__":
    main()

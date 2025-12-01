from bs4 import BeautifulSoup
from bs4.element import Tag
import pathlib
import os
import argparse
import requests
from urllib.parse import urljoin

EXTENSIONS = [".jpg", ".jpeg", ".png", ".gif", ".bmp", ".svg"]


def argsParser() -> list:

    try:
        parser = argparse.ArgumentParser()
        parser.add_argument("-l", dest="depth", nargs="?", const=5, type=int)
        parser.add_argument("-r", dest="recursive", action="store_true")
        parser.add_argument("-p", dest="path")
        parser.add_argument("url")
        args = parser.parse_args()
        args = vars(args)
        if args["depth"] and not args["recursive"]:
            parser.error("-l can only be used if -r is present.")
        if args["recursive"] and not args["depth"]:
            args["depth"] = 5
        if not args["path"]:
            args["path"] = "./data"
        return args
    except Exception as Error:
        print(Error)
        exit(1)


def fetch_content_page(url: str) -> str:
    try:
        r = requests.get(url, headers={"User-Agent": "Mozilla/5.0"})
        r.raise_for_status()
        return r.content
    except Exception as Error:
        print(f"Error fetching {url} -> {Error}")


def create_download_directory(path: str) -> None:
    try:
        dl_path = pathlib.Path(path)
        if dl_path.exists() and not dl_path.is_dir():
            raise NotADirectoryError(f"{dl_path} is not a directory")
        dl_path.mkdir(parents=True, exist_ok=True)

    except Exception as Error:
        raise Error


def resolve_images_src(tag: Tag, url: str) -> str | None:
    src = tag.get("src")
    if not src or src.startswith("data:"):
        return None
    if src.startswith("http"):
        full_url = src
    elif src.startswith("//"):
        full_url = "https:" + src
    else:
        full_url = urljoin(url, src)

    if full_url.lower().endswith(tuple(EXTENSIONS)):
        return full_url
    return None


def get_images_source(soup: BeautifulSoup, url: str) -> set:
    images: set = {
        img_url
        for img in soup.find_all("img")
        if (img_url := resolve_images_src(img, url)) is not None
    }
    return images


def resolve_full_url(href: str, url: str) -> str:
    if not href.startswith("http"):
        return urljoin(url, href)
    return href


def clean_href(tag: Tag) -> str | None:
    href = tag.get("href")
    if href and not href.startswith("#"):
        return href
    return None


def get_links_in_page(soup: BeautifulSoup, url: str) -> list:
    links = [
        resolve_full_url(href, url)
        for href in map(clean_href, soup.find_all("a"))
        if href is not None
    ]
    return links


def download_imgs_from_current_page(img_src: list, dl_path: str) -> None:

    for img in img_src:
        download_img(img, dl_path + "/")


def download_img(img_src: str, dl_path: str, downloaded: set = set()) -> None:
    if img_src in downloaded:
        return
    downloaded.add(img_src)
    img_name = os.path.basename(img_src).split("?", 1)[0]
    data = requests.get(img_src, headers={"User-Agent": "Mozilla/5.0"}).content
    with open(dl_path + img_name, "wb") as f:
        f.write(data)
    print(f"{img_name} downloaded")


def run_spider(
    url: str, args: dict, current_depth: int = 0, visited: set = set()
) -> None:

    if url in visited:
        return
    if args["recursive"] and current_depth > args["depth"]:
        return
    page_content = fetch_content_page(url)
    soup = BeautifulSoup(page_content, "html.parser")
    imgs = get_images_source(soup, url)
    download_imgs_from_current_page(imgs, args["path"])
    if args["recursive"] and current_depth < args["depth"]:
        links = get_links_in_page(soup, args["url"])
        for link in links:
            run_spider(link, args, current_depth + 1, visited)


def main():

    try:
        args = argsParser()
        create_download_directory(args["path"])
        print(args)
        run_spider(args["url"], args)

    except Exception as Error:
        print(Error)
        exit(1)


if __name__ == "__main__":
    main()

from html.parser import HTMLParser
import os
import requests
from requests.models import Response
from urllib.parse import urljoin
from urllib.parse import urlparse
import re


class MyHTMLParser(HTMLParser):

    def __init__(self, args, *, convert_charrefs=True):
        super().__init__(convert_charrefs=convert_charrefs)
        try:
            self.count = 0
            self.images = []
            self.link = []
            self.visited = set()
            self.downloaded = set()
            print(args)
            self.download_path = args["p"] if args["p"] else "data"
            self.url = args["url"]
            self.base_domain = urlparse(self.url).netloc
            self.recursive = args["r"]
            self.limit = args["l"] if args["l"] else 5
            self.page = self.get_html_page(self.url)
            if self.page is None:
                exit(1)
            os.makedirs(self.download_path, exist_ok=True)
        except Exception as e:
            print(f"Error fetching page: {e}")
            exit(1)

    def handle_starttag(self, tag, attrs):
        if tag == "img":
            for name, value in attrs:
                if name == "src" and value.startswith("http"):
                    self.images.append(value)
                elif name == "src" and not value.startswith("http"):
                    self.images.append(urljoin(self.url, value))
        elif tag == "a":
            for name, value in attrs:
                if (
                    name == "href"
                    and not value.startswith("#")
                    and not value.startswith("mailto:")
                ):
                    full_url = urljoin(self.url, value)
                    self.link.append(full_url)

    def get_html_page(self, path: str) -> Response:
        try:
            page = requests.get(
                path, headers={"User-Agent": "Mozilla/5.0"}, timeout=5
                )
            page.raise_for_status()
            return page
        except Exception as e:
            print(f"Error fetching {path}: {e}")
            return None

    def sanitize_filename(self, s):
        return re.sub(r"[^a-zA-Z0-9]", "_", s)

    def execute(self):
        try:
            self.feed(self.page.text)
            self.download_the_images_from_the_current_page()
        except Exception as e:
            raise 
        if self.recursive and self.limit > 0:
            self.recursive_download(self.limit)

    def download_the_images_from_the_current_page(self):
        ext_accepted = (".jpg", ".jpeg", ".png", ".gif", ".bmp")
        for i, url in enumerate(self.images):
            if url in self.downloaded:
                continue
            self.downloaded.add(url)
            if url.startswith("http"):
                data = requests.get(
                    url, headers={"User-Agent": "Mozilla/5.0"}
                    ).content
                ext = os.path.splitext(url)[1][:4]
                if ext and ext.endswith(ext_accepted):
                    safe_url = self.sanitize_filename(url)[:85]
                    filename = (
                        f"{self.download_path}/img_{safe_url}_"
                        f"{self.count}_{i}{ext}"
                        )
                    with open(filename, "wb") as f:
                        f.write(data)
                    self.count += 1

    def recursive_download(self, limit):
        if limit <= 0:
            return
        current_links = self.link.copy()
        for value in current_links:
            if value in self.visited:
                continue
            if urlparse(value).netloc != self.base_domain:
                continue
            self.visited.add(value)
            self.url = value
            self.images = []
            self.link = []
            self.page = self.get_html_page(value)
            if self.page is None:
                continue
            self.feed(self.page.text)
            self.download_the_images_from_the_current_page()
            self.recursive_download(limit - 1)

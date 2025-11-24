from html.parser import HTMLParser
import os
import requests
from requests.models import Response


class MyHTMLParser(HTMLParser):

    def __init__(self, args, *, convert_charrefs=True):
        super().__init__(convert_charrefs=convert_charrefs)
        try:
            self.images = []
            print(args)
            self.download_path = args["p"] if args["p"] else "data"
            self.url = args["url"]
            self.recursive = args["r"]
            self.limit = args["l"] if args["l"] else 5
            self.page = self.get_html_page(self.url)
            print(f"RECURSIVITE = {self.recursive}")
            print(f"LIMIT = {self.limit}")
            os.makedirs(self.download_path, exist_ok=True)
        except Exception as e:
            print(f"Error fetching page: {e}")
            exit(1)

    def handle_starttag(self, tag, attrs):
        if tag == "img":
            for name, value in attrs:
                if name == "src":
                    self.images.append(value)

    def get_html_page(self, path: str) -> Response:
        page = requests.get(path, headers={"User-Agent": "Mozilla/5.0"})
        return page

    def execute(self):
        self.feed(self.page.text)
        self.Filter_images_by_extensions()
        self.download_the_images_from_the_current_page()
        if self.recursive and self.limit > 0:
            print("prout")


    def Filter_images_by_extensions(self):
        extensions = (".jpg", ".jpeg", ".png", ".gif", ".bmp")
        self.images = [url for url in self.images if url.lower().endswith(extensions)]

    def download_the_images_from_the_current_page(self):
        for i, url in enumerate(self.images):
            data = requests.get(
                self.images[i], headers={"User-Agent": "Mozilla/5.0"}
            ).content
            ext = os.path.splitext(url)[1]
            filename = open(f"{self.download_path}/img{i}.{ext}", "wb")
            filename.write(data)
            filename.close()

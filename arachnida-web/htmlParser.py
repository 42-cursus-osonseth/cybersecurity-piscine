from html.parser import HTMLParser
import os
import requests


class MyHTMLParser(HTMLParser):

    def __init__(self, download_folder, url, level = 0, *, convert_charrefs=True):
        super().__init__(convert_charrefs=convert_charrefs)
        print(f"url = {url} level = {level}")
        self.images = []
        self.download_path = download_folder if download_folder else "data"
        os.makedirs(self.download_path, exist_ok=True)

    def handle_starttag(self, tag, attrs):
        if tag == "img":
            for name, value in attrs:
                if name == "src":
                    self.images.append(value)

    def handle_endtag(self, tag):
        # print("Encountered an end tag :")
        pass

    def handle_data(self, data):
        # print("Encountered some data  :")
        pass

    def Filter_images_by_extensions(self):
        extensions = (".jpg", ".jpeg", ".png", ".gif", ".bmp")
        self.images = [
            url for url in self.images if url.lower().endswith(extensions)
        ]

    def download_the_images_from_the_current_page(self):
        for i, url in enumerate(self.images):
            data = requests.get(
                self.images[i], headers={"User-Agent": "Mozilla/5.0"}
            ).content
            ext = os.path.splitext(url)[1]
            filename = open(f"img{i}.{ext}", "wb")
            filename.write(data)
            filename.close()



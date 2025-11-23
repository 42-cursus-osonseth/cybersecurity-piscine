from requests.models import Response
import requests


def get_html_page(path: str) -> Response:
    page = requests.get(path, headers={"User-Agent": "Mozilla/5.0"})
    return page

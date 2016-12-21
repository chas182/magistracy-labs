from bs4 import BeautifulSoup
import requests


def get_links(html):
    soup = BeautifulSoup(html)
    return soup.find_all('a')


r = requests.get("https://ru.wikipedia.org/wiki/Python")
print get_links(r.text)

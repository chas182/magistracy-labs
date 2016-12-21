# coding: utf-8

from bs4 import BeautifulSoup
from time import sleep
import requests
import re

visited_pages = set()


def find_philosophy(link):
    sleep(2)
    if link in visited_pages:
        return
    visited_pages.add(link)

    r = requests.get(link)
    soup = BeautifulSoup(r.text)
    print soup.title.string

    if link == "https://ru.wikipedia.org/wiki/%D0%A4%D0%B8%D0%BB%D0%BE%D1%81%D0%BE%D1%84%D0%B8%D1%8F":
        print "Philosophy reached!!!"
        exit(0)

    for p_tag in soup.find('div', id="mw-content-text").find_all('p'):
        for a_tag in p_tag.find_all('a', text=re.compile("^(?!\[)")):
            if not a_tag.findParent('table', attrs={'class': 'infobox'}):
                find_philosophy("https://ru.wikipedia.org" + a_tag['href'])


find_philosophy("https://ru.wikipedia.org/wiki/Python")

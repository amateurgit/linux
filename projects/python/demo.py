#!bin/python3.9

from urllib.request import urlopen
from bs4 import BeautifulSoup
from urllib.error import URLError
from urllib.error import HTTPError

def getTitle(url):
        try:
                html = urlopen(url)
        except URLError as e:   # url 异常
                print('The server could not be found!')
                return None    
        except HTTPError as e:  # 网页异常
                print(e)
                return None

        try:
                bs = BeautifulSoup(html.read(), 'html.parser')
                title = bs.body.h1
        except AttributeError as e: # 标签异常
                return None
        else:
                return title
    
title = getTitle('http://pythonscraping.com/pages/page1.html')
if title == None:
        print('Title could not be found!')
else:
        print(title)


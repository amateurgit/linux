#!bin/python3.8
# -*- coding: utf-8 -*-

'''
1. 获取内链
'''
#from urllib.request import urlopen
#from bs4 import BeautifulSoup
#import datetime
#import random
#import re
#
#random.seed(datetime.datetime.now())
#
#def getLinks(articleUrl):
#    html = urlopen('https://baike.baidu.com{}'.format(articleUrl))
#    bs = BeautifulSoup(html, 'html.parser')
#    return bs.find('div', {'id':'slider_relations'}).find_all('a', href=re.compile('^(/item/)((?!:).)*$'))
#
#links = getLinks('/item/%E5%88%98%E5%BE%B7%E5%8D%8E/114923')
#
#while len(links) > 0:
#    newArticle = links[random.randint(0, len(links) - 1)].attrs['href']
#    print(newArticle)
#    links = getLinks(newArticle)






'''
2. 数据收集
'''
#from urllib.request import urlopen
#import urllib.parse     # python 3 url 不支持中文，需要转换
#from bs4 import BeautifulSoup
#import re
#
#pages = set()
#
#def getLinks(pageUrl):
#    global pages
#    html = urlopen('https://baike.baidu.com{}'.format(pageUrl))
#    bs = BeautifulSoup(html, 'html.parser')
#    try:
#        print(bs.title) # 数据收集
#    except AttributeError:
#        print('页面缺少一些属性！不过不用担心')
#
#    for link in bs.find_all('a', href=re.compile('^(/item/)')):
#        if 'href' in link.attrs:
#            if link.attrs['href'] not in pages:
#                newPage = link.attrs['href']
#                print(newPage)
#                print('-'*20)
#                pages.add(newPage)
#                encode = urllib.parse.quote(newPage)
#                getLinks(encode)
#
#
#getLinks('')




'''
互联网抓取
'''
from urllib.request import urlopen
from urllib.parse import urlparse
from bs4 import BeautifulSoup
import re
import datetime
import random
import time

pages = set()
random.seed(datetime.datetime.now())

# 获取页面中所有内链列表
def getInternalLinks(bs, includeUrl):
    internalLinks = []
    #找出所有以 / 开头或包含当前 URL 的链接
    for link in bs.find_all('a', 
            href=re.compile('^(/|.*' + includeUrl +')')):
        if link.attrs['href'] is not None:
            if link.attrs['href'] not in internalLinks:
                if (link.attrs['href'].startswith('/')):
                    internalLinks.append(
                            includeUrl + link.attrs['href'])
                else:
                    internalLinks.append(link.attrs['href'])
    return internalLinks


# 获取页面中所有外链列表
def getExternalLinks(bs, excludeUrl):
    externalLinks = []
    # 找出所有以"https"或"www"开头，且不好含当前 URL 的链接
    for link in bs.find_all('a', 
            href=re.compile('^(https|www)((?!'+excludeUrl+').)*$')):
        if link.attrs['href'] is not None:
            if link.attrs['href'] not in externalLinks:
                externalLinks.append(link.attrs['href'])
    return externalLinks

def getRandomExternalLink(startingSite):
    time.sleep(5)
    html = urlopen(startingSite)
    bs = BeautifulSoup(html, 'html.parser')
    externalLinks = getExternalLinks(bs, urlparse(startingSite).netloc)
    if len(externalLinks) == 0:
        print('No external links, look around the site for one')
        domain = '{}://{}'.format(urlparse(startingSite).scheme, urlparse(startingSite).netloc)
        internalLinks = getInternalLinks(bs, domain)
        return getRandomExternalLink(internalLinks[random.randint(0, len(internalLinks) - 1)])
    else:
        return externalLinks[random.randint(0, len(externalLinks) - 1)]

def followExternalOnly(startingSite):
    externalLink = getRandomExternalLink(startingSite)
    print('Random external link is : {}'.format(externalLink))
    followExternalOnly(externalLink)

followExternalOnly('https://www.oreilly.com/')
























#!bin/python3.8

'''
1. 下载多媒体文件

'''
'''
下载某个图片
'''

#from urllib.request import urlretrieve
#from urllib.request import urlopen
#from bs4 import BeautifulSoup
#
#html = urlopen("http://www.pythonscraping.com")
#bs = BeautifulSoup(html, 'html.parser');
#
#title = bs.find('div', {'class': 'pagelayer-wp-title-heading'}, text='Web Scraping with Python')
#
#imageLocation = title.parent.parent.find('img')['src']
#
#urlretrieve(imageLocation, 'logo.jpg')  # 下载图片


'''
下载页面上所有文件
'''
#import os
#from urllib.request import urlretrieve
#from urllib.request import urlopen
#from bs4 import BeautifulSoup
#
#downloadDirectiry = 'downloaded'
#baseUrl = 'https://pythonscraping.com'
#
#def getAbsoluteURL(baseUrl, source):
#    if source.startswith('https://www.'):
#        url = 'https://{}'.format(source[12:])
#    elif source.startswith('https://'):
#        url = source
#    elif source.startswith('www.'):
#        source = source[4:]
#        url = 'https://{}'.format(source)
#    else:
#        url = '{}/{}'.format(baseUrl, source)
#
#    if baseUrl not in url:
#        return None
#    return url
#
#def getDownLoadPath(baseUrl, absoluteUrl, downloadDirectiry):
#    path = absoluteUrl.replace('www.', '')
#    path = path.replace(baseUrl, '')
#    path = downloadDirectiry + path
#    directory = os.path.dirname(path)
#    if not os.path.exists(directory):
#        os.makedirs(directory)
#    return path
#
#
#html = urlopen("https://www.pythonscraping.com")
#bs = BeautifulSoup(html, 'html.parser');
#downloadList = bs.find_all(src=True)
#
#for download in downloadList:
#    fileUrl = getAbsoluteURL(baseUrl, download['src'])
#    if fileUrl is not None:
#        print(fileUrl)
#        urlretrieve(fileUrl,getDownLoadPath(baseUrl, fileUrl, downloadDirectiry))


'''
2. 写 csv 文件
'''
#import csv
#
#csvFile = open('test.csv', 'w+')
#
#try:
#    writer = csv.writer(csvFile)
#    writer.writerow(('number', 'number plus 2', 'number times 2')
#    for i in range(10):
#        writer.writerow((i, i * 2, i * i))
#finally:
#    csvFile.close()

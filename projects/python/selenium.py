#!bin/python3.8

'''
selenium 是请求网页数据的一种选择，可以驱动浏览器
'''

'''
环境
   firefox
        apt install firefox
   geckodriver
        wget https://github.com/mozilla/geckodriver/releases/download/v0.30.0/geckodriver-v0.30.0-linux64.tar.gz
        tar -xvf geckodriver-v0.30.0-linux64.tar.gz
        mv geckodriver /usr/local/bin
   selenium
        pip3 install selenium
'''



'''
1. 选择器
'''
#from bs4 import BeautifulSoup
#from selenium import webdriver
#import time
#
##选项
#options = webdriver.FirefoxOptions()
#options.add_argument('--headless')
#options.add_argument('--disable-gpu')
#
## 驱动
#driver = webdriver.Firefox(options=options)
#
## 请求
#driver.get('http://pythonscraping.com/pages/javascript/ajaxDemo.html')
#
#time.sleep(3)
#
## 选择器
#print(driver.page_source)
#
#'''
#print(driver.find_element_by_id('content').text)
#'''
#
#'''
#print(driver.find_element_by_css_selector('#content').text)
#'''
#
#contents = driver.find_elements_by_css_selector('#content')    # 选择列表
#for content in contents:
#    print(content.text)
#
#'''
#print(driver.find_element_by_tag_name('div').text)
#'''
#
#'''
#pageSource = driver.page_source
#bs = BeautifulSoup(pageSource, 'html.parser')
#print(bs.find(id='content').get_text())
#'''
#
#driver.close()


'''
2. 隐式等待
'''
#from selenium import webdriver
#from selenium.webdriver.common.by import By                         # 定位器, 支持的属性: ID,CLASS_NAME,CSS_SELECTOR,LINK_TEXT,PARTIAL_TEXT,NAME,TAG_NAME,XPATH
#from selenium.webdriver.support.ui import WebDriverWait             # 隐式等待
#from selenium.webdriver.support import expected_conditions as EC    # 隐式等待条件
#
#
##选项
#options = webdriver.FirefoxOptions()
#options.add_argument('--headless')
#options.add_argument('--disable-gpu')
#
## 驱动
#driver = webdriver.Firefox(options=options)
#
## 请求
#driver.get('http://pythonscraping.com/pages/javascript/ajaxDemo.html')
#
#try:            # 尝试做某件事
#    element = WebDriverWait(driver,10).until(                           # 核心代码
#            EC.presence_of_element_located((By.ID,'loadedButton')))     # 查找 ID 为 loadedButton 的元素
#except:         # 如果，try 异常，则执行
#    print("Error")
#else:           # 如果 try 没有异常，则执行(前提是，try 中没有 return)
#    print('OK')
#
#finally:        # 不管 try 有没有异常，是否有 return 都会执行
#   # print(driver.find_element_by_id('content').text)
#    print(driver.find_element(By.ID, 'content').text)           # 定位器转选择器
#    driver.close()



'''
3 客户端重定向 - 判断重定向是否已完成
'''
'''
检查跳转前的元素
'''
#from selenium import webdriver
#import time
#from selenium.webdriver.common.by import By                         # 定位器, 支持的属性: ID,CLASS_NAME,CSS_SELECTOR,LINK_TEXT,PARTIAL_TEXT,NAME,TAG_NAME,XPATH
#from selenium.webdriver.remote.webelement import WebElement
#from selenium.common.exceptions import StaleElementReferenceException
#
#
#
##选项
#options = webdriver.FirefoxOptions()
#options.add_argument('--headless')
#options.add_argument('--disable-gpu')
#
## 驱动
#driver = webdriver.Firefox(options=options)
#
#def waitForLoad(driver):
#    elem = driver.find_element(By.TAG_NAME, value='html')
#    count = 0
#    while True:
#        count += 1
#        if count > 20:
#            print('Timing out after 10 seconds and returning')
#            return
#        time.sleep(.5)
#        try:
#            elem = driver.find_element(By.TAG_NAME, value='html')
#        except StaleElementReferenceException:                          # 页面发生重定向后，抛出该异常
#            return
#
## 请求
#driver.get('http://pythonscraping.com/pages/javascript/redirectDemo1.html')
#print(driver.page_source)
#waitForLoad(driver)
#print(driver.page_source)
#driver.close()
#
#

'''
检查跳转后的元素
'''
#from selenium import webdriver
#from selenium.webdriver.common.by import By                         # 定位器, 支持的属性: ID,CLASS_NAME,CSS_SELECTOR,LINK_TEXT,PARTIAL_TEXT,NAME,TAG_NAME,XPATH
#from selenium.webdriver.support.ui import WebDriverWait             # 隐式等待
#from selenium.webdriver.support import expected_conditions as EC    # 隐式等待条件
#from selenium.common.exceptions import TimeoutException
#
#
##选项
#options = webdriver.FirefoxOptions()
#options.add_argument('--headless')
#options.add_argument('--disable-gpu')
#
## 驱动
#driver = webdriver.Firefox(options=options)
#
## 请求
#driver.get('http://pythonscraping.com/pages/javascript/redirectDemo1.html')
#
#try:
#    bodyElement = WebDriverWait(driver, 15).until(EC.presence_of_element_located(
#        (By.XPATH, '//body[contains(text(),"This is the page you are looking for!")]')))
#    print(bodyElement.text)
#except:
#    print('Did not find the element')
#
#driver.close()

#!bin/python3.9

import requests
import json
from http.cookiejar import LWPCookieJar
import getpass


# application/x-www-form-urlencoded, multipart/form-data, application/json
headers = {
    "Content-Type":"application/json;charset=utf-8",
}

# 实例化 session 对象
session = requests.session()
session.headers = headers
session.cookies = LWPCookieJar(filename='/tmp/cookies')

def verify_login():
	url = "https://www.lixinger.com/api/stock/stocks/stock-collections"
	data = {
		"stockType":"index"
	}
	response = session.post(url, data = json.dumps(data))
	print(response.text)

def login():
	username = input("username:")
	passwd   = getpass.getpass("passwd:")


	url = "https://www.lixinger.com/api/account/sign-in/by-account"

	data = {
        	"accountName" : username,
        	"password"    : passwd
	}



	# 提交请求
	response = session.post(url,  data = json.dumps(data))

	session.cookies.save()  # 保存 cookie

	verify_login()


def cookie_login():
	try:
		session.cookies.load(ignore_discard=True)
	except Exception:
		print("Cookies 未能加载，使用密码登录")
		login()
	else:
		verify_login()


if __name__ == "__main__":
	cookie_login()

#!bin/python3.8

#import json
#from urllib.request import urlopen

#def getCountry(ipAddress):
#    response = urlopen('https://ip.taobao.com/outGetIpInfo?ip=' + ipAddress).read().decode('utf-8')
#    responseJson = json.loads(response)
#    return responseJson.get("data")["country"]

#print(getCountry("50.78.253.58"))


#import json
#
#jsonString = '{ "arrayOfNums":[{"number":0}, {"number":1}, {"number":2}], "arrayOfFruits":[{"fruit":"apple"}, {"fruit":"banana"}, {"fruit":"pear"}]}'
#
#jsonObj = json.loads(jsonString)
#
#print(jsonObj.get("arrayOfNums"))
#print(jsonObj.get("arrayOfNums")[1])
#print(jsonObj.get("arrayOfNums")[1].get("number") + jsonObj.get("arrayOfNums")[2].get("number"))
#print(jsonObj.get("arrayOfFruits")[2].get("fruit"))

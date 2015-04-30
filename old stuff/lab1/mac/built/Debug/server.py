import requests, sys

url = 'http://www.elliotfiske.com/slumber.py'
num = sys.argv[1]
payload = {'command': num}

# GET
r = requests.get(url)

# POST with form-encoded data
r = requests.post(url, data=payload)

# Response, status etc
r.text
r.status_code

# print r.text.strip()[-1]
# print r.text


import requests, sys
url = 'http://www.elliotfiske.com/slumber.py'
payload = {'key1': 'value1', 'key2': 'value2'}

# GET
r = requests.get(url)

# Response, status etc
r.text
r.status_code

print r.text.strip()[-1]
# sys.exit(int(r.text.strip()[-1]))

import requests, sys
url = 'http://www.elliotfiske.com/slumber.py'

# GET
r = requests.get(url)

# print r.text.strip()[-1]
sys.exit(int(r.text.strip()[-1]))

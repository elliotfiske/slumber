import urllib, urllib2, sys

url = 'http://www.elliotfiske.com/slumber.py'
num = sys.argv[1]
payload = {'command': num}
data = urllib.urlencode(payload)

req = urllib2.Request(url, data)
response = urllib2.urlopen(req)

# GET
# r = requests.get(url)

# POST with form-encoded data
# r = requests.post(url, data=payload)

# Response, status etc
# r.text
# r.status_code

# print r.text.strip()[-1]
# print r.text


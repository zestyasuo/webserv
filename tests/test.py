import requests

if __name__ == "__main__" :
	res = requests.get('http://localhost:8080', stream=True)
	print(res.raw.read(1000))
import requests

if __name__ == "__main__" :
	webserv_res = requests.get('http://localhost:8080', stream=True)
	nginx_res = requests.get('http://localhost:8090', stream=True)
	
	print(webserv_res.status_code)
	print(nginx_res.status_code)
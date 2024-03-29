if [[ ! -d ./webserv ]]; then
	python3 -m venv webserv
	source webserv/bin/activate
	pip install --upgrade pip
	pip install requests
else
	source webserv/bin/activate
fi


{
	exec ../webserv

}&
{
	exec nginx -g 'daemon off;' -c /home/zyasuo/21school/my_server/tests/nginx.conf
}&
python3 test.py

pkill webserv
pkill nginx
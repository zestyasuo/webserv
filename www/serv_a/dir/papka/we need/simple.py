import sys
import os

print("<!doctype html><html>")

print("<h3>Python works ~~~~~:)</h3>")
# print(sys.path)

print("<style>body {font-family:Fira Code}</style>")

print("<body>")

# print("Color: ", requests.GET.get('color'));

path = os.environ.get('PATH_INFO', '')
if path:
    print('PATH_INFO:', path)
else:
    print('PATH_INFO not set!')

print("<br><br>")

#   Environment
for i, j in os.environ.items():
    print(i, ":<b>", j, "</b><br>")

# print(os.environ)

print("</body>")
print("</html>")













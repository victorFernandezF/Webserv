#!//usr/bin/python3

import os

try:
	print("Hola " + os.environ["first_name"] + " " + os.environ["last_name"] + "!")
except:
	print("Hola Anonimo!")
import os
from sys import argv
from textwrap import wrap

run, filename = argv

with open(filename,'r') as f:
	lines = f.readlines()

for line in lines:
	if (line.startswith("#") | line.startswith("\n") | line.startswith("\r")):
		continue
	if (line.startswith("//")):
		print("	 " + line)
		continue
	line = line.replace(" ", "")
	address = line.split(':')[0]
	value = line.split(':')[3]
	value = wrap(value, 2)
	value = ', 0x'.join(value)
	value = "0x" + value
	print("	{ (void*)" + address + ",{ " + value + " } },")
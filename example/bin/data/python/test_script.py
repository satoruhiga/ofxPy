print 'hello from test_script.py'

def my_fn():
	print 'hello from python function'

def get_random():
	import random
	return (random.random(), random.random())

def size_expression(t):
	import math
	return abs(math.sin(t * math.pi) + math.sin(t * math.pi * 1.5))
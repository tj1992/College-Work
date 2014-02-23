#!/usr/bin/python3.3
# k-means clustering

# import plotting functionality
import matplotlib.pyplot as plt

class Point:
	def __init__(self, n = 0):
		self.x = [ 0 ] * n
		self.y = [ 0 ] * n
		self.flag = [ 0 ] * n

# returns the euclidean distance between the points (x1, y1) and (x2, y2)
def dist(x1, y1, x2, y2):
	return (x1-x2)**2+(y1-y2)**2

# returns the index of closest point (from list c) to point (x, y)
def min_point(c, x, y):
	minimum = ( 0, dist(c.x[0], c.y[0], x, y) )
	for i in range(1, len(c.x)):
		d = dist(c.x[i], c.y[i], x, y)
		if d < minimum[1]:
			minimum = (i, d)
	
	return minimum[0]

# read the data from file 'xy_data'
def read_data(filename):
	points = Point()
	try:
		with open(filename, 'r') as fin:
			for i in fin:			# read next line
				x, y = i.split()	# split the line into whitespace separated items
				points.x.append(int(x))
				points.y.append(int(y))
				points.flag.append(-1)
	except FileNotFoundError:
		print ('Error: Unable to read file', filename)
		print ('Aborting')
		sys.exit(1)
	return points

# returns len(color)*len(mark)(=140) different plotting style markers
def style(index):
	color = [ 'r', 'g', 'b', 'c', 'm', 'y', 'k' ]
	mark = [ 'o', 'v', '^', '<', '>', '1', '2', '3', '4', 's', 'p', '*', 'h', 'H', '+', 'x', 'D', 'd', '|', '_' ]

	return color[index % len(color)]+mark[int(index / len(color)) % len(mark)]

# plot the graph of the supplied points
def plot_graph(points, k):
	arr = [ ]		# for holding points according to flag value
	for i in range(0,k):
		arr.append(Point())
	
	length = len(points.x)
	for i in range(0, length):
		arr[points.flag[i]].x.append(points.x[i])	# append to array with same flag value
		arr[points.flag[i]].y.append(points.y[i])

	plot_args = ()
	for i in range(0, k):
		tup = (arr[i].x, arr[i].y, style(i))		# create tuple of x, y, style
		plot_args += tup		# append to arguments' list

	# set labels and limits
	plt.xlim(0, max(points.x)+10)
	plt.ylim(0, max(points.y)+10)
	plt.xlabel('X-axis')
	plt.ylabel('Y-axis')

	# finally plot it!
	plt.plot(*plot_args)

	# create annotaions for centroids
	for i in range(k, 0, -1):
		plt.annotate('Centroid '+str(k-i+1), xy = (points.x[length-i], points.y[length-i]))

	# display the plot
	plt.show()

# Main
if __name__ == "__main__":
	import sys
	import getopt
	import random

	# get arguments
	args = getopt.getopt(sys.argv[1:], 'k:f:i:')

	# defaults
	k = 2
	filename = 'xy_data'
	iters = 30

	# get supplied values
	for i in args[0]:
		if i[0] == '-k':
			k = int(i[1])
		elif i[0] == '-f':
			filename = i[1]
		elif i[0] == '-i':
			iters = int(i[1])
		else:
			print ('Usage: [-k <k-parameter> -f <filename> -i <num-iterations> ]')
			sys.exit(1)
	
	print ('Using file', filename, 'as input and', k, 'as k-parameter')

	# read to 'points'
	points = read_data(filename)
	length = len(points.x)

	if k > length:
		print ('k-parameter > input size')
		print ('Aborting')
		sys.exit(1)
	
	# set default centroid points
	centroid = Point()
	for i in range(0, k):
		ri = random.randrange(length)
		centroid.x.append(points.x[ri])
		centroid.y.append(points.y[ri])
	
	print ('Before:')
	for i in range(0, k):
		print ('Centroid', i+1, ':', centroid.x[i], centroid.y[i])

	# calculate the centroid (max iters iterations)
	for j in range(0, iters):
		nc = Point(k)
	
		for i in range(0, length):	# iterate over the list of points
			flag = min_point(centroid, points.x[i], points.y[i])	# get the closest centroid point
			points.flag[i] = flag
			nc.x[flag] += points.x[i]	# accumulate the x-coordinate value
			nc.y[flag] += points.y[i]	# accumulate the y-coordinate value
			nc.flag[flag] += 1	# increment the element count
	
		# calculate the mean value
		for i in range(0, k):
			nc.x[i] = nc.x[i]/nc.flag[i]
			nc.y[i] = nc.y[i]/nc.flag[i]
	
		if nc.x == centroid.x and nc.y == centroid.y:	# compare with previous values
			break
		else:
			centroid.x = nc.x		# copy references
			centroid.y = nc.y
		
	print ('After', j+1, 'iterations:')
	for i in range(0, k):
		print ('Centroid', i+1, ':', centroid.x[i], centroid.y[i])
	
	centroid.flag = list(range(0, k))	# set flag value for plotting
	points.x.extend(centroid.x)	# add centroid points to list of points
	points.y.extend(centroid.y)
	points.flag.extend(centroid.flag)
	
	# plot
	plot_graph(points, k)

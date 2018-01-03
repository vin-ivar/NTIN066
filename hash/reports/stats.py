import matplotlib.pyplot as plt
import numpy as np
import sys

labels = [] ; elems = []
mins, maxs, means, medians, deciles = [], [], [], [], [] 
metrics = [mins, maxs, means, medians, deciles]
names = ['Mins', 'Maxes', 'Means', 'Medians', 'Deciles']
number_of_plots = 1

for line in sys.stdin:
	label = int(line.split("\t")[0][1:])
	elem = [int(i) for i in line.rstrip("\n").split("\t")[1:-1]]
	labels.append(label)
	elems.append(elem)
	mins.append(min(elem))
	maxs.append(max(elem))
	means.append(np.mean(elem))
	medians.append(np.median(elem))
	deciles.append(np.percentile(elem, 90))

lines1, lines2 = [], []
mid = int(len(mins))

for name, metric in zip(names,metrics):
	lines1.append(plt.plot(labels[:mid], metric[:mid], '-', label=name))

for n, (l1, l2) in enumerate(zip(lines1, lines2)):
	plt.setp(l1, color='C{}'.format(n+3))

#plt.yscale('log')
plt.legend()
#plt.show()
plt.savefig('plot4.png')

# lmin = plt.plot(labels, mins, '-', label='Minimums')
# lmax = plt.plot(labels, maxs, '-', label='Maximums')
# lmean = plt.plot(labels, means, '-', label='Means')
# lmedian = plt.plot(labels, medians, '-', label='Medians')
# ldecile = plt.plot(labels, deciles, '-', label='Deciles')

# plt.setp(lmin, color='C3')
# plt.setp(lmax, color='C4')
# plt.setp(lmean, color='C5')
# plt.setp(lmedian, color='C6')
# plt.setp(ldecile , color='C7')

# plt.legend()
# plt.show()

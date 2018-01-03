import matplotlib.pyplot as plt
import numpy as np
import sys

d = {} ; m = {}
labels = [] ; elems = []
mins, maxs, means, medians, deciles = [], [], [], [], [] 
metrics = ['mins', 'maxs', 'means', 'medians', 'deciles']
names = ['Mins', 'Maxes', 'Means', 'Medians', 'Deciles']
number_of_plots = 1

for i in metrics:
	m[i] = []

for line in sys.stdin:
	if line == "":
		continue
	label = int(line.split("\t")[0][1:])
	elem = [int(i) for i in line.rstrip("\n").split("\t")[1:-1]]
	try:
		d[label].extend(elem)
	except KeyError:
		d[label] = elem

print(d[8])
for k in d.keys():
	m['mins'].append(min(d[k]))
	m['maxs'].append(max(d[k]))
	m['means'].append(np.mean(d[k]))
	m['medians'].append(np.median(d[k]))
	m['deciles'].append(np.percentile(d[k], 90))

lines1 = []

for name, metric in zip(names,metrics):
	print(d.keys())
	print(m[metric])
	lines1.append(plt.plot(list(d.keys()), m[metric], '-', label=name))

for n, l1 in enumerate(lines1):
	plt.setp(l1, color='C{}'.format(n+3))

#plt.yscale('log')
plt.legend()
plt.show()

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

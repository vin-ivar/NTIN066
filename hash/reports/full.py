import sys
import numpy as np
import matplotlib.pylab as plt

sizes = []
arrs = []
maxs, mins, means, medians, deciles = [], [], [], [], []
metrics = [maxs, mins, means, medians, deciles]
labels = ["Maximums", "Minimums", "Means", "Medians", "Deciles"]

for line in sys.stdin:
	num = int(line.split("\t")[0][1:])
	sizes.append(num)
	vals = [float(i) for i in line.split("\t")[1:]]
	maxs.append(np.max(vals))
	mins.append(np.min(vals))
	means.append(np.mean(vals))
	medians.append(np.median(vals))
	deciles.append(np.percentile(vals, 10))

lines = []
for metric, label in zip(metrics, labels):
	lines.append(plt.plot(sizes, metric, '-', label=label))

for n, l in enumerate(lines):
	plt.setp(l, color='C{}'.format(n+3))

plt.legend()
plt.savefig('tab2.png')

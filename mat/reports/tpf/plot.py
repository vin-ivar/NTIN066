import matplotlib.pyplot as plt
import sys

X_cl = []
y_cl = []
X_nv = []
y_nv = []
plt.figure(figsize=(12,8))
with open("input.txt", "r") as f:
	for line in f:
		size, time = line.rstrip("\n").split("\t")
		X_cl.append(float(size))
		y_cl.append(float(time)/ 100)

with open("input_nv.txt", "r") as f:
	for line in f:
		size, time = line.rstrip("\n").split("\t")
		X_nv.append(float(size))
		y_nv.append(float(time)/ 100)

line1 = plt.plot(X_cl, y_cl, '-', label="Classic")
line2 = plt.plot(X_nv, y_nv, '-', label="Naive")
#plt.setp(line1, color='C2')
#plt.setp(line2, color='C3')
axes = plt.gca() 
axes.set_ylim([0, 45])
plt.xscale('log', basex=2)
plt.xlabel('Size of one dimension')
plt.ylabel('Time in ns')

plt.legend()
plt.savefig('fig1.png')

import matplotlib.pyplot as plt
import sys

X_cl = []
y_cl = []
X_nv = []
y_nv = []
with open("report", "r") as f:
	for line in f:
		size, access, fault = line.rstrip("\n").split("\t")
		X_cl.append(int(size))
		y_cl.append(int(fault) / (int(access) / 2))

with open("report_nv", "r") as f:
	for line in f:
		size, access, fault = line.rstrip("\n").split("\t")
		X_nv.append(int(size))
		y_nv.append(int(fault) / (int(access) / 2))

line1 = plt.plot(X_cl, y_cl, '-', label="Classic")
line2 = plt.plot(X_nv, y_nv, '-', label="Naive")
plt.setp(line1, color='C2')
plt.setp(line2, color='C3')
plt.xlabel('Size')
plt.ylabel('PFs / access')

plt.legend()
plt.show()

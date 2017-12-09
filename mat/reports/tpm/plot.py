import matplotlib.pyplot as plt
import sys

X_cl = []
y_cl = []
X_nv = []
y_nv = []
with open("input.txt", "r") as f:
	for line in f:
		size, time = line.rstrip("\n").split("\t")
		X_cl.append(int(size))
		y_cl.append(int(time))

with open("input_nv.txt", "r") as f:
	for line in f:
		size, time = line.rstrip("\n").split("\t")
		X_nv.append(int(size))
		y_nv.append(int(time))

line1 = plt.plot(X_cl, y_cl, '-', label="Classic")
line2 = plt.plot(X_nv, y_nv, '-', label="Naive")
plt.setp(line1, color='C2')
plt.setp(line2, color='C3')
plt.xlabel('Size')
plt.ylabel('Time')

plt.legend()
plt.show()

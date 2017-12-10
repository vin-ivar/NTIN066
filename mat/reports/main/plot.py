import matplotlib.pyplot as plt
import sys, math

X_6464 = [] ; y_6464 = []
X_641024 = [] ; y_641024 = []
X_644096 = [] ; y_644096 = []
X_512512 = [] ; y_512512 = []
X_409664 = [] ; y_409664 = []

with open("64_64.tsv", "r") as f:
	for line in f:
		elements, accesses, misses = line.rstrip("\n").split("\t")
		X_6464.append(math.sqrt(float(elements)))
		y_6464.append(float(misses) / (float(accesses)))

with open("64_1024.tsv", "r") as f:
	for line in f:
		elements, accesses, misses = line.rstrip("\n").split("\t")
		X_641024.append(math.sqrt(float(elements)))
		y_641024.append(float(misses) / (float(accesses)))

with open("64_4096.tsv", "r") as f:
	for line in f:
		elements, accesses, misses = line.rstrip("\n").split("\t")
		X_644096.append(math.sqrt(float(elements)))
		y_644096.append(float(misses) / (float(accesses)))

with open("512_512.tsv", "r") as f:
	for line in f:
		elements, accesses, misses = line.rstrip("\n").split("\t")
		X_512512.append(math.sqrt(float(elements)))
		y_512512.append(float(misses) / (float(accesses)))

with open("4096_64.tsv", "r") as f:
	for line in f:
		elements, accesses, misses = line.rstrip("\n").split("\t")
		X_409664.append(math.sqrt(float(elements)))
		y_409664.append(float(misses) / (float(accesses)))

line1 = plt.plot(X_6464, y_6464, '-', label="64_64")
line2 = plt.plot(X_641024, y_641024, '-', label="64_1024")
line3 = plt.plot(X_644096, y_644096, '-', label="64_4096")
line4 = plt.plot(X_512512, y_512512, '-', label="512_512")
line5 = plt.plot(X_409664, y_409664, '-', label="4096_64")
plt.setp(line1, color='C3')
plt.setp(line2, color='C4')
plt.setp(line3, color='C5')
plt.setp(line4, color='C6')
plt.setp(line5, color='C7')
plt.xscale('log')
plt.xlabel('Size')
plt.ylabel('Accesses')

plt.legend()
plt.show()

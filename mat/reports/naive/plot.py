import matplotlib.pyplot as plt
import sys

X_6464 = [] ; y_6464 = []
X_641024 = [] ; y_641024 = []
X_644096 = [] ; y_644096 = []
X_512512 = [] ; y_512512 = []
X_409664 = [] ; y_409664 = []

with open("64_64.tsv", "r") as f:
	for line in f:
		elements, accesses, misses = line.rstrip("\n").split("\t")
		X_6464.append(elements)
		y_6464.append(float(misses) / (float(accesses) / 2))

with open("64_1024.tsv", "r") as f:
	for line in f:
		elements, accesses, misses = line.rstrip("\n").split("\t")
		X_641024.append(elements)
		y_641024.append(float(misses) / (float(accesses) / 2))

with open("64_4096.tsv", "r") as f:
	for line in f:
		elements, accesses, misses = line.rstrip("\n").split("\t")
		X_644096.append(elements)
		y_644096.append(float(misses) / (float(accesses) / 2))

with open("512_512.tsv", "r") as f:
	for line in f:
		elements, accesses, misses = line.rstrip("\n").split("\t")
		X_512512.append(elements)
		y_512512.append(float(misses) / (float(accesses) / 2))

with open("4096_64.tsv", "r") as f:
	for line in f:
		elements, accesses, misses = line.rstrip("\n").split("\t")
		X_409664.append(elements)
		y_409664.append(float(misses) / (float(accesses) / 2))

line1 = plt.plot(X_6464, y_6464, '-', label="Classic")
line2 = plt.plot(X_641024, y_641024, '-', label="Naive")
line3 = plt.plot(X_644096, y_644096, '-', label="Naive")
line4 = plt.plot(X_512512, y_512512, '-', label="Naive")
line5 = plt.plot(X_409664, y_409664, '-', label="Naive")
plt.setp(line1, color='C3')
plt.setp(line2, color='C4')
plt.setp(line3, color='C5')
plt.setp(line4, color='C6')
plt.setp(line5, color='C7')
plt.xlabel('Size')
plt.ylabel('Accesses')

plt.legend()
plt.show()

import matplotlib.pyplot as plt


with open("special_naive.tsv") as f:
	dump_nv = f.read().split("\n")[:-1]

with open("special_classic.tsv") as f:
	dump_cl = f.read().split("\n")[:-1]

X_nv, y_nv = [], []
X_cl, y_cl = [], []

for line in dump_cl:
	cols = line.split("\t")
	try:
		X_cl.append(cols[1]) ; y_cl.append(cols[0])
	except:
		pass

for line in dump_nv:
	cols = line.split("\t")
	try:
		X_nv.append(cols[1]) ; y_nv.append(cols[0])
	except:
		pass
 
line1 = plt.plot(X_cl, y_cl, '-', label="Classic")
line2 = plt.plot(X_nv, y_nv, '-', label="Naive")
plt.setp(line1, color='C2')
plt.setp(line2, color='C5')
plt.xlabel('Heap size')
plt.ylabel('Number of operations')

plt.legend()
plt.savefig('fig_2.png')

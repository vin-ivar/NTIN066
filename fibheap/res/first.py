import matplotlib.pyplot as plt

colors = ['C{}'.format(i) for i in range(2)]

for color in colors:
	with open("random.tsv") as f:
		dump_nv = f.read().split("\n")[:-1]

	with open("biased.tsv") as f:
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
	 

line1 = plt.plot(X_cl, y_cl, '-', label="Biased")
line2 = plt.plot(X_nv, y_nv, '-', label="Random")
plt.setp(line1, color='C4')
plt.setp(line2, color='C2')
plt.xlabel('Heap size')
plt.ylabel('Number of operations')

plt.legend()
plt.savefig('fig_1.png')

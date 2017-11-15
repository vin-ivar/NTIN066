import matplotlib.pyplot as plt

plt.figure(figsize=(12,8))
sizes = [10000, 100000, 1000000]
colors = ['C{}'.format(i) for i in range(6)]

for size, color in zip(sizes, colors):
    with open("{}.nv.tsv".format(size)) as f:
        dump_nv = f.read().split("\n")[1:]

    with open("{}.cl.tsv".format(size)) as f:
        dump_cl = f.read().split("\n")[1:]

    X_nv, y_nv = [], []
    X_cl, y_cl = [], []

    for line in dump_cl:
        cols = line.split("\t")
        try:
            X_cl.append(cols[1]) ; y_cl.append(cols[2])
        except:
            pass
    
    for line in dump_nv:
        cols = line.split("\t")
        try:
            X_nv.append(cols[1]) ; y_nv.append(cols[2])
        except:
            pass
     
    line1 = plt.plot(X_cl, y_cl, '-', label="{}".format(size))
    line2 = plt.plot(X_nv, y_nv, ':')
    plt.setp(line1, color=color)
    plt.setp(line2, color=color)
    plt.xlabel("Set size")
    plt.ylabel("Average path length")

plt.legend()
plt.savefig('../submit/figure_3.png')

import matplotlib.pyplot as plt

plt.figure(figsize=(12,8))
sizes = [10, 100, 1000, 10000, 100000, 1000000]
for size in sizes:
    with open("{}.cl.tsv".format(size)) as f:
        dump_cl = f.read().split("\n")[1:]

    X_cl, y_cl = [], []

    for line in dump_cl:
        cols = line.split("\t")
        try:
            X_cl.append(cols[1]) ; y_cl.append(cols[2])
        except:
            pass
    plt.xlabel("Set size")
    plt.ylabel("Average path length")
    plt.plot(X_cl, y_cl, label="{}".format(size))


plt.legend()
plt.savefig('../submit/figure_1.png')
#plt.show()

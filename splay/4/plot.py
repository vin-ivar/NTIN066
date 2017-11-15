import matplotlib.pyplot as plt

plt.figure(figsize=(12,8))
sizes = {'n': 'Naive', 'c': 'Classical'}
for size in sizes:
    with open("{}.tsv".format(size)) as f:
        dump_nv = f.read().split("\n")

    X_nv, y_nv = [], []

    for line in dump_nv:
        cols = line.split("\t")
        try:
            X_nv.append(cols[1]) ; y_nv.append(cols[2])
        except:
            pass
    plt.xlabel("Set size")
     
    plt.ylabel("Average path length")
  
    plt.plot(X_nv, y_nv, label="{}".format(sizes[size]))

plt.legend()
plt.savefig('../submit/figure_4.png')

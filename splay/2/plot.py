import matplotlib.pyplot as plt

plt.figure(figsize=(12,8))
sizes = [10, 100, 1000, 10000, 100000, 1000000]
for size in sizes:
    with open("{}.nv.tsv".format(size)) as f:
        dump_nv = f.read().split("\n")[1:]

    X_nv, y_nv = [], []

    for line in dump_nv:
        cols = line.split("\t")
        try:
            X_nv.append(cols[1]) ; y_nv.append(cols[2])
        except:
            pass
    
    plt.xlabel("Set size")
    plt.ylabel("Average path length")
  
    plt.plot(X_nv, y_nv, label="{}".format(size))

plt.legend()
plt.savefig('../submit/figure_2.png')

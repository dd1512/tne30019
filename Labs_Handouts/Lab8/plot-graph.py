import csv
import matplotlib.pyplot as plt
from itertools import accumulate

filename = "80.csv"
with open(filename, "r") as csv_file:
    csv_reader = csv.reader(csv_file)

    flows = {}
    for record in csv_reader:
        # print(record[0])
        if record[0] not in flows:
            flows[record[0]] = {"timestamp": [], "length": []}
        
        flows[record[0]]["timestamp"].append(float(record[1]))
        flows[record[0]]["length"].append(int(record[2]))

fig, ax = plt.subplots()
for fid, data in flows.items():
    accum = list(accumulate(data["length"]))
    ax.plot(data["timestamp"], accum, label=fid, marker=".", linestyle="none")


plt.show()

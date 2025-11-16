import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv("results.csv")



def remove_outliers(group, factor=5.0):

    median = group["time_ms"].median()
    if median > 0:
        return group[group["time_ms"] <= factor * median]
    else:
        return group

df_clean = (
    df.groupby(["type", "threshold"], group_keys=False)
      .apply(remove_outliers, factor=5.0)
)

print("До очистки:", len(df), "строк")
print("После очистки:", len(df_clean), "строк")

df = df_clean


types = ["random", "reversed", "almost"]
thresholds = sorted(df["threshold"].unique())

for arr_type in types:
    sub = df[df["type"] == arr_type]

    plt.figure(figsize=(10, 6))

    for thr in thresholds:
        sub_thr = sub[sub["threshold"] == thr].sort_values("size")
        if sub_thr.empty:
            continue

        label = f"threshold = {thr}" if thr != 0 else "pure MERGE (0)"
        plt.plot(sub_thr["size"], sub_thr["time_ms"],
                 linewidth=1.2, label=label)

    plt.title(f"Зависимость времени сортировки от размера массива ({arr_type})")
    plt.xlabel("Размер массива, n")
    plt.ylabel("Время, мс")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"time_vs_size_{arr_type}.png", dpi=200)
    plt.close()


sizes_of_interest = [1000, 10000, 50000, 100000]

for arr_type in types:
    plt.figure(figsize=(10, 6))

    for n in sizes_of_interest:
        sub = df[(df["type"] == arr_type) & (df["size"] == n)]
        sub = sub.sort_values("threshold")
        if sub.empty:
            continue

        plt.plot(sub["threshold"], sub["time_ms"],
                 marker="o", linewidth=1.2, label=f"n = {n}")

    plt.title(f"Зависимость времени от threshold для разных n ({arr_type})")
    plt.xlabel("threshold")
    plt.ylabel("Время, мс")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"time_vs_threshold_{arr_type}.png", dpi=200)
    plt.close()

print("Готово! Графики перерисованы без выбросов.")

import matplotlib.pyplot as plt

# === Настройки ===
input_file = "pyinput.txt"
xlabel = "Максимальный вес ребра r"
ylabel = "Время работы (мс)"
title = "Сравнение времени работы алгоритмов"
other_params = "n = 1500, m = 1000n, q=1"

use_log = False
use_norm = False

use_log_x = use_log   # логарифмическая шкала по X
use_log_y = use_log    # логарифмическая шкала по Y

def main():
    global other_params
    
    # === Считывание данных ===
    param, t_a, t_b = [], [], []

    with open(input_file, 'r') as f:
        for line in f:
            if not line.strip():
                continue
            parts = line.split()
            if len(parts) < 3:
                continue
            p, a, b = map(float, parts[:3])
            param.append(p)
            if use_norm:
                t_a.append(a)
                t_b.append(b / p)
            else:
                t_a.append(a)
                t_b.append(b)

    # === Построение графика ===
    plt.figure(figsize=(8, 5))

    plt.plot(param, t_a, marker='o', label='Алгоритм A (Дейкстра)')
    plt.plot(param, t_b, marker='s', label='Алгоритм B (Форд-Беллман)')

    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    plt.legend()

    # логарифмические шкалы
    if use_log_x:
        plt.xscale("log")
    if use_log_y:
        plt.yscale("log")

    # подпись с параметрами

    if use_log:
        other_params += ", логарифмический масштаб"

    if use_norm:
        other_params += ", масштабирование по порядку роста"     
    
    plt.tight_layout(rect=[0, 0.05, 1, 1])
    plt.figtext(0.5, 0.01, f"Параметры: {other_params}", ha="center", fontsize=10)

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()

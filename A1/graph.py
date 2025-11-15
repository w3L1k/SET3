import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# --- 1. Определение констант ---

# Точное значение площади S_TRUE (для контрольной линии на графике сходимости)
# S = 0.25 * pi + 1.25 * arcsin(0.8) - 1
S_TRUE = 0.25 * np.pi + 1.25 * np.arcsin(0.8) - 1.0

# Имя файла, куда был сохранен вывод C++ кода
FILE_NAME = 'results.csv'

# --- 2. Загрузка данных ---
try:
    # Загружаем данные. Указываем, что разделитель - запятая, т.к. код выводит CSV
    df = pd.read_csv(FILE_NAME)
except FileNotFoundError:
    print(f"Ошибка: Файл '{FILE_NAME}' не найден.")
    print("Убедитесь, что вы запустили C++ код и сохранили его вывод в этот файл.")
    exit()

# --- 3. График первого типа: Сходимость площади (Area vs. N) ---
plt.figure(figsize=(12, 6))
plt.style.use('seaborn-v0_8-whitegrid')

# Узкая область (Area_Narrow)
plt.plot(df['N'], df['Area_Narrow'], 
         label='S, узкая область ([0.8, 2.1] x [0.8, 2.2])', 
         marker='o', markersize=3, linestyle='-', linewidth=1.5)

# Широкая область (Area_Wide)
plt.plot(df['N'], df['Area_Wide'], 
         label='S, широкая область ([0, 3.5] x [0, 3.5])', 
         marker='x', markersize=3, linestyle='-', linewidth=1.5)

# Точное значение (горизонтальная линия)
plt.axhline(y=S_TRUE, color='r', linestyle='--', 
            label=f'Точное значение S ≈ {S_TRUE:.6f}', 
            linewidth=2)

plt.title('График 1: Сходимость оценки площади S к точному значению', fontsize=16)
plt.xlabel('Количество точек N', fontsize=14)
plt.ylabel('Оценка площади S', fontsize=14)
plt.legend(loc='lower right', fontsize=10)
plt.grid(True, linestyle='--', alpha=0.6)
# plt.savefig('graph_1_convergence.png') # Раскомментируйте, чтобы сохранить график в файл
plt.show()

# --- 4. График второго типа: Относительная погрешность (Error vs. N) ---
plt.figure(figsize=(12, 6))
plt.style.use('seaborn-v0_8-whitegrid')

# Узкая область (Error_Narrow)
plt.plot(df['N'], df['Error_Narrow'], 
         label='δ, узкая область', 
         color='blue', marker='o', markersize=3, linestyle='-')

# Широкая область (Error_Wide)
plt.plot(df['N'], df['Error_Wide'], 
         label='δ, широкая область', 
         color='orange', marker='x', markersize=3, linestyle='-')

plt.title('График 2: Зависимость относительной погрешности δ от N', fontsize=16)
plt.xlabel('Количество точек N', fontsize=14)
plt.ylabel('Относительная погрешность δ', fontsize=14)

# Использование логарифмической шкалы для лучшей визуализации закона больших чисел
plt.xscale('log') 
plt.yscale('log') 

plt.legend(loc='upper right', fontsize=12)
plt.grid(True, which="both", linestyle='--', alpha=0.6)
# plt.savefig('graph_2_error.png') # Раскомментируйте, чтобы сохранить график в файл
plt.show()
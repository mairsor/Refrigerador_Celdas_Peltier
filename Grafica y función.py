import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Leer la tabla de datos desde el archivo Excel sin asumir los nombres de las columnas
df = pd.read_excel(r'C:\Users\kurta\OneDrive\Desktop\datos.xlsx', header=None)

# Asignar los nombres correctos a las columnas
df.columns = ['Tiempo', 'Temperatura']  # Ajusta estos nombres según corresponda

# Verificar que los nombres se asignaron correctamente
print("Columnas en el DataFrame:", df.columns)

# Convertir la columna Tiempo a números enteros o flotantes si es necesario
df['Tiempo'] = pd.to_numeric(df['Tiempo'], errors='coerce')  # 'coerce' convierte no numéricos a NaN

# Eliminar filas con valores NaN si los hubiera
df = df.dropna()

# Extraer los datos de las columnas como arreglos de NumPy
x = df['Tiempo'].values.astype(float)  # Convertir a float explícitamente
y = df['Temperatura'].values.astype(float)  # Convertir a float explícitamente

# Generar los datos para el ajuste exponencial
exp_fit = np.polyfit(x, np.log(y), 1)
y_fit = np.exp(exp_fit[1]) * np.exp(exp_fit[0] * x)

# Crear la gráfica
plt.figure(figsize=(10, 6))
plt.scatter(x, y, label='Datos')
plt.plot(x, y_fit, color='red', label='Ajuste Exponencial')
plt.xlabel('Tiempo')
plt.ylabel('Temperatura')
plt.title('Gráfica Exponencial')
plt.legend()
plt.grid(True)
plt.show()

#%%
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Leer la tabla de datos desde el archivo Excel sin asumir los nombres de las columnas
df = pd.read_excel(r'C:\Users\kurta\OneDrive\Desktop\datos.xlsx', header=None)

# Asignar los nombres correctos a las columnas
df.columns = ['Tiempo', 'Temperatura']  # Ajusta estos nombres según corresponda

# Convertir la columna Tiempo a números enteros o flotantes si es necesario
df['Tiempo'] = pd.to_numeric(df['Tiempo'], errors='coerce')  # 'coerce' convierte no numéricos a NaN

# Eliminar filas con valores NaN si los hubiera
df = df.dropna()

# Extraer los datos de las columnas como arreglos de NumPy
x = df['Tiempo'].values.astype(float)  # Convertir a float explícitamente
y = df['Temperatura'].values.astype(float)  # Convertir a float explícitamente

# Generar los datos para el ajuste exponencial
exp_fit = np.polyfit(x, np.log(y), 1)
y_fit = np.exp(exp_fit[1]) * np.exp(exp_fit[0] * x)

# Crear la gráfica
plt.figure(figsize=(10, 6))
plt.scatter(x, y, label='Datos')
plt.plot(x, y_fit, color='red', label='Ajuste Exponencial')
plt.xlabel('Tiempo')
plt.ylabel('Temperatura')
plt.title('Gráfica Exponencial')
plt.legend()
plt.grid(True)
plt.show()

# Obtener la función exponencial
a = exp_fit[1]
b = exp_fit[0]
print(f'La función exponencial ajustada es: y = exp({a:.4f}) * exp({b:.4f} * x)')


#%%
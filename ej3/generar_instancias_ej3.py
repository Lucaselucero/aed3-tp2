import random

def generar_instancia(cantidad_casos):
    instancia = str(cantidad_casos) + '\n'

    for _ in range(cantidad_casos):
        N = random.randint(1, 10000)
        R = random.randint(1, 10000)
        W = random.randint(1, N-1)
        U = random.randint(1, 10)
        V = random.randint(U, 10)

        instancia += f"{N} {R} {W} {U} {V}\n"

        puntos = set()
        for _ in range(N):
            while True:
                x = random.randint(0, 100000)
                y = random.randint(0, 100000)
                if (x, y) not in puntos:
                    puntos.add((x, y))
                    instancia += f"{x} {y}\n"
                    break

    return instancia

# Generar una cantidad arbitraria de casos de ejemplo (por ejemplo, 5 casos)
cantidad_casos_ejemplo = 100
instancia_ejemplo = generar_instancia(cantidad_casos_ejemplo)

# Guardar la instancia en un archivo
with open("instancias.txt", "w") as archivo:
    archivo.write(instancia_ejemplo)

print("La instancia se ha guardado en el archivo 'instancias.txt'.")
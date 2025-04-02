with open("test_lineal.txt", "w") as archivo:
    for n in range(1, 7000):
        x = 2**(n-1)
        tupla = (x, x)
        archivo.write(f"{tupla[0]} {tupla[1]}\n")

print("Las tuplas se han guardado en el archivo 'test_lineal.txt'.")
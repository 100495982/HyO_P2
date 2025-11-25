import constraint

p = constraint.Problem()

tablero_inicial = [
    [None, 1, None, None, None, 1],
    [None, None, 1, None, None, 0],
    [None, 0, None, None, 1, None],
    [0, None, 1, None, None, None],
    [None, 1, None, 0, None, None],
    [None, None, None, None, None, None]
]
n = len(tablero_inicial)

for i in range(n):
    for j in range(n):
        nombre = f"{i}-{j}"  # El nombre será "0-0", "0-1", etc.
        valor_fijo = tablero_inicial[i][j]

        if valor_fijo is None:
            # Si está vacía, el dominio es [0, 1]
            p.addVariable(nombre, [0, 1])
        else:
            # Si ya hay una pista, el dominio es fijo [valor]
            p.addVariable(nombre, [valor_fijo])


def noTresConsecutivos(v1, v2, v3):
    """Impide que haya más de 2 fichas iguales seguidas"""
    suma = v1 + v2 + v3
    return suma != 0 and suma != 3

def mismaCantidadColores(*args):
    """Obliga a tener la misma cantidad de blancos y negros"""
    longitud = len(args)
    return sum(args) == longitud // 2


for i in range(n):
    for j in range(n):
        # Horizontal: Si caben 3 seguidos hacia la derecha
        if j + 2 < n:
            vars_triplet = [f"{i}-{j}", f"{i}-{j + 1}", f"{i}-{j + 2}"]
            p.addConstraint(noTresConsecutivos, vars_triplet)

        # Vertical: Si caben 3 seguidos hacia abajo
        if i + 2 < n:
            vars_triplet = [f"{i}-{j}", f"{i + 1}-{j}", f"{i + 2}-{j}"]
            p.addConstraint(noTresConsecutivos, vars_triplet)

for i in range(n):
    # Recopilamos toda la Fila i
    vars_fila = [f"{i}-{c}" for c in range(n)]
    p.addConstraint(mismaCantidadColores, vars_fila)

    # Recopilamos toda la Columna i
    vars_columna = [f"{r}-{i}" for r in range(n)]
    p.addConstraint(mismaCantidadColores, vars_columna)

print("Buscando soluciones...")
soluciones = p.getSolutions()
n_soluciones = 0

for sol in soluciones:
    n_soluciones += 1
print(n_soluciones, "soluciones encontradas")
# Imprimimos la solución en el formato deseado
for i in range(n):
    fila = ""
    for j in range(n):
        valor = sol[f"{i}-{j}"]

        # Si es 1 ponemos "X", si no ponemos "O"
        if valor == 1:
            caracter = "X"
        else:
            caracter = "O"

        fila += caracter + " "
    print(fila)
print("-" * 2 * n)

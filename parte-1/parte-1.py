#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import constraint


def parse_tablero_input(archivo_entrada: str) -> list:
    """
    Lee el tablero de input del archivo de entrada, y lo parsea
    para poder revisar que sea de longitud par y que sea cuadrado (nxn).
    Retorna tablero_inicial como una matriz compuesta de:
        None ('.'), 1 ('X'), 0 ('O')
    para poder resolver el problema CSP.
    """
    lineas = []
    with open(archivo_entrada, "r", encoding="utf-8") as f:
        for raw in f:
            fila = raw.rstrip("\n")
            if fila == "":
                continue
            lineas.append(fila)

    if not lineas:
        raise ValueError("el fichero de entrada esta vacío")

    n = len(lineas)
    for r, fila in enumerate(lineas):
        if len(fila) != n:
            raise ValueError(f"Instancia no cuadrada: fila {r} tiene longitud {len(fila)} y se esperaban {n}")
        for c in fila:
            if c not in (".", "X", "O"):
                raise ValueError(f"Caracter invalido en entrada: {c!r}. Solo se permiten '.', 'X', 'O'")

    if n % 2 != 0:
        raise ValueError(f"El tamaño n={n} es impar se requiere n par.")

    tablero = []  # instanciacion del tablero, lista
    for fila in lineas:
        tablero_fila = []
        for c in fila:
            if c == ".":
                tablero_fila.append(None)
            elif c == "X":
                tablero_fila.append(1)
            else:  # 'O'
                tablero_fila.append(0)
        tablero.append(tablero_fila)

    return tablero


# RESTRICCION 1: NO PUEDEN HABER 3 CARACTERES DEL MISMO TIPO CONSECUTIVAMENTE
def no_tres_consecutivos(v1, v2, v3) -> bool:
    """
    Asegura que no hay 000 ni 111 utilizando la suma de 3 caracteres consecutivos.
    Retorna True si se cumple, False si no
    """
    s = v1 + v2 + v3
    return s != 0 and s != 3


# RESTRICCION 2: TIENE QUE HABER EXACTAMENTE N/2 DE CADA TIPO DE CARACTER (0/1)
def misma_cantidad_colores(*args):
    """
    ASegura que hay n/2 caracteres de cada tipo
    """
    n = len(args)
    return sum(args) == n // 2


def genera_CSP(tablero_inicial):

    p = constraint.Problem()  # creamos la instancia de problema CSP de la libreria constraint
    n = len(tablero_inicial)  # ya validado como tablero cuadrado pillamos el valor de n

    # variables del problemas CSP: X
    for i in range(n):
        for j in range(n):
            name = f"{i}-{j}"
            fixed = tablero_inicial[i][j]
            if fixed is None:
                p.addVariable(name, [0, 1])
            else:
                p.addVariable(name, [fixed])

    # Enforzamos la restriccion 1: no 3 caracteres del mismo tipo consecutivamente
    # tanto como en fila que en columna.
    for i in range(n):
        for j in range(n):
            if j + 2 < n:  # se aplica la restriccion en columna j
                trip = [f"{i}-{j}", f"{i}-{j+1}", f"{i}-{j+2}"]
                p.addConstraint(no_tres_consecutivos, trip)
            if i + 2 < n:  # se aplica la restriccion en fila i
                trip = [f"{i}-{j}", f"{i+1}-{j}", f"{i+2}-{j}"]
                p.addConstraint(no_tres_consecutivos, trip)

    # Enforzamos la restriccion 2: hay n/2 caracteres de cada tipo tanto en fila que
    # en columna.
    for i in range(n):
        row_vars = [f"{i}-{c}" for c in range(n)]
        col_vars = [f"{r}-{i}" for r in range(n)]
        p.addConstraint(misma_cantidad_colores, row_vars)
        p.addConstraint(misma_cantidad_colores, col_vars)

    return p


def tablero_to_symbols(tablero_inicial):
    """
    Funcion para imprimir el trablero incial de input en la pantalla mapeando,
    los None como " ", 1 como "X", y 0 como "O".
    """
    nuevo_tablero = []
    for fila in tablero_inicial:
        nueva_fila = []
        for v in fila:
            if v is None:
                nueva_fila.append(" ")
            elif v == 1:
                nueva_fila.append("X")
            else:
                nueva_fila.append("O")
        nuevo_tablero.append(nueva_fila)
    return nuevo_tablero


def formatear_solucion(solucion, n):
    """
    Mapea todas las soluciones encontradas pasando los 1s a X y los 0s a O.
    """
    tablero = []
    for i in range(n):
        fila = []
        for j in range(n):
            v = solucion[f"{i}-{j}"]
            fila.append("X" if v == 1 else "O")
        tablero.append(fila)
    return tablero


def imprime_tablero(tablero, fuente_salida):
    """
    Imprime el tablero en este formato:
    +---+---+...+
    | X |   | O |
    +---+---+...+
    """
    n = len(tablero)
    borde_tablero = "+" + "+".join(["---"] * n) + "+"
    print(borde_tablero, file=fuente_salida)
    for fila in tablero:
        linea = "|" + "|".join(f" {c} " for c in fila) + "|"
        print(linea, file=fuente_salida)
    print(borde_tablero, file=fuente_salida)


def main():
    if len(sys.argv) != 3:
        print("Uso: ./parte-1.py <fichero-entrada> <fichero-salida>")
        sys.exit(1)  # el programa se para si no hay exactamente 3 argumentos

    # pilla los archivos de entrada y salida de los argumentos
    archivo_entrada = sys.argv[1]
    archivo_salida = sys.argv[2]

    # lee el archivo de entrada y parsea el tablero del archivo como tablero inicial
    tablero_inicial = parse_tablero_input(archivo_entrada)
    n = len(tablero_inicial)  # tamaño de tablero ya despues de ser validado como cuadrado y par

    problema_CSP = genera_CSP(tablero_inicial)  # se crea la instancia de probelma CSP
    solutions = problema_CSP.getSolutions()  # guarda las soluciones encontradas a problema
    numero_soluciones = len(solutions)  # pilla el numero de soluciones encontradas

    imprime_tablero(tablero_to_symbols(tablero_inicial), sys.stdout)  # imprime el tablero de input
    print(f"{numero_soluciones} soluciones encontradas")  # imprime la cantidad de soluciones encontradas

    # se escribe al archivo de salida el tablero inicial y UNA de las soluciones encontradas
    with open(archivo_salida, "w", encoding="utf-8") as f:
        imprime_tablero(tablero_to_symbols(tablero_inicial), f)
        if numero_soluciones > 0:
            sol = solutions[0]  # pilla la primera solucion encontrada
            imprime_tablero(formatear_solucion(sol, n), f)
        else:
            print("No se encontraron soluciones.", file=f)


if __name__ == "__main__":
    main()

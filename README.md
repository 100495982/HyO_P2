# HyO_P2

# Parte 1 (BINAIRO)
**Ejecucion**: ./parte-1.py <fichero-entrada> <fichero-salida> 
(2 argumentos obligatorios: ficheros entrada y salida dentro del mismo directorio que el script 
parte-1.py)

**Entrada**: fichero con n lineas de n caracteres, solo . (desconocido), X (negro), O 
(blanco).

**Salida por pantalla**: imprime el tablero inicial y despues cuantas soluciones 
han sido encontradas.

**Fichero de salida**: escribe primero la instancia inicial y a continuacion una solucion 
valida (ambas delimitadas en tabla).

# Parte 2 — Camino más corto (DIMACS)
## NOTA: PARA ELEGIR EL ALGORITMO A UTILIZAR, DESCOMENTA SU RESPECTIVA LLAMADA/LINEA EN main.cpp ENTRE LAS LINEAS 38-41.

Esta carpeta contiene la solución de la **Parte 2**: encontrar el camino más corto entre dos vértices en un mapa DIMACS (`.gr` + `.co`) ejecutando `parte-2.py`, que a su vez lanza el ejecutable C++ `./parte2`. [file:1]

## Requisitos
- Python 3.8+ (recomendado 3.10+).
- CMake 3.16+ y un compilador C++ con soporte C++23 (por ejemplo AppleClang/Clang).
- Ficheros del mapa: `USA-road-d.<MAP>.gr` y `USA-road-d.<MAP>.co` en esta misma carpeta (mismo directorio que `parte-2.py`). [file:1]

## Estructura esperada
- `parte-2.py` (script principal). [file:1]
- `parte2` (ejecutable C++ que resuelve el problema y escribe la ruta en el fichero de salida). [file:1]
- `USA-road-d.<MAP>.gr` (arcos) y `USA-road-d.<MAP>.co` (coordenadas). [file:1]

## Compilación (C++)
Desde el directorio `parte-2`:
cmake -S . -B build
cmake --build build -j
cp ./build/parte2 ./parte2

Esto genera el binario en `build/parte2`.

### Hacer visible `./parte2` para el script
El script ejecuta `./parte2`, así que hay que copiar o enlazar el binario al directorio actual:
cp ./build/parte2 ./parte2

### Permisos de ejecución (Linux/macOS)
chmod +x ./parte2
chmod +x ./parte-2.py


## Ejecución (script evaluable)
Formato exigido:
./parte-2.py <vertice-1> <vertice-2> <nombre-del-mapa (sin el '.gr/.co')> <fichero-salida>
### Ejemplo (BAY)
./parte-2.py 1 309 USA-road-d.BAY solucion.txt


## Salida por pantalla
El script imprime (en este orden) la información requerida:
- Número de vértices procesados del fichero `.co`. [file:1]
- Número de arcos procesados del fichero `.gr`. [file:1]
- Coste de la solución óptima encontrada. [file:1]
- Número de nodos expandidos y rendimiento `nodes/sec` (calculado como `expansiones / tiempo`). [file:1]
- Tiempo de ejecución dedicado a resolver la instancia. [file:1]

## Fichero de salida
El fichero indicado como cuarto argumento contiene la ruta en el formato:

`inicio - coste - ... - final` (alternando vértices y costes de arco). [file:1]

Ejemplo:
1 - (1498) - 308 - (8718) - 309






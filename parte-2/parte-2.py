#!/usr/bin/env python3
import sys
import subprocess
import os

def main():
    if len(sys.argv) != 5:
        print("Uso: python parte-2.py V1 V2 NOMBRE_MAPA SALIDA")
        print("Ejemplo: python parte-2.py 1 309 NY solucion.txt")
        sys.exit(1)
    
    v1, v2, mapa, salida = sys.argv[1:5]
    
    # Windows: buscar parte2.exe también
    exe = "./parte2.exe" if os.name == 'nt' else "./parte2"
    
    if not os.path.exists(exe):
        print("ERROR: Compila primero con cmake/make o g++")
        sys.exit(1)
    
    cmd = [exe, v1, v2, mapa, salida]
    resultado = subprocess.run(cmd, capture_output=True, text=True)
    
    print(resultado.stdout, end='')
    if resultado.returncode != 0:
        print("ERROR:", resultado.stderr)

if __name__ == "__main__":
    main()

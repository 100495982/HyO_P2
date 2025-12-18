#!/usr/bin/env python3
# parte-2.py
#
# Uso:
#   ./parte-2.py START GOAL MAP_BASE_PATH OUT_FILE
#
# MAP_BASE_PATH identifica MAP_BASE_PATH.gr y MAP_BASE_PATH.co. [file:1]

import subprocess
import sys
from pathlib import Path


def usage(code: int = 1) -> None:
    print("Uso: ./parte-2.py VERTICE1 VERTICE2 NOMBRE_DEL_MAPA FICHERO_SALIDA", file=sys.stderr)
    print("Ejemplo: ./parte-2.py 1 309 USA-road-d.BAY solucion.txt", file=sys.stderr)
    sys.exit(code)


def main() -> int:
    if len(sys.argv) != 5:
        usage(1)

    start = sys.argv[1]
    goal = sys.argv[2]
    map_base = Path(sys.argv[3])
    out_file = Path(sys.argv[4])

    gr_path = Path(str(map_base) + ".gr")
    co_path = Path(str(map_base) + ".co")

    if not gr_path.exists():
        print(f"Error: no existe {gr_path}", file=sys.stderr)
        return 2
    if not co_path.exists():
        print(f"Error: no existe {co_path}", file=sys.stderr)
        return 2

    # Must be executable in current working directory
    cpp_bin = (Path(__file__).resolve().parent / "parte2")
    if not cpp_bin.exists():
        print("Error: no se encuentra el ejecutable ./parte2", file=sys.stderr)
        return 3

    cmd = [str(cpp_bin), str(start), str(goal), str(gr_path), str(co_path), str(out_file)]
    p = subprocess.run(cmd, capture_output=True, text=True)

    if p.returncode != 0:
        print("Error ejecutando ./parte2", file=sys.stderr)
        if p.stderr.strip():
            print(p.stderr, file=sys.stderr)
        return p.returncode

    # C++ must print exactly the required lines (vertices, arcs, cost, expanded, time). [file:1]

    lines = [ln.strip() for ln in p.stdout.splitlines() if ln.strip()]
    if len(lines) < 5:
        print("Error: salida de ./parte2 incompleta", file=sys.stderr)
        sys.stdout.write(p.stdout)
        return 4

    n_vertices = lines[0]
    n_arcos    = lines[1]
    coste      = lines[2]
    exp        = lines[3]
    tiempo     = lines[4]

    print(f"# vertices: {n_vertices}")
    print(f"# arcos   : {n_arcos}")
    print(f"Solución óptima encontrada con coste {coste}")
    print("")
    print(f"Tiempo de ejecución: {float(tiempo):.6f} segundos")
    exp_i = int(exp)
    t = float(tiempo)

    nodes_per_sec = (exp_i / t) if t > 0 else float("inf")

    print(f"# expansiones: {exp_i} ({nodes_per_sec:.2f} nodes/sec)")



    return 0



if __name__ == "__main__":
    raise SystemExit(main())

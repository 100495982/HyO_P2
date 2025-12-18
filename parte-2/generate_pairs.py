#!/usr/bin/env python3
import csv
import math
import random
import sys
from pathlib import Path

# Haversine distance in meters using lon/lat in degrees
def haversine_m(lon1, lat1, lon2, lat2):
    R = 6371000.0
    to_rad = math.pi / 180.0
    lon1 *= to_rad; lat1 *= to_rad; lon2 *= to_rad; lat2 *= to_rad
    dlon = lon2 - lon1
    dlat = lat2 - lat1
    a = math.sin(dlat/2)**2 + math.cos(lat1)*math.cos(lat2)*math.sin(dlon/2)**2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1-a))
    return R * c

def load_vertices_from_co(co_path: Path):
    ids = []
    coords = {}  # id -> (lon_deg, lat_deg)
    with co_path.open("r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            if not line or line[0] != "v":
                continue
            parts = line.split()
            if len(parts) < 4:
                continue
            # v id lon lat  (lon/lat are int scaled by 1e6) [file:1]
            vid = int(parts[1])
            lon_i = int(parts[2])
            lat_i = int(parts[3])
            lon = lon_i / 1_000_000.0
            lat = lat_i / 1_000_000.0
            ids.append(vid)
            coords[vid] = (lon, lat)
    return ids, coords

def main():
    if len(sys.argv) < 3:
        print("Usage: ./generate_pairs.py MAP_BASE_PATH output.csv [--seed 123] [--k 5]")
        print("Example: ./generate_pairs.py USA-road-d.BAY pairs.csv --seed 1 --k 5")
        return 1

    base = Path(sys.argv[1])
    out_csv = Path(sys.argv[2])

    seed = 1
    k = 5  # number of pairs per category
    args = sys.argv[3:]
    if "--seed" in args:
        seed = int(args[args.index("--seed") + 1])
    if "--k" in args:
        k = int(args[args.index("--k") + 1])

    co_path = Path(str(base) + ".co")
    if not co_path.exists():
        print(f"Error: CO file not found: {co_path}")
        return 2

    random.seed(seed)

    ids, coords = load_vertices_from_co(co_path)
    if len(ids) < 2:
        print("Error: not enough vertices parsed from .co")
        return 3

    # Helper: pick a random id that exists
    def pick():
        return random.choice(ids)

    # SHORT pairs: pick u, then choose v from a pool closest to u (by straight-line distance)
    def gen_short():
        u = pick()
        lon_u, lat_u = coords[u]
        # sample candidates and take the closest
        cand = random.sample(ids, min(3000, len(ids)))
        cand = [x for x in cand if x != u]
        best_v = min(cand, key=lambda v: haversine_m(lon_u, lat_u, coords[v][0], coords[v][1]))
        return u, best_v

    # MEDIUM pairs: random pairs
    def gen_medium():
        u = pick()
        v = pick()
        while v == u:
            v = pick()
        return u, v

    # LONG pairs: pick u, then choose v from a pool farthest from u (by straight-line distance)
    def gen_long():
        u = pick()
        lon_u, lat_u = coords[u]
        cand = random.sample(ids, min(5000, len(ids)))
        cand = [x for x in cand if x != u]
        best_v = max(cand, key=lambda v: haversine_m(lon_u, lat_u, coords[v][0], coords[v][1]))
        return u, best_v

    rows = []
    for _ in range(k):
        u, v = gen_short()
        rows.append(("short", u, v))
    for _ in range(k):
        u, v = gen_medium()
        rows.append(("medium", u, v))
    for _ in range(k):
        u, v = gen_long()
        rows.append(("long", u, v))

    with out_csv.open("w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["map_base", "kind", "start", "goal"])
        for kind, u, v in rows:
            w.writerow([str(base), kind, u, v])

    print(f"Wrote {len(rows)} pairs to {out_csv}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())

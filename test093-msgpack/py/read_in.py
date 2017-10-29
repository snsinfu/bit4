import msgpack
import sys

import numpy as np

def main():
    if len(sys.argv) == 2:
        load_from_file(sys.argv[1])
    else:
        load_from_stdin()

def load_from_file(path):
    with open(path, 'rb') as src:
        load(src)

def load_from_stdin():
    load(sys.stdin.detach())

def load(stream):
    unpacker = msgpack.Unpacker(stream)

    metadata = unpacker.unpack()
    name = metadata[b'name'].decode('utf-8')
    num_points = metadata[b'num_points']
    dimension = metadata[b'dimension']
    print('Name:', name)
    print('Shape:', num_points, 'x', dimension)

    num_frames = unpacker.read_array_header()
    mean_squared_displacements = []
    first_points = None
    for _ in range(num_frames):
        points = np.frombuffer(unpacker.unpack(), dtype=np.float32).reshape(num_points, dimension)
        if first_points is None:
            first_points = points
        msd = np.sqrt(np.mean((points - first_points)**2))
        mean_squared_displacements.append(msd)

    time_points = np.arange(num_frames)
    mean_squared_displacements = np.array(mean_squared_displacements)
    correlation = np.corrcoef(time_points, mean_squared_displacements)[0, 1]
    print('MSD-r:', correlation)

if __name__ == '__main__':
    main()

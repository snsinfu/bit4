import io
import sys

import msgpack
import numpy as np

def main():
    if len(sys.argv) == 2:
        dump_to_file(sys.argv[1])
    else:
        dump_to_stdout()

def dump_to_stdout():
    dump(sys.stdout.detach())

def dump_to_file(path):
    with open(path, 'wb') as dest:
        dump(dest)

def dump(stream):
    num_frames = 50
    num_points = 60000
    dimension = 3
    sigma = 0.01

    metadata = {
        'name': 'Lorem ipsum dolor sit amet',
        'num_points': num_points,
        'dimension': dimension
    }

    packer = msgpack.Packer(use_single_float=True)
    stream.write(packer.pack(metadata))
    stream.write(packer.pack_array_header(num_frames))
    for points in generate_frames(num_frames, num_points, dimension, sigma):
        points_data = points.astype(np.float32).tobytes()
        stream.write(packer.pack(points_data))

def generate_frames(num_frames, num_points, dimension, sigma):
    points = np.random.uniform(-1, 1, size=(num_points, dimension))
    for _ in range(num_frames):
        points += np.random.normal(0, sigma, size=points.shape)
        yield points

if __name__ == '__main__':
    main()

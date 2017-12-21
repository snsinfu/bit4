import concurrent.futures
import copy
import os
import sys

import colorspacious as cs
import numpy as np
import scipy.spatial.distance as spd

vision_colorspaces = {
    'norm': 'sRGB1',
    'deut': {'name': 'sRGB1+CVD', 'cvd_type': 'deuteranomaly', 'severity': 100},
    'prot': {'name': 'sRGB1+CVD', 'cvd_type': 'protanomaly', 'severity': 100},
    'trit': {'name': 'sRGB1+CVD', 'cvd_type': 'tritanomaly', 'severity': 100}
}
color_dimension = 3

distance_lower_bounds = {
    'norm': 25,
    'deut': 25,
    'prot': 25,
    'trit': 25
}
palette_size = 16
color_range = 0.05, 0.95

search_sigmas = [0.005, 0.01, 0.01, 0.01, 0.01, 0.01]
replica_temperatures = [0.01, 0.1, 1, 10, 100, 1000]
replica_exchanges = 5000
exchange_chunk = 50
logging_interval = 50

def main():
    np.random.seed(int(os.getenv('SEED') or 1))
    max_workers = int(os.getenv('PARALLEL') or 2)

    cost_model = MarginCost(distance_lower_bounds)
    searchers = []
    for temperature, sigma in zip(replica_temperatures, search_sigmas):
        initial_palette = np.random.uniform(size=(palette_size, color_dimension))
        searcher = PaletteSearcher(initial_palette, cost_model, temperature, sigma)
        searchers.append(searcher)

    for step in range(replica_exchanges):
        if step % logging_interval == 0:
            best_cost = min(searcher.best_cost for searcher in searchers)
            sys.stderr.write('%d\t%g\n' % (step, best_cost))

        if best_cost == 0:
            break

        replica_exchange(searchers, step % 2)

        with concurrent.futures.ProcessPoolExecutor(max_workers) as executor:
            updates = []
            for searcher in searchers:
                update = executor.submit(searcher.multistep, exchange_chunk)
                updates.append(update)
            searchers = [update.result() for update in updates]

    best = min(searchers, key=lambda s: s.best_cost)
    print('# Cost = %g' % best.best_cost)

    for r, g, b in best.best_palette:
        print('%.3f\t%.3f\t%.3f' % (r, g, b))

def replica_exchange(replicas, offset):
    for i in range(offset, len(replicas) - 1, 2):
        j = i + 1
        this = replicas[i]
        that = replicas[j]

        criterion = (this.cost - that.cost) * (1.0 / this.temperature - 1.0 / that.temperature)
        log_uniform = np.log(1 - np.random.uniform())
        if log_uniform < criterion:
            this.temperature, that.temperature = that.temperature, this.temperature

class PaletteSearcher:
    def __init__(self, palette, cost_model, temperature, sigma):
        self._palette = Palette(np.clip(palette, *color_range))
        self._cost_model = cost_model
        self._temperature = temperature
        self._sigma = sigma
        self._cost = self._cost_model.evaluate(self._palette)
        self._best_palette = self._palette
        self._best_cost = self._cost
        self._random = np.random.RandomState(np.random.randint(2**32))

    @property
    def best_cost(self):
        return self._best_cost

    @property
    def best_palette(self):
        return self._best_palette

    @property
    def cost(self):
        return self._cost

    @property
    def temperature(self):
        return self._temperature

    @temperature.setter
    def temperature(self, temperature):
        self._temperature = temperature

    def multistep(self, count):
        for _ in range(count):
            self.step()
        return self

    def step(self):
        proposed_palette = self._palette.copy()

        index = self._random.randint(len(proposed_palette))
        delta = self._random.normal(0, self._sigma, size=color_dimension)
        proposed_palette[index] = np.clip(proposed_palette[index] + delta, *color_range)
        proposed_cost = self._cost_model.evaluate(proposed_palette)

        if self._is_acceptable(proposed_cost):
            self._palette = proposed_palette
            self._cost = proposed_cost

        if proposed_cost < self._best_cost:
            self._best_palette = proposed_palette
            self._best_cost = proposed_cost

        return self

    def _is_acceptable(self, cost):
        neglog_uniform = -np.log(1 - self._random.uniform())
        return cost - self._cost < self._temperature * neglog_uniform

class MarginCost:
    def __init__(self, distance_lower_bounds):
        self._distance_lower_bounds = distance_lower_bounds

    def evaluate(self, palette):
        cost = 0
        for name, lowerbound in self._distance_lower_bounds.items():
            pair_distances = non_diagonal(palette.distance_matrix(name))
            margins = pair_distances - lowerbound
            violated_margins = np.minimum(margins, 0)
            cost += np.sum(violated_margins**2)
        return cost

def non_diagonal(matrix):
    return np.stack([
        np.concatenate((matrix[i, (i + 1):], matrix[i, :i]))
        for i in range(matrix.shape[0])
    ])

class Palette:
    def __init__(self, palette):
        self._palette = np.array(palette)

        self._vision_points = {
            name: ucs(palette, name) for name in vision_colorspaces
        }
        self._distance_matrices = {
            name: spd.squareform(spd.pdist(points))
            for name, points in self._vision_points.items()
        }

    def copy(self):
        return copy.deepcopy(self)

    def distance_matrix(self, name):
        return self._distance_matrices[name]

    def __len__(self):
        return len(self._palette)

    def __iter__(self):
        return iter(self._palette)

    def __getitem__(self, index):
        return self._palette[index]

    def __setitem__(self, index, rgb):
        self._palette[index] = rgb
        self._update_partial(index)

    def _update_partial(self, index):
        self._update_point(index)
        self._update_dists(index)

    def _update_point(self, index):
        for name, points in self._vision_points.items():
            points[index] = ucs(self._palette[index], name)

    def _update_dists(self, index):
        for name, points in self._vision_points.items():
            dists = np.linalg.norm(points[index] - points, axis=1)
            dist_mat = self._distance_matrices[name]
            dist_mat[index, :] = dists
            dist_mat[:, index] = dists

def ucs(color, name):
    return cs.cspace_convert(color, vision_colorspaces[name], 'CAM02-UCS')

if __name__ == '__main__':
    main()

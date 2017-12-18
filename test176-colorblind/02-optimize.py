import logging
import sys

import colorspacious
import numpy as np
import scipy.spatial.distance as spd

logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s - %(message)s',
                    datefmt='%T')
np.random.seed(1)

points = np.random.uniform(0, 1, size=(16, 3))
points = np.array([
    (  0,   0,   0),
    (255, 255, 255), # Added
    (  0,  73,  73),
    (  0, 146, 146),
    (255, 109, 182),
    (255, 182, 219),
    ( 73,   0, 146),
    (  0, 109, 219),
    (182, 109, 255),
    (109, 182, 255),
    (182, 219, 255),
    (146,   0,   0),
    (146,  73,   0),
    (219, 109,   0),
    ( 36, 255,  36),
    (255, 255, 109),
    (255, 255, 255), # Added
]) / 255.0

norm_space = 'sRGB1'
deut_space = {
    'name':     'sRGB1+CVD',
    'cvd_type': 'deuteranomaly',
    'severity': 100,
}
prot_space = {
    'name':     'sRGB1+CVD',
    'cvd_type': 'protanomaly',
    'severity': 100,
}
trit_space = {
    'name':     'sRGB1+CVD',
    'cvd_type': 'tritanomaly',
    'severity': 100,
}

norm_min_dist = 30
deut_min_dist = 30
prot_min_dist = 30
trit_min_dist = 30

def eval_cost(points):
    norm_ucs = colorspacious.cspace_convert(points, norm_space, 'CAM02-UCS')
    deut_ucs = colorspacious.cspace_convert(points, deut_space, 'CAM02-UCS')
    prot_ucs = colorspacious.cspace_convert(points, prot_space, 'CAM02-UCS')
    trit_ucs = colorspacious.cspace_convert(points, trit_space, 'CAM02-UCS')
    dist = np.concatenate([
        spd.pdist(norm_ucs) - norm_min_dist,
        spd.pdist(deut_ucs) - deut_min_dist,
        spd.pdist(prot_ucs) - prot_min_dist,
        spd.pdist(trit_ucs) - trit_min_dist,
    ])
    cost_collision = np.minimum(0, dist)**2
    cost_contrast = -np.log(1 + np.maximum(0, dist))
    return np.sum(cost_collision + cost_contrast)

prev_cost = eval_cost(points)
prev_points = points.copy()
max_steps = 100000
log_interval = 1000
sigma = 0.03

for step in range(max_steps):
    index = np.random.randint(2, points.shape[0])
    points[index] += np.random.normal(0, sigma, size=points.shape[1])
    points[index] = np.clip(points[index], 0, 1)
    cost = eval_cost(points)

    fract_step = float(step) / max_steps
    temperature = 10.0
    temperature *= 1 / (1 + 1000 * fract_step**2)

    if cost - prev_cost < -temperature * np.log(1 - np.random.uniform()):
        prev_cost = cost
        prev_points = points.copy()
    else:
        points = prev_points.copy()

    if step % log_interval == 0:
        logging.info('%.0f%%\t%g\t%g' % (fract_step * 100, prev_cost, temperature))

for point in points:
    print('%.2f\t%.2f\t%.2f' % (point[0], point[1], point[2]))

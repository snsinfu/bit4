import logging
import os
import signal

import colorspacious
import numpy as np
import scipy.spatial.distance as spd

signal.signal(signal.SIGINT, signal.SIG_DFL)
signal.signal(signal.SIGPIPE, signal.SIG_DFL)

logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s - %(message)s',
                    datefmt='%T')
np.random.seed(int(os.getenv('SEED') or '1'))

points = np.vstack([
    [(1, 1, 1), (0, 0, 0)],
    np.random.uniform(0, 1, size=(15, 3)),
])

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

max_steps = 100000
log_interval = 1000
sigma = 0.03
min_rgb = 0.2

points = np.clip(points, min_rgb, 1)
prev_cost = eval_cost(points)
prev_points = points.copy()

for step in range(max_steps):
    index = np.random.randint(2, points.shape[0])
    points[index] += np.random.normal(0, sigma, size=points.shape[1])
    points[index] = np.clip(points[index], min_rgb, 1)
    cost = eval_cost(points)

    fract_step = float(step) / max_steps
    temperature = 100.0
    temperature *= 1 / (1 + 1000 * fract_step**2)

    if cost - prev_cost < -temperature * np.log(1 - np.random.uniform()):
        prev_cost = cost
        prev_points = points.copy()
    else:
        points = prev_points.copy()

    if step % log_interval == 0:
        logging.info('%.0f%%\t%g\t%g' % (fract_step * 100, prev_cost, temperature))

for point in points[1:]:
    print('%.3f\t%.3f\t%.3f' % (point[0], point[1], point[2]))

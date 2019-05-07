import unittest

import numpy as np
import tensorflow.keras.backend as K

from .layers import LayerNormalization


class TestLayerNormalization(unittest.TestCase):
    def test_actual_output_shape(self):
        norm = LayerNormalization()
        x = K.zeros((2, 3, 5, 7, 11))
        y = norm(x)
        self.assertEqual(K.int_shape(y), K.int_shape(x))

    def test_output_statistics(self):
        norm = LayerNormalization()
        x = np.array([[1, 2, 3], [1, 3, 5], [1, 5, 9]])
        y = K.eval(norm(K.constant(x)))
        self.assertAlmostEqual(y[0].mean(), 0, places=4)
        self.assertAlmostEqual(y[1].mean(), 0, places=4)
        self.assertAlmostEqual(y[2].mean(), 0, places=4)
        self.assertAlmostEqual(y[0].std(), 1, places=4)
        self.assertAlmostEqual(y[1].std(), 1, places=4)
        self.assertAlmostEqual(y[2].std(), 1, places=4)

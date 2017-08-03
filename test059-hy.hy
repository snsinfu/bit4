(import numpy)
(import [sklearn.linear_model    [LinearRegression]]
        [sklearn.model_selection [train_test_split]])

(setv model (LinearRegression))
(setv X (numpy.random.uniform 0.0 1.0 (, 100 5)))
(setv y (+ (* 1.5 (** X 1.9))
           (* 1.9 (** X 0.2))
           0.5))
(setv (, train_X test_X train_y test_y) (train_test_split X y))

(model.fit train_X train_y)

(print "R^2" (.score model test_X test_y))
(print (numpy.round model.coef_ 1))

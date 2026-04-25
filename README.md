# Spec-driven-development-

#Add graidant descent
SPEC: fit_gradient_descent()
-------------------------------
PURPOSE : Find slope and intercept by iteratively
          improving them using gradient descent

INPUTS  :
  - learning_rate (double) --> how big each step is
  - epochs (int)           --> how many iterations

PROCESS :
  1. Start slope = 0, intercept = 0
  2. For each epoch:
     a. compute predictions
     b. compute error = prediction - actual
     c. compute gradients
     d. update slope and intercept
  3. Print loss every 100 epochs

FORMULA :
  grad_slope     = (2/n) * sum(error * X)
  grad_intercept = (2/n) * sum(error)
  slope          = slope - lr * grad_slope
  intercept      = intercept - lr * grad_intercept

OUTPUT  :
  - updated slope and intercept
  - printed loss every 100 epochs
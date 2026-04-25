#include <iostream>
#include <vector>
#include <cmath>

class LinearRegression {
private:
    std::vector<double> X;
    std::vector<double> y;
    double slope;
    double intercept;

public:

    // constructor -- start with slope=0, intercept=0
    LinearRegression() {
        slope     = 0.0;
        intercept = 0.0;
    }

    // load data manually for now
    void load_data(std::vector<double>& x_in,
                   std::vector<double>& y_in) {
        X = x_in;
        y = y_in;
    }

    // gradient descent fit
    void fit_gradient_descent(double learning_rate, int epochs) {
        int n = X.size();

        for (int epoch = 0; epoch < epochs; epoch++) {

            // STEP 1 -- compute predictions and errors
            double grad_slope     = 0.0;
            double grad_intercept = 0.0;
            double loss           = 0.0;

            for (int i = 0; i < n; i++) {
                double prediction = slope * X[i] + intercept;
                double error      = prediction - y[i];

                // STEP 2 -- accumulate gradients
                grad_slope     += (2.0 / n) * error * X[i];
                grad_intercept += (2.0 / n) * error;

                // accumulate loss (MSE)
                loss += (error * error) / n;
            }

            // STEP 3 -- update slope and intercept
            slope     -= learning_rate * grad_slope;
            intercept -= learning_rate * grad_intercept;

            // print loss every 100 epochs
            if (epoch % 100 == 0) {
                std::cout << "Epoch " << epoch
                          << "  Loss: " << loss
                          << "  Slope: " << slope
                          << "  Intercept: " << intercept << "\n";
            }
        }
    }

    double predict(double x) {
        return slope * x + intercept;
    }
};

int main() {
    // same house data as before
    std::vector<double> X = {50, 80, 100, 120, 150};
    std::vector<double> y = {200000, 320000, 400000, 480000, 600000};

    LinearRegression model;
    model.load_data(X, y);
    model.fit_gradient_descent(0.000001, 1000);

    std::cout << "\nPrediction for x=120 : " << model.predict(120.0) << "\n";

    return 0;
}
#include <iostream>

// define the struct
struct LinearModel {
    double slope;
    double intercept;
};

// function that returns BOTH values at once
LinearModel compute_model(double mean_x, double mean_y, double var_x, double cov_xy) {
    LinearModel model;
    model.slope     = cov_xy / var_x;
    model.intercept = mean_y - model.slope * mean_x;
    return model;
}

int main() {
    // dummy values for now to test the struct
    double mean_x = 100.0;
    double mean_y = 400000.0;
    double var_x  = 1000.0;
    double cov_xy = 4000000.0;

    LinearModel result = compute_model(mean_x, mean_y, var_x, cov_xy);

    std::cout << "Slope     : " << result.slope     << "\n";
    std::cout << "Intercept : " << result.intercept << "\n";

    return 0;
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

class LinearRegression {

private:
    std::vector<double> X;      // feature values
    std::vector<double> y;      // target values
    double slope;               // from Lesson 3
    double intercept;           // from Lesson 3

public:

    // STEP 1 - load data (Lesson 1)
    void load_data(const std::string& filename) {
        
    }

    // STEP 2 - fit the model (Lesson 2 + 3)
    void fit() {
        
    }

    // STEP 3 - predict (Lesson 4)
    double predict(double x) {
        
        return 0.0;
    }

    // STEP 4 - evaluate with MSE
    void evaluate() {
        
    }

    // STEP 5 - print model
    void print() {
        
    }
};

int main() {
    LinearRegression model;
    model.load_data("house_data.csv");
    model.fit();
    model.print();
    model.evaluate();
    std::cout << "Prediction for x=120 : " << model.predict(120.0) << "\n";
    return 0;
}
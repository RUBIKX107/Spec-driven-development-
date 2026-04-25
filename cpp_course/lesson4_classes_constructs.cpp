#include <iostream>

class LinearRegression {
// private: only accessible inside the class
private:
    double slope;
    double intercept;

// public: accessible from outside the class
public:

    // constructor -- called when object is created
    // same idea as Python's __init__
    LinearRegression(double s, double i) {
        slope     = s;
        intercept = i;
    }

    // predict method -- same as Python's self.predict(x)
    double predict(double x) {
        return slope * x + intercept;
    }

    // print method
    void print() {
        std::cout << "Slope     : " << slope     << "\n";
        std::cout << "Intercept : " << intercept << "\n";
    }
};

int main() {
    // create the object -- constructor is called here
    LinearRegression model(4000.0, 0.0);

    // use it
    model.print();
    std::cout << "Prediction for x=120 : " << model.predict(120.0) << "\n";
    std::cout << "Prediction for x=150 : " << model.predict(150.0) << "\n";

    return 0;
}
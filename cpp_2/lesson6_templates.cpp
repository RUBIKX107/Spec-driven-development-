#include <iostream>
#include <vector>

// TEMPLATE FUNCTION
// T is a placeholder -- becomes whatever type you call it with
template <typename T>
T compute_mean(std::vector<T>& v) {
    T sum = 0;
    for (int i = 0; i < (int)v.size(); i++) {
        sum += v[i];
    }
    return sum / v.size();
}

// TEMPLATE CLASS
// works for any numeric type
template <typename T>
class Stats {
private:
    std::vector<T> data;

public:
    Stats(std::vector<T>& d) {
        data = d;
    }

    T mean() {
        T sum = 0;
        for (int i = 0; i < (int)data.size(); i++) {
            sum += data[i];
        }
        return sum / data.size();
    }

    T max_val() {
        T max = data[0];
        for (int i = 1; i < (int)data.size(); i++) {
            if (data[i] > max) max = data[i];
        }
        return max;
    }

    T min_val() {
        T min = data[0];
        for (int i = 1; i < (int)data.size(); i++) {
            if (data[i] < min) min = data[i];
        }
        return min;
    }
};

int main() {

    // PART 1 -- template function with different types
    std::vector<double> doubles = {1.5, 2.5, 3.5, 4.5};
    std::vector<int>    ints    = {10, 20, 30, 40};

    std::cout << "Mean doubles : " << compute_mean(doubles) << "\n";
    std::cout << "Mean ints    : " << compute_mean(ints)    << "\n";

    // PART 2 -- template class
    Stats<double> stats_d(doubles);
    std::cout << "\nDouble stats:\n";
    std::cout << "  Mean : " << stats_d.mean()    << "\n";
    std::cout << "  Max  : " << stats_d.max_val() << "\n";
    std::cout << "  Min  : " << stats_d.min_val() << "\n";

    Stats<int> stats_i(ints);
    std::cout << "\nInt stats:\n";
    std::cout << "  Mean : " << stats_i.mean()    << "\n";
    std::cout << "  Max  : " << stats_i.max_val() << "\n";
    std::cout << "  Min  : " << stats_i.min_val() << "\n";

    return 0;
}
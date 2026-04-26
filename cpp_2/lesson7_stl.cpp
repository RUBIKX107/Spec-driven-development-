#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <string>

int main() {

    // ── PART 1: std::map (like Python dict) ──────────────
    std::cout << "=== MAP ===\n";

    std::map<std::string, double> scores;
    scores["alice"] = 95.0;
    scores["bob"]   = 87.0;
    scores["carol"] = 92.0;

    // access like Python dict
    std::cout << "Alice score : " << scores["alice"] << "\n";

    // loop through map -- like Python dict.items()
    for (auto& pair : scores) {
        std::cout << pair.first << " : " << pair.second << "\n";
    }

    // check if key exists -- like Python "in" operator
    if (scores.count("bob") > 0) {
        std::cout << "Bob exists!\n";
    }

    // ── PART 2: std::set (like Python set) ───────────────
    std::cout << "\n=== SET ===\n";

    std::set<int> unique_labels;
    unique_labels.insert(1);
    unique_labels.insert(2);
    unique_labels.insert(1);  // duplicate -- ignored!
    unique_labels.insert(3);

    std::cout << "Unique labels: ";
    for (auto& val : unique_labels) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    // ── PART 3: STL algorithms ───────────────────────────
    std::cout << "\n=== ALGORITHMS ===\n";

    std::vector<double> data = {3.5, 1.2, 4.8, 2.1, 5.0, 0.9};

    // sort -- like Python sorted()
    std::sort(data.begin(), data.end());
    std::cout << "Sorted: ";
    for (auto& val : data) std::cout << val << " ";
    std::cout << "\n";

    // max and min -- like Python max() min()
    auto max_it = std::max_element(data.begin(), data.end());
    auto min_it = std::min_element(data.begin(), data.end());
    std::cout << "Max : " << *max_it << "\n";
    std::cout << "Min : " << *min_it << "\n";

    // sum -- like Python sum()
    double total = std::accumulate(data.begin(), data.end(), 0.0);
    std::cout << "Sum : " << total << "\n";

    // find -- like Python "in" operator
    auto it = std::find(data.begin(), data.end(), 4.8);
    if (it != data.end()) {
        std::cout << "Found 4.8 in data!\n";
    }

    return 0;
}
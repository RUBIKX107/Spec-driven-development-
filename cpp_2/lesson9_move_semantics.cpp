#include <iostream>
#include <vector>
#include <string>
#include <utility>  // for std::move

// helper function to print a vector
void print_vector(const std::string& name,
                  const std::vector<double>& v) {
    std::cout << name << " (size=" << v.size() << "): ";
    for (int i = 0; i < (int)v.size(); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";
}

// ── class with copy and move support ─────────────────────
class Dataset {
private:
    std::vector<double> data;
    std::string name;

public:
    // regular constructor
    Dataset(std::string n, std::vector<double> d) {
        name = n;
        data = d;
        std::cout << "Constructed : " << name << "\n";
    }

    // copy constructor -- makes a full duplicate
    Dataset(const Dataset& other) {
        name = other.name + "_copy";
        data = other.data;
        std::cout << "Copied : " << name << "\n";
    }

    // move constructor -- transfers ownership
    Dataset(Dataset&& other) {
        name = other.name + "_moved";
        data = std::move(other.data);  // transfer data
        other.name = "empty";          // original is now empty
        std::cout << "Moved : " << name << "\n";
    }

    void print() {
        std::cout << "Dataset " << name
                  << " size=" << data.size() << "\n";
    }
};

int main() {

    // ── PART 1: copy vs move with vectors ────────────────
    std::cout << "=== COPY vs MOVE ===\n";

    std::vector<double> original = {1.0, 2.0, 3.0, 4.0, 5.0};

    // COPY -- original still has data
    std::vector<double> copied = original;
    print_vector("original", original);
    print_vector("copied  ", copied);

    std::cout << "\n";

    // MOVE -- original is now empty!
    std::vector<double> moved = std::move(original);
    print_vector("original after move", original);
    print_vector("moved              ", moved);

    // ── PART 2: copy vs move with class ──────────────────
    std::cout << "\n=== CLASS COPY vs MOVE ===\n";

    std::vector<double> d = {10.0, 20.0, 30.0};

    // construct
    Dataset ds1("training_data", d);
    ds1.print();

    std::cout << "\n";

    // copy -- ds1 still exists
    Dataset ds2 = ds1;
    ds1.print();
    ds2.print();

    std::cout << "\n";

    // move -- ds1 is now empty!
    Dataset ds3 = std::move(ds1);
    ds1.print();   // empty now
    ds3.print();   // has the data

    // ── PART 3: why move matters for ML ──────────────────
    std::cout << "\n=== WHY THIS MATTERS ===\n";

    // imagine 1 million floats
    std::vector<double> big_dataset(1000000, 1.0);
    std::cout << "big_dataset size : " << big_dataset.size() << "\n";

    // COPY -- duplicates 1 million values in memory
    // std::vector<double> copy_ds = big_dataset;  // slow!

    // MOVE -- just transfers ownership, no duplication
    std::vector<double> moved_ds = std::move(big_dataset);
    std::cout << "big_dataset after move : " << big_dataset.size() << "\n";
    std::cout << "moved_ds size          : " << moved_ds.size()    << "\n";
    std::cout << "no duplication -- just transferred ownership!\n";

    return 0;
}
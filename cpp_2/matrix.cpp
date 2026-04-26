#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <stdexcept>

std::mutex print_mutex;

template <typename T>
class Matrix {
private:
    int rows;
    int cols;
    T* data;    // heap allocated array

public:

    // ── constructor -- allocate heap memory ───────────────
    Matrix(int r, int c) {
        rows = r;
        cols = c;
        data = new T[rows * cols];  // heap allocation

        // initialize to zero
        for (int i = 0; i < rows * cols; i++) {
            data[i] = 0;
        }
        std::cout << "Matrix created (" << rows
                  << "x" << cols << ")\n";
    }

    // ── destructor -- free heap memory ────────────────────
    ~Matrix() {
        delete[] data;             // must free heap memory!
        std::cout << "Matrix destroyed\n";
    }

    // ── copy constructor -- deep copy ─────────────────────
    Matrix(const Matrix& other) {
        rows = other.rows;
        cols = other.cols;
        data = new T[rows * cols]; // new heap allocation
        for (int i = 0; i < rows * cols; i++) {
            data[i] = other.data[i];  // copy every value
        }
        std::cout << "Matrix copied\n";
    }

    // ── move constructor -- transfer ownership ────────────
    Matrix(Matrix&& other) {
        rows = other.rows;
        cols = other.cols;
        data = other.data;       // steal the pointer
        other.data = nullptr;    // original has nothing now
        other.rows = 0;
        other.cols = 0;
        std::cout << "Matrix moved\n";
    }

    // ── set value ─────────────────────────────────────────
    void set(int row, int col, T value) {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Index out of range!");
        }
        data[row * cols + col] = value;
    }

    // ── get value ─────────────────────────────────────────
    T get(int row, int col) const {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Index out of range!");
        }
        return data[row * cols + col];
    }

    // ── print matrix ──────────────────────────────────────
    void print() const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                std::cout << get(i, j) << " ";
            }
            std::cout << "\n";
        }
    }

    // ── single thread matrix multiply ─────────────────────
    Matrix<T> multiply(const Matrix<T>& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Invalid matrix dimensions!");
        }

        Matrix<T> result(rows, other.cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                T sum = 0;
                for (int k = 0; k < cols; k++) {
                    sum += get(i, k) * other.get(k, j);
                }
                result.set(i, j, sum);
            }
        }
        return result;
    }

    // ── threaded matrix multiply ──────────────────────────
    Matrix<T> multiply_threaded(const Matrix<T>& other,
                                 int num_threads = 4) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Invalid matrix dimensions!");
        }

        Matrix<T> result(rows, other.cols);

        // each thread computes a chunk of rows
        int chunk = rows / num_threads;

        auto compute_rows = [&](int start, int end) {
            for (int i = start; i < end; i++) {
                for (int j = 0; j < other.cols; j++) {
                    T sum = 0;
                    for (int k = 0; k < cols; k++) {
                        sum += get(i, k) * other.get(k, j);
                    }
                    result.set(i, j, sum);
                }
            }
        };

        // launch threads
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; i++) {
            int start = i * chunk;
            int end   = (i == num_threads - 1) ? rows : start + chunk;
            threads.push_back(std::thread(compute_rows, start, end));
        }

        // wait for all threads
        for (auto& t : threads) {
            t.join();
        }

        return result;
    }

    int get_rows() const { return rows; }
    int get_cols() const { return cols; }
};

int main() {

    // ── PART 1: basic matrix operations ──────────────────
    std::cout << "=== BASIC MATRIX ===\n";

    Matrix<double> A(3, 3);
    A.set(0, 0, 1.0); A.set(0, 1, 2.0); A.set(0, 2, 3.0);
    A.set(1, 0, 4.0); A.set(1, 1, 5.0); A.set(1, 2, 6.0);
    A.set(2, 0, 7.0); A.set(2, 1, 8.0); A.set(2, 2, 9.0);

    std::cout << "Matrix A:\n";
    A.print();

    // ── PART 2: copy vs move ──────────────────────────────
    std::cout << "\n=== COPY vs MOVE ===\n";

    Matrix<double> B = A;              // copy
    std::cout << "Copied B:\n";
    B.print();

    Matrix<double> C = std::move(B);   // move
    std::cout << "Moved to C:\n";
    C.print();

    // ── PART 3: matrix multiply ───────────────────────────
    std::cout << "\n=== MATRIX MULTIPLY ===\n";

    Matrix<double> M1(2, 3);
    M1.set(0, 0, 1.0); M1.set(0, 1, 2.0); M1.set(0, 2, 3.0);
    M1.set(1, 0, 4.0); M1.set(1, 1, 5.0); M1.set(1, 2, 6.0);

    Matrix<double> M2(3, 2);
    M2.set(0, 0, 7.0);  M2.set(0, 1, 8.0);
    M2.set(1, 0, 9.0);  M2.set(1, 1, 10.0);
    M2.set(2, 0, 11.0); M2.set(2, 1, 12.0);

    std::cout << "M1:\n"; M1.print();
    std::cout << "M2:\n"; M2.print();

    // single thread multiply
    std::cout << "\nSingle thread result:\n";
    Matrix<double> result1 = M1.multiply(M2);
    result1.print();

    // ── PART 4: threaded multiply ─────────────────────────
    std::cout << "\n=== THREADED MULTIPLY ===\n";

    Matrix<double> big1(4, 4);
    Matrix<double> big2(4, 4);

    // fill with values
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            big1.set(i, j, i + j + 1.0);
            big2.set(i, j, i * j + 1.0);
        }
    }

    std::cout << "big1:\n"; big1.print();
    std::cout << "big2:\n"; big2.print();

    std::cout << "\nThreaded multiply result:\n";
    Matrix<double> result2 = big1.multiply_threaded(big2, 4);
    result2.print();

    return 0;
}
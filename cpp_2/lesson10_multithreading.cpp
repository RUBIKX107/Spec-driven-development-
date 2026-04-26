#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

// mutex to prevent threads printing at same time
std::mutex print_mutex;

// ── simple thread function ────────────────────────────────
void compute_chunk(int thread_id,
                   std::vector<double>& data,
                   int start,
                   int end,
                   double& result) {

    double sum = 0.0;
    for (int i = start; i < end; i++) {
        sum += data[i];
    }
    result = sum;

    // lock mutex before printing
    // prevents garbled output from multiple threads
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "Thread " << thread_id
              << " processed indices " << start
              << " to " << end
              << " sum=" << sum << "\n";
}

int main() {

    // ── PART 1: basic thread ──────────────────────────────
    std::cout << "=== BASIC THREAD ===\n";

    // lambda function -- like Python lambda
    auto task = [](int id) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "Hello from thread " << id << "\n";
    };

    // create 3 threads
    std::thread t1(task, 1);
    std::thread t2(task, 2);
    std::thread t3(task, 3);

    // wait for all threads to finish
    t1.join();
    t2.join();
    t3.join();

    // ── PART 2: parallel data processing ─────────────────
    std::cout << "\n=== PARALLEL DATA PROCESSING ===\n";

    // large dataset
    int size = 1000;
    std::vector<double> data(size, 1.0);  // 1000 ones

    // split into 4 chunks -- one per thread
    int chunk = size / 4;
    std::vector<double> results(4, 0.0);

    // create 4 threads -- each processes one chunk
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; i++) {
        int start = i * chunk;
        int end   = start + chunk;
        threads.push_back(
            std::thread(compute_chunk, i, 
                       std::ref(data),
                       start, end,
                       std::ref(results[i]))
        );
    }

    // wait for all threads
    for (auto& t : threads) {
        t.join();
    }

    // combine results
    double total = 0.0;
    for (int i = 0; i < 4; i++) {
        total += results[i];
    }
    std::cout << "Total sum : " << total << "\n";
    std::cout << "Expected  : " << size  << "\n";

    // ── PART 3: timing comparison ─────────────────────────
    std::cout << "\n=== TIMING ===\n";

    // single thread sum
    auto start_time = std::chrono::high_resolution_clock::now();

    double single_sum = 0.0;
    for (int i = 0; i < size; i++) {
        single_sum += data[i];
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast
                    <std::chrono::microseconds>
                    (end_time - start_time).count();

    std::cout << "Single thread sum  : " << single_sum << "\n";
    std::cout << "Single thread time : " << duration << " microseconds\n";

    return 0;
}
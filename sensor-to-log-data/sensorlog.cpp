#include <iostream>
#include <queue>
#include <string>
#include <vector>

// 1. Define a Task with a Priority
struct Task {
    int priority; // Higher number = Higher priority
    std::string name;

    // Overload the < operator so priority_queue knows how to sort
    bool operator<(const Task& other) const {
        return priority < other.priority; 
    }
};

int main() {
    // 2. The Priority Queue is our "Real-Time Scheduler"
    std::priority_queue<Task> scheduler;

    // 3. Simulating tasks arriving in random order
    std::cout << "Adding tasks to the scheduler..." << std::endl;
    
    scheduler.push({1, "Log: Temperature is 22C"});       // Low priority
    scheduler.push({10, "CRITICAL: Engine Overheat!"});    // High priority
    scheduler.push({5, "Sensor: Battery Level 80%"});     // Medium priority

    // 4. Executing tasks based on Priority, not arrival time
    std::cout << "\nExecuting tasks in Real-Time order:" << std::endl;
    
    while (!scheduler.empty()) {
        Task currentTask = scheduler.top();
        std::cout << "[Priority " << currentTask.priority << "] Processing: " << currentTask.name << std::endl;
        scheduler.pop();
    }

    return 0;
}
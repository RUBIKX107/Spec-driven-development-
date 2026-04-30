#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Put the struct at the very top
struct Task {
    int id;
    std::string name;
    int priority; 
    std::string type; 
};

class TaskAgent {
public:
    void reorderTasks(std::vector<Task>& taskList, bool lowBattery) {
        for (auto& task : taskList) {
            if (lowBattery && task.type == "System") task.priority += 5;
            if (task.type == "Medical" && task.priority < 8) task.priority = 8;
        }
        std::sort(taskList.begin(), taskList.end(), [](const Task& a, const Task& b) {
            return a.priority > b.priority;
        });
    }
};

int main() {
    std::vector<Task> tasks = {
        {1, "Refill Prescription", 3, "Medical"},
        {3, "Safety Power Save", 4, "System"}
    };

    TaskAgent agent;
    agent.reorderTasks(tasks, true);

    for (const auto& t : tasks) {
        std::cout << "[" << t.type << "] " << t.name << " | Priority: " << t.priority << std::endl;
    }
    return 0;
}
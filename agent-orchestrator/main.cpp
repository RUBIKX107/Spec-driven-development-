#include "task.hpp"
#include "agent.cpp" // Simple inclusion for this exe
#include <iostream>
#include <vector>

int main() {
    std::vector<Task> tasks = {
        {1, "Refill Prescription", 3, "Medical"},
        {2, "Update Anime Watchlist", 2, "User"},
        {3, "Safety Power Save", 4, "System"}
    };

    TaskAgent agent;
    
    std::cout << "Agent is analyzing system state..." << std::endl;
    agent.reorderTasks(tasks, true); // Simulate low battery

    std::cout << "\nAgent's Optimized Execution Order:" << std::endl;
    for (const auto& t : tasks) {
        std::cout << "[" << t.type << "] " << t.name << " | Priority: " << t.priority << std::endl;
    }

    return 0;
}
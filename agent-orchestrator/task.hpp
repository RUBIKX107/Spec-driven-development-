#ifndef TASK_HPP
#define TASK_HPP

#include <string>

struct Task {
    int id;
    std::string name;
    int priority; 
    std::string type; // "Medical", "System", "User"
};

#endif
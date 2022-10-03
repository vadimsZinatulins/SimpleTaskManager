# Documentation a little bit outdated!
# Simple Task Manager

Simple Task Manager library that manages asynchronous tasks which may also have some dependecies between them.

## Usage

### Initialization and shutdown

Before runing any async task it is required it `initialize()` the `Scheduler` singleton first and `terminate()` at the end like so:

```cpp
#include <SimpleTaskManager/Scheduler.h>

int main(int argc, char *argv[]) {
    stm::Scheduler::getInstance().initialize();
    // ...
    stm::Scheduler::getInstance().terminate();

    return 0;
}
```

### Creating simple task

To create a task you must use `make_task(lambda)` where you provide a lambda that will run asynchronously:

```cpp
#include <SimpleTaskManager/Scheduler.h>
#include <SimpleTaskManager/make_task.h>
#include <iostream>

int main(int argc, char *argv[]) {
    stm::Scheduler::getInstance().initialize();

    // Create async task that will return 4
    auto asyncTask { stm::make_task([] { return 4; }); };

    // Call result() to obtain the result of a task. This method will
    // block the execution until the task is finished
    int result = asyncTask->result();

    stm::Scheduler::getInstance().terminate();

    return 0;
}
```

#### Important notes

It is not possible to have a task with return type of `void`. Unfortunately every async task must have a return type.

### Task dependecy

The following example shows how to use advantage of task dependency. For this you must use `make_task(lambda, dependencies)`:

```cpp
#include <SimpleTaskManager/Scheduler.h>
#include <SimpleTaskManager/TaskJoiner.h>
#include <SimpleTaskManager/make_task.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace stm;

int main(int argc, char *argv[]) {
    // Fake data to work with
    std::unordered_map<std::string, unsigned int> usernameToIdMap {
        std::make_pair("Patty O’Furniture", 1),
        std::make_pair("Liz Erd", 2),
        std::make_pair("Rita Book", 3),
        std::make_pair("Colin Sik", 4)
    };
    std::vector<unsigned int> salaries { 1200, 1500, 1350, 920 };

    // Initialize the scheduler
    Scheduler::getInstance().initialize();

    // Might represent retrieving username from the user
    auto retrieveUsername { make_task([] { return std::string("Colin Sik"); }) };

    // Might represent retrieving user id from a database
    auto retrieveUserID {
        make_task([&usernameToIdMap] (std::string username) {
            auto pairPtr = usernameToIdMap.find(username);
            if(pairPtr == usernameToIdMap.end()) {
                return 0u;
            }

            return pairPtr->second;
        }, retrieveUsername) 
    };
    
    // Might represent retrieving user id from another database
    auto retrieveUserSalary { 
        make_task([&salaries] (unsigned int id) {
            if(id == 0) {
                return 0u;
            }
    
            return salaries[id - 1];
        }, retrieveUserID)
    };
    
    // Display data to the user
    auto printUserSalary {
        make_task([] (std::string username, unsigned int salary) {
            std::cout << "User: " << username << std::endl;
            std::cout << "Salary: " << salary << std::endl;

            return 0;
        }, retrieveUsername, retrieveUserSalary)
    };  

    // Wait until the data is presented to the user
    printUserSalary->result();
        
    // Terminate the scheduler
    Scheduler::getInstance().terminate();

    return 0;
}
```

#### Important notes

Once again make sure that none of your tasks have a return type of void.

### Task unpack

You can return a task from you tasks. By doing so, you task will get executed asynchronously and will be unpacked like so:

```cpp
#include <SimpleTaskManager/Scheduler.h>
#include <SimpleTaskManager/make_task.h>
#include <iostream>

using namespace stm;

int main(int argc, char *argv[]) {
    Scheduler::getInstance().initialize();

    auto task { 
        make_task([] { 
            return make_task([] { return 4; }); 
        })
    };  

    int result = task->result();

    std::cout << "Result: " << result << std::endl; // Will print 4

    Scheduler::getInstance().terminate();

    return 0;
}
```

## Build Simple Task Manager

To build this project make sure you're using C++ 20 and have CMake installed.

In the root directory you can run the following commands to build and install this library:

```bash
cmake -B build
cmake --build build
sudo cmake --install build
```

## Include Simple Task Manager

To include Simple Task Manager you can use the following inside your `CMakeLists.txt`:

```cmake
find_package(SimpleTaskManager)

target_link_libraries(MyTarget STM::SimpleTaskManager)
```

# Credits

Credits goes to [Vladimir Mokhovikov](https://github.com/nongeneric)

This project is based on Vladimir Mokhovikov's tutorial "[a tiny C++ task library](https://www.rcebits.com/code/2019/05/02/tinytasks.html)" which you should definitely checkout if you are interested in implementation details.

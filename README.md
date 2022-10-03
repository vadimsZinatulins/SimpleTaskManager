# Simple Task Manager

This is a small library that manages tasks that run asynchronously. It also manages the dependecy between tasks as well as unpacking tasks (see examples bellow).

# Usage

## Initialization and shutdown

Before runing any async task it is required it `initialize()` the `Scheduler` (*singleton* class) first and `terminate()` it at the end like so:

```cpp
#include <SimpleTaskManager/Scheduler.h>

int main(int argc, char *argv[]) {
    stm::Scheduler::getInstance().initialize();
    // ...
    stm::Scheduler::getInstance().terminate();

    return 0;
}
```

## Hello World task

The creation of tasks is done by calling `make_task(lambda)` like in the following example:

```cpp
#include <SimpleTaskManager/Scheduler.h>
#include <SimpleTaskManager/make_task.h>
#include <iostream>

int main(int argc, char *argv[]) {
    stm::Scheduler::getInstance().initialize();

    // Create async task that will print "Hello World"
    auto helloWorldTask { stm::make_task([] { std::cout << "Hello World!" << std::endl; }) };

    // The result() blocks the rest of the execution until
    // helloWorldTask task gets executed
    helloWorldTask->result();

    stm::Scheduler::getInstance().terminate();

    return 0;
}
```

## Task with return value

Sometimes it is necessary to a task return a value. This library allows tasks to have a return type:

```cpp
#include <SimpleTaskManager/Scheduler.h>
#include <SimpleTaskManager/make_task.h>
#include <iostream>

int main(int argc, char *argv[]) {
    stm::Scheduler::getInstance().initialize();

    // Create async task that will return 4
    auto getFour { stm::make_task([] { return 4; }) };

    // Create async task that will return 6
    auto getSix { stm::make_task([] { return 6; }) };

    // Calculate the sum of both tasks and print it
    int sum { getFour->result() + getSix->result() };
    std::cout << "Sum: " << sum << std::endl;

    stm::Scheduler::getInstance().terminate();

    return 0;
}
```

The return type of `result()` is based on the return type of the lambda that is passed to `make_task(lambda)`.

## Task dependecy

It is possible for one task to depend on the result of the other task (or other tasks). For this it is necessary to specify which tasks the current task being created is dependent on when using `make_task(lambda, ...dependencies)` like so:

```cpp
#include <SimpleTaskManager/Scheduler.h>
#include <SimpleTaskManager/make_task.h>
#include <iostream>

int main(int argc, char *argv[]) {
    stm::Scheduler::getInstance().initialize();

    // Create async task that will return 4
    auto getFour { stm::make_task([] { return 4; }) };

    // Create async task that will return 6
    auto getSix { stm::make_task([] { return 6; }) };

    // Create a task that will print the sum of both tasks
    auto printSum { stm::make_task([] (int a, int b) { std::cout << "Sum: " << (a + b) << std::endl; }, getFour, getSix) };

    // Make sure that the result gets printed before the application gets closed
    printSum->result();

    stm::Scheduler::getInstance().terminate();

    return 0;
}
```

The types of the arguments for the lambda (for `printSum` task) must match the return type of the dependencies except for those with `void` return type which is discussed bellow.

## Pseudo task order execution

It is not completly possible to specify the order for tasks execution. However it is possible to use the *Task dependency* to specify that one task must only run after the execution of another task like so:

```cpp
#include <SimpleTaskManager/Scheduler.h>
#include <SimpleTaskManager/make_task.h>
#include <iostream>

int main(int argc, char *argv[]) {
    stm::Scheduler::getInstance().initialize();

    // Create async task that will print "Hello"
    auto printHello { stm::make_task([] { std::cout << "Hello" << std::endl; }) };
    
    // Create async task that will return a string
    auto getName { stm::make_task([] { return std::string("Vadims") }) };

    // Create a task that will print the value retrieve from getName task
    auto printName {  stm::make_task([] (std::string name) { std::cout << name << std::endl; }, getName, printHello) };

    // Make sure the message gets printed before the applications gets closed
    printName->result();

    stm::Scheduler::getInstance().terminate();

    return 0;
}
```

In normal circumstances nothing prevents the `getName` task and `printName` task to execute faster/before the `printHello` task which would lead to an undesired output. To make sure that `printName` executes after `printHello` the `printHello` is marked as a dependency for `printName`. Note that `printName` has two dependencies but only one argument, this is because `void` type tasks get excluded from argument list for the lambda.

## Task unpack

It is also possible for a task to return another task. In this case the return type of the task is defined by the deepest task:

```cpp
#include <SimpleTaskManager/Scheduler.h>
#include <SimpleTaskManager/make_task.h>
#include <iostream>

using namespace stm;

int main(int argc, char *argv[]) {
    Scheduler::getInstance().initialize();

    // Create async task that returns another task
    auto task { 
        make_task([] { 
            // The return type of this task will define the return type of the outer task
            return make_task([] { return 4; }); 
        })
    };  

    // The return type of task is int
    int result { task->result() };

    // Will print 4
    std::cout << "Result: " << result << std::endl; 

    Scheduler::getInstance().terminate();

    return 0;
}
```

# Build Simple Task Manager

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

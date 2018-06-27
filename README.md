# Description
This is a tiny thread pool based on  new C++11 features including `std::function`, `std::thread`, `std::bind`, `lambda` and `condition_variable`. **Only one header file and not extra dependencies**.

**This project is not a high-performance or efficient library** with complicated logic. The goal of this project is to provide a clean, beautiful and easy-to-understand thread pool implementation for beginners.

I think its a good learning material for those who: 
> * want to grasp new C++11 features about **threading supported at the language-level**, including threading communication, from simple examples 
> * need a deep understanding of thread pool from a concise and clear implementation

For C++ beginners, this project is also useful.


# Basic Usage
```C++
// include the head file
#include "thread_pool.hpp" 

// create a thread pool with 8 worker threads
ttp::thread_pool tp(8);

//add a task to the thread pool
std::function<void()> bar = []() { return 1; };
tp.add_task(bar);

// stop the thread pool and wait for workers completion
tp.stop();
```

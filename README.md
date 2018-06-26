# Description
This is a tiny thread pool based on  C++11 new features including `std::function`, `std::thread`, `std::bind`, `lambda` and `condition_variable`. **Only one header file and not extra dependencies**.

I think its a good learning material for those who: 
> * want to grasp C++11 new features on **threading supported at the language-level**, from simple examples 
> * need a deep understanding of thread pool from a concise and clear implementation.

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

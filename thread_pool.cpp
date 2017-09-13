#include "thread_pool.h"


thread_pool::thread_pool(std::size_t nb_threads):should_stop(false){
  for(int i=0;i<nb_threads;i++){
    workers.push_back(std::thread(std::bind(&thread_pool::unit_run, this)));
  }
}


thread_pool::~thread_pool(void){
  stop();
}


void thread_pool::unit_run(void){
  while(!should_stop){
    task_t task = fetch_task();
    if(task){
      task();
    }
  }
}

void thread_pool::stop(void){
  should_stop =true;
  tasks_condvar.notify_all();

  for(auto& worker: workers) {worker.join();}

  workers.clear();
}

void thread_pool::add_task(task_t new_task){
  std::unique_lock<std::mutex> lock(tasks_mtx);
  tasks.push(new_task);
  lock.unlock();
  tasks_condvar.notify_all();
}



task_t thread_pool::fetch_task(){
  std::unique_lock<std::mutex> lock(tasks_mtx);
  tasks_condvar.wait(lock, [&]{return should_stop || !tasks.empty();});


  if(tasks.empty()){ return nullptr; }
  task_t task=std::move(tasks.front());
  tasks.pop();
  return task;
}


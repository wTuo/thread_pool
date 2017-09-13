#include "thread_pool.h"


thread_pool::thread_pool(std::size_t nb_threads){
  for(int i=0;i<nb_threads;i++){
    workers.push_back(std::thread(std::bind(&thread_pool::unit_run, this)));
  }
}


// thread_pool::~thread_pool(void){
//   stop();
// }


void thread_pool::unit_run(void){
  while(true){
    task_t task = fetch_task();
    if(task){
      task();
    }
  }
}

// void thread_pool::stop(void){
//   m_should_stop =true;
//   m_tasks_condvar.notify_all();

//   for(auto& worker: m_workers) {worker.join();}

//   m.workers.clear();
// }

void thread_pool::add_task(task_t new_task){
  std::unique_lock<std::mutex> lock(tasks_mtx);
  tasks.push(new_task);
  lock.unlock();
  tasks_condvar.notify_all();
}



task_t thread_pool::fetch_task(){
  std::unique_lock<std::mutex> lock(tasks_mtx);
  tasks_condvar.wait(lock, [&]{return tasks.empty();});


  if(tasks.empty()){ return nullptr; }
  task_t task=std::move(tasks.front());
  tasks.pop();
  return task;
}


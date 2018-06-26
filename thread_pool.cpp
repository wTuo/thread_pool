#include <thread_pool.hpp>


namespace ttp {

thread_pool::thread_pool(std::size_t nb_threads) {

  for (std::size_t i = 0; i < nb_threads; ++i) { m_workers.push_back(std::thread(std::bind(&thread_pool::run, this))); }
}

thread_pool::~thread_pool(void) {
  stop();
}

// worker main loop
void
thread_pool::run(void) {

  while (!m_should_stop) {
    task_t task = fetch_task();
    if (task) {
      task();
    }
  }

}

// stop the thread pool and wait for workers completion

void
thread_pool::stop(void) {
  if (!is_running()) { return; }

  m_should_stop = true;
  m_tasks_condvar.notify_all();

  for (auto& worker : m_workers) { worker.join(); }

  m_workers.clear();

}

// whether the thread_pool is running or not

bool
thread_pool::is_running(void) const {
  return !m_should_stop;
}

//! retrieve a new task

thread_pool::task_t
thread_pool::fetch_task(void) {
  std::unique_lock<std::mutex> lock(m_tasks_mtx);

  m_tasks_condvar.wait(lock, [&] { return m_should_stop || !m_tasks.empty(); });

  if (m_tasks.empty()) { return nullptr; }

  task_t task = std::move(m_tasks.front());
  m_tasks.pop();
  return task;
}

// add tasks to thread pool

void
thread_pool::add_task(const task_t& task) {
  std::lock_guard<std::mutex> lock(m_tasks_mtx);
  m_tasks.push(task);
  m_tasks_condvar.notify_all();
}

thread_pool&
thread_pool::operator<<(const task_t& task) {
  add_task(task);
  return *this;
}

} // ttp

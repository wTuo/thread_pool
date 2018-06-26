#include <atomic>
#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>


namespace ttp {

class thread_pool {
public:
  explicit thread_pool(std::size_t nb_threads);
  ~thread_pool(void);

  thread_pool(const thread_pool&) = delete;
  thread_pool& operator=(const thread_pool&) = delete;

public:
  // task typedef
  typedef std::function<void()> task_t;

  // add tasks to thread pool
  void add_task(const task_t& task);
  thread_pool& operator<<(const task_t& task);

  // stop the thread pool and wait for workers completion
  void stop(void);

public:
  // whether the thread_pool is running or not
  bool is_running(void) const;

private:
  // worker main loop
  void run(void);

  // retrieve a new task
  task_t fetch_task(void);

private:
  // threads
  std::vector<std::thread> m_workers;

  // whether the thread_pool should stop or not
  std::atomic<bool> m_should_stop = ATOMIC_VAR_INIT(false);

  // tasks
  std::queue<task_t> m_tasks;

  // thread safety
  std::mutex m_tasks_mtx;
  std::condition_variable m_tasks_condvar;
};

} // ttp 


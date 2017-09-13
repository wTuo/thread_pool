#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

typedef std::function<void()> task_t;

class thread_pool{
 public:
  thread_pool(std::size_t nb_threads);
  ~thread_pool(void);
  void add_task(task_t new_task);
  void stop();
 private:
  bool should_stop;
  std::vector<std::thread> workers;
  std::queue<task_t> tasks;
  void unit_run(void);
  task_t fetch_task();
  std::mutex tasks_mtx;
  std::condition_variable tasks_condvar;

};


#ifndef SYNC_QUEUE_H
#define SYNC_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class sync_queue
{
 public:

  void pop(T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex);
    while (queue.empty())
    {
      cond.wait(mlock);
    }
    item = queue.front();
    queue.pop();
  }

  void push(const T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex);
    queue.push(item);
    mlock.unlock();
    cond.notify_one();
  }

 private:
  std::queue<T> queue;
  std::mutex mutex;
  std::condition_variable cond;
};

#endif // SYNC_QUEUE_H

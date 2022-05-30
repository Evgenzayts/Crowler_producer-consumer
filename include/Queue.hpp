// Copyright 2022 Evgenzayts evgenzaytsev2002@yandex.ru

#ifndef INCLUDE_QUEUE_HPP_
#define INCLUDE_QUEUE_HPP_

#include <iostream>
#include <mutex>
#include <queue>
#include <string>

// класс, выполняющий все методы класса std::queue
class Queue {
 public:
  void push(std::string& str) {
    std::lock_guard<std::mutex> lk{mutex};
    _queue.push(str);
  }

  std::string front() {
    std::lock_guard<std::mutex> lk{mutex};
    return _queue.front();
  }

  void pop() {
    std::lock_guard<std::mutex> lk{mutex};
    _queue.pop();
  }

  bool empty() {
    std::lock_guard<std::mutex> lk{mutex};
    return _queue.empty() ? 1 : 0;
  }

 private:
  std::mutex mutex;
  std::queue<std::string> _queue;
};

#endif  // INCLUDE_QUEUE_HPP_

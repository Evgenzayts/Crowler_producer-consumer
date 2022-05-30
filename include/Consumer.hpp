// Copyright 2022 Evgenzayts evgenzaytsev2002@yandex.ru

#ifndef INCLUDE_CONSUMER_HPP_
#define INCLUDE_CONSUMER_HPP_

#include <gumbo.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

#include "Queue.hpp"
#include "ThreadPool.hpp"

class Consumer {
 public:
  explicit Consumer(size_t& threads);

  void Working(std::string& output_file, Queue& producer_queue);
  void Write_in_file(std::string& output_file);
  void Search_for_images(GumboNode* node);

 private:
  ThreadPool _threads_links;  // для ссылок
  Queue _queue;
};

#endif  // INCLUDE_CONSUMER_HPP_

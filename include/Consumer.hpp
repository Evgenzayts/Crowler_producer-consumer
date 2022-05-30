// Copyright 2022 Evgenzayts evgenzaytsev2002@yandex.ru

#ifndef INCLUDE_CONSUMER_HPP_
#define INCLUDE_CONSUMER_HPP_

#include <fstream>
#include <unistd.h>
#include <iostream>
#include <string>

#include <gumbo.h>

#include "ThreadPool.hpp"
#include "Queue.hpp"

class Consumer {
 public:
  explicit Consumer(size_t& threads);

  void Working(std::string &output_file, Queue& producer_queue);
  void Write_in_file(std::string & output_file);
  void Search_for_images(GumboNode* node);

 private:
  ThreadPool _threads_links; // для ссылок
  Queue _queue;
};

#endif  // INCLUDE_CONSUMER_HPP_

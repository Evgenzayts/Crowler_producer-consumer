//
// Created by ubuntu on 5/29/22.
//

#ifndef INCLUDE_PRODUCER_HPP_
#define INCLUDE_PRODUCER_HPP_

#include <iostream>
#include <vector>
#include <fstream>
#include <gumbo.h>
#include "ThreadPool.hpp"

class Producer {
 private:
  ThreadPool _threads;
  std::vector<std::string> _links;

 public:
  explicit Producer(size_t& threads);

  void DownloadPages(std::string& url);
};

#endif  // INCLUDE_PRODUCER_HPP_

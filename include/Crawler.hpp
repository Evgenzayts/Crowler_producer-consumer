// Copyright 2022 Evgenzayts evgenzaytsev2002@yandex.ru

#ifndef INCLUDE_CRAWLER_HPP_
#define INCLUDE_CRAWLER_HPP_

#include <string>
#include <iostream>

#include <boost/asio/ip/tcp.hpp>
#include <boost/program_options.hpp>
#include <gumbo.h>

#include "Consumer.hpp"
#include "Producer.hpp"

namespace po = boost::program_options;

struct Arguments {
  std::string url;
  std::string output;
  size_t depth;
  size_t network_threads;
  size_t parser_threads;
};

void start(Arguments& arguments, Consumer& consumer, Producer& producer);

#endif  // INCLUDE_CRAWLER_HPP_

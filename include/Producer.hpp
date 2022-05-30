// Copyright 2022 Evgenzayts evgenzaytsev2002@yandex.ru

#ifndef INCLUDE_PRODUCER_HPP_
#define INCLUDE_PRODUCER_HPP_

#include <iostream>
#include <future>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>

#include <gumbo.h>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/error.hpp>

#include "RootCertificates.hpp"
#include "ThreadPool.hpp"
#include "Queue.hpp"

namespace ssl = boost::asio::ssl;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

struct MyConnect {
  std::string host;
  std::string target;
  std::string port = "443"; // https
  int version = 11;
};

class Producer {
 private:
  ThreadPool _threads;
  std::vector<std::string> _links;
  MyConnect _info_connect;
  std::mutex mutex;

 public:
  //future- это объект, который может извлекать значение
  // из некоторого объекта,
  // правильно синхронизируя доступ, если он находится в разных потоках.
  std::vector<std::future<std::string>> _my_urls;
  Queue _queue;

 public:
  explicit Producer(size_t& threads);

  std::string Download_page(std::string& url);
  void Search_for_links(GumboNode* node);
  void Download_next();

  MyConnect Get_info();

 private:
  void Take_host(std::string& url);
  void Take_target(std::string& url);
};

#endif  // INCLUDE_PRODUCER_HPP_

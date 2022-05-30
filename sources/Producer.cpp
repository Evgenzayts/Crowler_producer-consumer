// Copyright 2022 Evgenzayts evgenzaytsev2002@yandex.ru

#include "Producer.hpp"

void Producer::Search_for_links(GumboNode* node) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }

  // Структура, представляющая один атрибут в теге HTML
  /*GumboAttribute* href;
  if (node->v.element.tag == GUMBO_TAG_A &&
      (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
    std::cout << href->value << std::endl;
  }*/

  // загружаем все дочерние ссылки
  GumboVector* children = &node->v.element.children;
  for (size_t i = 0; i < children->length; ++i) {
    Search_for_links(static_cast<GumboNode*>(children->data[i]));
  }
}

Producer::Producer(size_t& threads) : _threads(threads) {}

std::string Producer::Download_page() {
  try {
    boost::asio::io_context ioc;
    // объект контекста SSL используется
    // для установки параметров SSL: режим проверки,
    // файлы сертификатов, ...
    ssl::context sslc{ssl::context::sslv23_client};
    load_root_certificates(sslc);

    tcp::resolver resolver{ioc};
    ssl::stream<tcp::socket> stream{ioc, sslc};

    auto const temp_host = _info_connect.host.c_str();
    if (!SSL_set_tlsext_host_name(stream.native_handle(), temp_host)) {
      boost::system::error_code ec{static_cast<int>(::ERR_get_error()),
                                   boost::asio::error::get_ssl_category()};
      throw boost::system::system_error{ec};
    }
    // Ищем доменное имя
    auto const results =
        resolver.resolve(_info_connect.host, _info_connect.port);

    boost::asio::connect(stream.next_layer(), results.begin(), results.end());

    // Устанавливаем SSL соединенение
    stream.handshake(ssl::stream_base::client);

    // Настройка HTTP-запроса на получение сообщения
    http::request<http::string_body> request{
        http::verb::get, _info_connect.target, _info_connect.version};
    request.set(http::field::host, _info_connect.host);
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    http::write(stream, request);  // Отправить HTTP-запрос

    boost::beast::flat_buffer buffer;
    http::response<http::string_body> response;
    http::read(stream, buffer, response);

    boost::system::error_code ec;
    if (ec) {
      stream.shutdown(ec);
      throw boost::system::system_error{ec};
    }

    // cтавим ответ в очередь для парсера
    _queue.push(response.body());

    return response.body();
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
  }

  return "";
}

void Producer::Download_next() {
  size_t counts = _my_urls.size();
  for (size_t i = 0; i < counts; ++i) {
    GumboOutput* out = gumbo_parse(_my_urls[i].get().c_str());
    // root - указатель на корневой узел. Это тег,
    // который формирует корень документа.
    Search_for_links(out->root);
    // освобождаем память, выделенную под парсинг
    gumbo_destroy_output(&kGumboDefaultOptions, out);
  }
}

void Producer::Take_host(std::string& url) {
  std::string temp = url;

  if (url.find("https://") == 0) temp = url.substr(8);

  for (char sym : temp) {
    if (sym == '/') break;

    _info_connect.host += sym;
  }
}

void Producer::Take_target(std::string& url) {
  std::string temp = url;

  if (url.find("https:") == 0) temp = url.substr(8);

  size_t end_host = 0;
  for (; end_host < temp.size(); ++end_host) {
    if (temp[end_host] == '/') break;
  }

  for (size_t i = end_host; i < temp.size(); ++i) {
    _info_connect.target += temp[i];
  }
}
MyConnect Producer::Get_info() { return _info_connect; }

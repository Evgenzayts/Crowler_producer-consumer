// Copyright 2022 Evgenzayts evgenzaytsev2002@yandex.ru

#include "Crawler.hpp"

void start(Arguments& arguments, Consumer& consumer, Producer& producer) {
  auto temp_depth = arguments.depth;

  GumboOutput* out = gumbo_parse(
      producer.Download_page(arguments.url).c_str());
  // подгружаем ссылки с начальной страницы
  producer.Search_for_links(out->root);
  gumbo_destroy_output(&kGumboDefaultOptions, out);

  // обрабатываем их
  consumer.Working(arguments.output, producer._queue);
  --temp_depth;

  //переходим на следующую страницу, пока не достигнут предел глубины
  while (temp_depth > 0){
    --temp_depth;
    producer.Download_next();
    consumer.Working(arguments.output, producer._queue);
  }
}
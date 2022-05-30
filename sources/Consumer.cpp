// Copyright 2022 Evgenzayts evgenzaytsev2002@yandex.ru

#include "Consumer.hpp"

Consumer::Consumer(size_t& threads) : _threads_links(threads) {}

void Consumer::Working(std::string& output_file, Queue& producer_queue) {
  //в пул потоков парсера забираем задачи из producer_queue
  while (true) {
    if (producer_queue.empty()) {
      sleep(2);
      if (producer_queue.empty())
        break;
      else
        continue;
    }

    //помещаем в пул все картинки
    _threads_links.enqueue(&Consumer::Search_for_images, this,
                        gumbo_parse(producer_queue.front().c_str())->root);
    //снимаем задачу
    producer_queue.pop();
  }

  Write_in_file(output_file);
}

void Consumer::Write_in_file(std::string& output_file) {
  std::ofstream file;
  file.open(output_file);

  while (true) {
    if (_queue.empty()) {
      sleep(2);
      if (_queue.empty())
        break;
      else
        continue;
    }

    file << _queue.front() << std::endl;
    _queue.pop();
  }

  file.close();
}

void Consumer::Search_for_images(GumboNode* node) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }

  // Это пара имя-значение, но также включает в себя информацию
  // о местоположении источника и исходном исходном тексте
  GumboAttribute* src = nullptr;
  //проверяем что это изображение
  if (node->v.element.tag == GUMBO_TAG_IMG &&
      (src = gumbo_get_attribute(&node->v.element.attributes, "src"))) {
    std::string str = src->value;

    // ставим в очередь на запись в файл
    _queue.push(str);
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    Search_for_images(static_cast<GumboNode*>(children->data[i]));
  }
}

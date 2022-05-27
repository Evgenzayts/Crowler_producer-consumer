#include <iostream>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace po = boost::program_options;
namespace ssl = boost::asio::ssl;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

struct Arguments {
  std::string url;
  std::string output;
  size_t depth;
  size_t network_threads;
  size_t parser_threads;
};

void parse_console(int argc,char *argv[], Arguments& arg){
  // options_description - класс, объявляющий разрешённые опции.
  // add_options - метод этого класса, который возвращает
  // специальный прокси-объект, определяющий operator().
  // Вызовы этого оператора объявляют параметры.

  try {
    po::options_description desc{"Arguments"};
    desc.add_options()
        ("help", "Help screen")
          ("url", po::value<std::string>())
            ("output", po::value<std::string>())
               ("depth",po::value<size_t>())
                    ("network_threads",po::value<size_t>())
                        ("parser_threads",po::value<size_t>());

    po::variables_map vm; // аналогично std::map
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help"))
      std::cout << desc << std::endl;
    if (vm.count("url")
        && vm.count("output")
        && vm.count("depth")
        && vm.count("network_threads")
        && vm.count("parser_threads")) {
      arg.url = vm["url"].as<std::string>();
      arg.output = vm["output"].as<std::string>();
      arg.depth = vm["depth"].as<size_t>();
      arg.network_threads = vm["network_threads"].as<size_t>();
      arg.parser_threads = vm["parser_threads"].as<size_t>();
    } else {
      throw boost::program_options::error(
          "Invalid arguments\n"
          "Look to --help or -h and write all options");
    }
  } catch (const boost::program_options::error &e) {
  std::cerr << e.what() << std::endl;
  }
}

int main(int argc, char* argv[]) {
  Arguments arg;
  parse_console(argc, argv, arg);

  //Producer p(arg.network_threads);
  //Consumer k(arg.parser_threads);

  //crawler(arg.url,arg.depth,k, p, arg.output, p.parser_queue_);

  return 0;
}
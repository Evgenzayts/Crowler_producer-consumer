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

void parse_console(int argc,char *argv[], Arguments& arguments){
  // options_description - класс, объявляющий разрешённые опции.
  // add_options - метод этого класса, который возвращает
  // специальный прокси-объект, определяющий operator().
  // Вызовы этого оператора объявляют параметры.

  try {
    po::options_description desc{"Arguments"};
    desc.add_options()
        ("help", "Help screen")
          ("url", po::value<std::string>(),
              "Input url")
            ("output", po::value<std::string>(),
                "Name of output file")
               ("depth",po::value<size_t>(),
                   "Depth of search by page")
                    ("network_threads",po::value<size_t>(),
                        "Count of threads for downloading")
                        ("parser_threads",po::value<size_t>(),
                            "Count of threads for processing");

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
      arguments.url = vm["url"].as<std::string>();
      arguments.output = vm["output"].as<std::string>();
      arguments.depth = vm["depth"].as<size_t>();
      arguments.network_threads = vm["network_threads"].as<size_t>();
      arguments.parser_threads = vm["parser_threads"].as<size_t>();
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
  Arguments arguments;
  parse_console(argc, argv, arguments);

  try {
    //Producer p(arguments.network_threads);
    //Consumer k(arguments.parser_threads);
    //crawler(arguments.url,arguments.depth,k, p, arguments.output, p.parser_queue_);
  } catch (std::exception const& exception) {
    std::cerr << exception.what() << std::endl;
  }

  return 0;
}
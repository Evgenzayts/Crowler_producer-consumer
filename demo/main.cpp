#include "Producer.hpp"
#include "Consumer.hpp"
#include "Crawler.hpp"

void parse_console(int argc,char *argv[], Arguments& arguments){
  try {
    po::options_description desc{"Arguments"};
    desc.add_options()
        ("help,h", "Help screen")
          ("url,u", po::value<std::string>(),
              "Input url")
            ("output,o", po::value<std::string>(),
                "Name of output file")
               ("depth,d",po::value<size_t>(),
                   "Depth of search by page")
                    ("network_threads,n",po::value<size_t>(),
                        "Count of threads for downloading")
                        ("parser_threads,p",po::value<size_t>(),
                            "Count of threads for processing");

    po::variables_map vm; // аналогично std::map
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
    }
    if (vm.count("url")) {
      arguments.url = vm["url"].as<std::string>();
    }
    if (vm.count("output")) {
      arguments.output = vm["output"].as<std::string>();
    }
    if (vm.count("depth")) {
      arguments.depth = vm["depth"].as<size_t>();
    }
    if (vm.count("network_threads")) {
      arguments.network_threads = vm["network_threads"].as<size_t>();
    }
    if (vm.count("parser_threads")) {
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
    Producer producer(arguments.network_threads);
    Consumer consumer(arguments.parser_threads);
    start(arguments, consumer, producer);
  } catch (std::exception const& exception) {
    std::cerr << exception.what() << std::endl;
  }

  return 0;
}
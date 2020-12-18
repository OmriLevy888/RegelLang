#include "cli/CliParser.hpp"
#include "cli/CliArguments.hpp"
#include "cli/Context.hpp"
#include "cli11/App.hpp"
#include "cli11/Config.hpp"
#include "cli11/Formatter.hpp"

namespace rgl {
bool CliParser::parseCliArgument(size_t argc, const char **argv) {
  CLI::App app{"Regelang compiler"};
  CliArguments arguments;

  std::string file;
  app.add_option("project-file", file,
                 "Path to project file (defaults to Project.json)");
  app.add_option("-p,--project-file", arguments.m_projectFilePath,
                 "Path to the project file (defaults to Project.json)");
  app.add_option("-t,--target", arguments.m_target, "Build target");

  CLI11_PARSE(app, argc, argv);

  if (file != "" && arguments.m_projectFilePath != "") {
    // TODO: log error message
    std::cerr << app.help();
    return false;
  } else if (file != "") {
    arguments.m_projectFilePath = file;
  }

  Context::getInstance().m_cliArguments = arguments;
  return true;
}
}; // namespace rgl

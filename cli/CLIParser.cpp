#include "cli/CLIParser.hpp"
#include "cli/CLIArguments.hpp"
#include "cli/CLIContext.hpp"
#include "cli11/App.hpp"
#include "cli11/Config.hpp"
#include "cli11/Formatter.hpp"
#include "common/Formatter.hpp"
#include "common/errors/ErrorManager.hpp"

namespace rgl {
bool CLIParser::parseCliArgument(size_t argc, const char **argv) {
  CLI::App app{"Regelang compiler"};
  CLIArguments arguments;

  std::string file;
  app.add_option("project-file", file,
                 "Path to project file (defaults to Project.json)");
  app.add_option("-p,--project-file", arguments.m_projectFilePath,
                 "Path to the project file (defaults to Project.json)");
  app.add_option("-t,--target", arguments.m_target, "Build target");

  CLI11_PARSE(app, argc, argv);

  if (file != "" && arguments.m_projectFilePath != "") {
    ErrorManager::logErrorFmt(
        ErrorTypes::E_AMBIGIOUS_PROJECT_FILE,
        "Specified project file both as a positional value ({}) and "
        "as a switch value ({}), can only specify one",
        file, arguments.m_projectFilePath);
    std::cerr << app.help();
    return false;
  } else if (file != "") {
    arguments.m_projectFilePath = file;
  } else if (arguments.m_projectFilePath == "") {
    // TODO: change this when support for single file is added
    ErrorManager::logError(ErrorTypes::E_NO_PROJECT_FILE_SPECIFIED,
                           "No project file specified");
    std::cerr << app.help();
    return false;
  }

  CLIContext::getInstance().m_cliArguments = arguments;
  return true;
}
}; // namespace rgl

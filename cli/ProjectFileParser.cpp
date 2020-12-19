#include "cli/ProjectFileParser.hpp"
#include "cli/Context.hpp"
#include "common/Formatter.hpp"
#include "common/errors/ErrorManager.hpp"

namespace rgl {
bool ProjectFileParser::parseProjectFile(const std::string &path,
                                         const std::string &chosenTarget) {
  Context &context = Context::getInstance();

  std::ifstream ifs{path};
  if (!ifs.is_open()) {
    ErrorManager::logErrorFmt("Failed to open {}, no such project file", path);
    return false;
  }

  const size_t lastSlashIdx = path.rfind('/');
  if (lastSlashIdx == std::string::npos) {
    context.m_projectDir = Formatter("{}/", path).toString();
  } else {
    context.m_projectDir =
        Formatter("{}/", path.substr(0, lastSlashIdx)).toString();
  }

  rapidjson::Document document;
  std::string jsonStr{std::istreambuf_iterator<char>(ifs),
                      std::istreambuf_iterator<char>()};
  document.Parse(jsonStr.c_str());
  std::string target;

  if (!document.HasMember("ProjectName") || !document["ProjectName"].IsString())
    context.m_projectName = "";
  else
    context.m_projectName = document["ProjectName"].GetString();

  if (chosenTarget == "") {
    if (document.HasMember("DefaultTarget")) {
      if (!document["DefaultTarget"].IsString()) {
        ErrorManager::logError("No build target specified and DefaultTarget "
                               "field is not a string");
        return false;
      }
      target = document["DefaultTarget"].GetString();
    } else {
      ErrorManager::logError(
          "No build target specified and DefaultTarget field does not exist");
      return false;
    }
  } else
    target = chosenTarget;

  if (target == "" || !document.HasMember(target.c_str())) {
    ErrorManager::logError("Build target {} does not exists", target);
    return false;
  } else if (!document[target.c_str()].IsObject()) {
    ErrorManager::logErrorFmt("Build target {} is not an object", target);
    return false;
  }

  auto buildTarget =
      BuildTarget::makeTarget(document[target.c_str()].GetObject());
  if (!buildTarget.has_value()) {
    ErrorManager::logError("Failed to parse build target");
    return false;
  }

  context.m_target = std::move(buildTarget.value());
  interpretFields();

  return true;
}

void ProjectFileParser::interpretFields() {
  Context &context = Context::getInstance();
  interpretField(context.m_target.m_output);
  for (auto &file : context.m_target.m_files) {
    interpretField(file);
  }
}

void ProjectFileParser::interpretField(std::string &fieldValue) {
  Context &context = Context::getInstance();
  std::string formatted = "";
  for (size_t idx = 0; idx < fieldValue.size(); idx++) {
    if (fieldValue[idx] != '$' || idx >= fieldValue.size() - 3) {
      formatted += fieldValue[idx];
      continue;
    }

    if (fieldValue[idx + 1] != '{') { // did not find {
      formatted += fieldValue[idx];
      continue;
    }

    size_t end;
    for (end = idx + 2; end < fieldValue.size(); end++) {
      if (fieldValue[end] == '}')
        break;
    }

    if (end > fieldValue.size()) { // did not find }
      formatted += fieldValue[idx];
      continue;
    }

    std::string_view substitutionName{fieldValue.c_str() + idx + 2,
                                      end - idx - 2};
    if (substitutionName == "ProjectName") {
      formatted += context.m_projectName;
      idx = end;
    } else if (substitutionName == "ProjectDir") {
      formatted += context.m_projectDir;
      idx = end;
    } else {
      formatted += fieldValue[idx];
    }
  }
  fieldValue = formatted;
}
}; // namespace rgl

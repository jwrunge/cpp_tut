#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <string>
#include <sys/stat.h>

namespace fs = std::filesystem;

fs::file_time_type get_folder_last_modified_time(const fs::path &folder_path) {
  fs::file_time_type last_modified_time = fs::last_write_time(folder_path);

  for (const auto &entry : fs::recursive_directory_iterator(folder_path)) {
    try {
      fs::file_time_type entry_time = fs::last_write_time(entry.path());
      if (entry_time > last_modified_time) {
        last_modified_time = entry_time;
      }
    } catch (const fs::filesystem_error &ex) {
      std::cerr << "Error accessing " << entry.path() << ": " << ex.what()
                << std::endl;
    }
  }
  return last_modified_time;
}

void runBuildCommands() {
  fs::file_time_type generatedLastModifiedTime =
      get_folder_last_modified_time("build/Release/generators");
  fs::file_time_type srcLastModified = get_folder_last_modified_time("src");

  if (srcLastModified <= generatedLastModifiedTime) {
    std::cout << "No changes detected in src directory. Skipping build."
              << std::endl;
    return;
  }

  int installResult = system("conan install . --build=missing");
  if (installResult) {
    std::cout << "Conan install failed with error code: " << installResult
              << std::endl;
    std::abort();
  }

  int cmakeResult = system("cd build/Release/generators && cmake ../../..");
  if (cmakeResult) {
    std::cout << "CMake configuration failed with error code: " << cmakeResult
              << std::endl;
    std::abort();
  }

  int buildResult = system("cd build/Release/generators && cmake --build .");
  if (buildResult) {
    std::cout << "Build failed with error code: " << buildResult << std::endl;
    std::abort();
  }
}

int main(int argc, char *argv[]) {
  bool init = false;
  bool run = false;

  std::string runOptions[] = {"--run", "-r", "run"};
  if (argc > 1 && std::find(std::begin(runOptions), std::end(runOptions),
                            std::string(argv[1])) != std::end(runOptions)) {
    run = true;
  }

  runBuildCommands();
  if (run) {
    std::cout << "Running the application..." << std::endl;
    system("./build/Release/generators/cpp_tut");
  }
  return 0;
}
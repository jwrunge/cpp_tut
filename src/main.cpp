#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
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
      // Handle errors, e.g., permission denied for certain files/directories
      std::cerr << "Error accessing " << entry.path() << ": " << ex.what()
                << std::endl;
    }
  }
  return last_modified_time;
}

void runBuildCommands() {
  struct stat generatedStat;
  if (stat("build/Release/generators", &generatedStat) == 0) {
    fs::file_time_type srcLastModified = get_folder_last_modified_time("src");

    auto srcEpochTime = srcLastModified.time_since_epoch();
    auto generatedEpochTime = std::chrono::seconds(generatedStat.st_mtime);
    auto srcSeconds =
        std::chrono::duration_cast<std::chrono::seconds>(srcEpochTime);

    if (srcSeconds <= generatedEpochTime) {
      std::cout << "No changes detected in src directory. Skipping build."
                << std::endl;
      return;
    }
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
  bool run = false;
  if (argc > 1 && std::string(argv[1]) == "--run") {
    run = true;
  }

  runBuildCommands();
  if (run) {
    std::cout << "Running the application..." << std::endl;
    system("./build/Release/generators/cpp_tut");
  }
  return 0;
}
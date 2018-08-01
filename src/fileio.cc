#include <fstream>
#include <sstream>
#include <string>
#include "fileio.h"

std::string file2str(const char *filename)
{
  std::ifstream t(filename);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

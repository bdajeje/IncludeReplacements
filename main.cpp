#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <boost/algorithm/string.hpp>

#include "utils.hpp"

std::vector<std::string> getFilePaths(int argc, char** argv)
{
  if(argc < 1)
    return {};

  std::vector<std::string> results;
  results.reserve(argc - 1);

  for( int i = 1; i < argc; ++i )
    results.push_back( argv[i] );

  return results;
}

/*! Find '#include "..."' sentences and replace them by '#include <...>'
 *  \param filepath - file to update
 */
void update(const std::string& filepath)
{
  // Try to read file
  std::string content;
  if(!utils::file::read(filepath, content))
  {
    std::cerr << "Can't read file: " << filepath << std::endl;
    return;
  }

  // Update line by line
  std::vector<std::string> lines;
  boost::algorithm::split(lines, content, boost::is_any_of("\n"));

  std::string new_content;
  const std::string target_begin {"#include <"};
  const std::string target_end {".hpp>"};
  for( auto& line : lines )
  {
    if( boost::starts_with(line, target_begin) &&
        boost::ends_with(line, target_end) )
    {
      std::cout << "Line: " << line << std::endl;

      std::string include_content = line.substr(target_begin.length());
      boost::algorithm::trim_right(include_content);
      line = "#include <daneel/" + include_content.substr(0, include_content.length() - 1) + ">";

      std::cout << "Replaced by: " << line << std::endl;
    }

    new_content += line + "\n";
  }

  // Override file
  utils::file::write(filepath, new_content);
}

int main(int argc, char** argv)
{
  auto filespath = getFilePaths(argc, argv);
  for( const auto& filepath : filespath )
    update(filepath);

  return EXIT_SUCCESS;
}


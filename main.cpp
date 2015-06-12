#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <boost/algorithm/string.hpp>

bool readFile( const std::string& filepath, std::string& result )
{
  std::ifstream file(filepath.c_str());
  if( !file.is_open() )
    return false;

  // Get length of file
  file.seekg(0, file.end);
  const int length = file.tellg();
  file.seekg(0, file.beg);

  // If file content is empty, exit
  if( length < 1 )
    return true;

  // Read the file
  result.resize( length );
  file.read(&result[0], length);

  // Close the file and return good read
  file.close();
  return true;
}

bool writeFile( const std::string& filepath, const std::string& content )
{
  // Try to open the file
  std::ofstream new_file(filepath, std::ios::trunc);
  if( !new_file.is_open() )
    return false;

  // Write content into
  new_file << content;

  // Closing file
  new_file.close();

  return true;
}

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
  if(!readFile(filepath, content))
  {
    std::cerr << "Can't read file: " << filepath << std::endl;
    return;
  }

  // Update line by line
  std::vector<std::string> lines;
  boost::algorithm::split(lines, content, boost::is_any_of("\n"));

  std::string new_content;
  const std::string target {"#include \""};
  for( auto& line : lines )
  {
    if( boost::starts_with(line, target) )
    {
      std::cout << "Line: " << line << std::endl;

      std::string include_content = line.substr(target.length());
      boost::algorithm::trim_right(include_content);
      line = "#include <" + include_content.substr(0, include_content.length() - 1) + ">";

      std::cout << "Replaced by: " << line << std::endl;
    }

    new_content += line + "\n";
  }

  // Override file
  writeFile(filepath, new_content);
}

int main(int argc, char** argv)
{
  auto filespath = getFilePaths(argc, argv);
  for( const auto& filepath : filespath )
    update(filepath);

  return EXIT_SUCCESS;
}


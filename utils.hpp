#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

namespace utils {
namespace file {

  bool read( const std::string& filepath, std::string& result );
  bool write( const std::string& filepath, const std::string& content );

}
}

#endif // UTILS_HPP


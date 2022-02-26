#ifndef _APPLICATION_EXCEPTION_HPP_
#define _APPLICATION_EXCEPTION_HPP_
#include <stdexcept>

class ApplicationException: public std::runtime_error {
  using std::runtime_error::runtime_error;
};

#endif



#include <string>
#ifndef __ERR_HANDLE__
#define __ERR_HANDLE__

struct err_handler {
  std::string msg;
  explicit err_handler(std::string_view sv) : msg(sv) {}
  std::string_view what() {
    return msg;
  }
};
#endif
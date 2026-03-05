#include "fmt/base.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include <fmt/core.h>
#include <fmt/std.h>

#include <rang.hpp>
template<class T>
concept color_t = requires(){
    std::is_same<T, rang::bg>() or std::is_same<T, rang::bgB>() or std::is_same<T, rang::fg>() or std::is_same<T, rang::fgB>();
};

struct txt_map{
    std::vector<std::vector<wchar_t>> real_map;
    std::variant<rang::fg,rang::fgB> fg;
    std::variant<rang::bg,rang::bgB> bg;
};

template<color_t T>
std::string color_unix(T color){
    return "\033["+fmt::to_string(static_cast<int>(color))+"m";
}

#include "fmt/format.h"
#include <fmt/core.h>
#include <fmt/std.h>
#include <rang.hpp>
template<class...T>
concept is_in_t = (std::is_same_v<T, rang::bg> or ...); //不要()不然就实例化了，就变成纯粹false了

template<class T>
concept color_t = is_in_t<rang::fg,rang::fgB,rang::bgB,rang::bg>;

template<class T>
concept forecolor_t = is_in_t<rang::fg,rang::fgB>;

template<class T>
concept backcolor_t = is_in_t<rang::bg,rang::bgB>;

template<color_t T>
std::string color_unix(T color){
    return "\033["+fmt::to_string(static_cast<int>(color))+"m";
}

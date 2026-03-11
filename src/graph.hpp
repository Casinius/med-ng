#include "event_system/events.hpp"
#include "render_system/colors.hpp"
#include "render_system/linux_term.hpp"
#include "render_system/pipeline.hpp"
#include "render_system/term_asnihack.hpp"

#include <cstdio>
#include <ranges>
#include <string>
#include <vector>
#ifndef __GRAPH_SYSTEM__
#define __GRAPH_SYSTEM__
using namespace std::ranges;
constexpr auto clear_all_str = "\033c";
template<backcolor_t back>
pipeline_renderer::frame form_rectangle(float w_percent, float h_percent, float x_pos, float y_pos,
                                        back bg) {

  using frame = pipeline_renderer::frame;
  frame f;
  constexpr auto col_func = [](int cols, float x_pos, float w_percent, frame& f, back bg) {
    std::string str;
    str.append(color_unix(0));
    for (auto c : views::iota(0, static_cast<int>(cols * x_pos))) {
      str.push_back(' ');
    }

    str.append(color_unix(bg));
    if (cols >=
        (static_cast<int>(cols * x_pos) + static_cast<int>(cols * w_percent))) { // 如果没超出
      auto end_drawnblock = (static_cast<int>(cols * x_pos) + static_cast<int>(cols * w_percent));
      for (auto c : views::iota(static_cast<int>(cols * x_pos), end_drawnblock)) {
        str.push_back(' ');
      }
      str.append(color_unix(0));
      for (auto c : views::iota(end_drawnblock, cols)) {
        str.push_back(' ');
      }
    } else { // 如果超出
      auto end_drawnblock = cols;
      for (auto c : views::iota(static_cast<int>(cols * x_pos), end_drawnblock)) {
        str.push_back(' ');
      }
    }

    f.buffer.emplace_back(str);
    str.clear();
  };
  bool once_flag = false;
  bool once_bflag = false;

  bool void_flag = false;
  auto rows = get_terminal_size_unix()->rows;
  auto cols = get_terminal_size_unix()->cols;
  std::string str;
  str.append(color_unix(0));
  for (auto i : views::iota(0, static_cast<int>(rows * y_pos))) { // 空白
    for (auto c : views::iota(0, static_cast<int>(cols * x_pos))) {
      str.push_back(' ');
    }
    f.buffer.emplace_back(str);
    str.clear();
  }

  if (rows >= (static_cast<int>(rows * y_pos) + static_cast<int>(rows * h_percent))) { // 没超出
    auto end_drawnblock = (static_cast<int>(rows * y_pos) + static_cast<int>(rows * h_percent));
    for (auto i : views::iota(static_cast<int>(rows * y_pos),
                              end_drawnblock)) { // 非空白
      col_func(cols, x_pos, w_percent, f, bg);
    }
    std::string str;
    str.append(color_unix(0));
    for (auto i : views::iota(end_drawnblock,
                              rows)) { // 空白
      for (auto c : views::iota(0, static_cast<int>(cols * x_pos))) {
        str.push_back(' ');
      }
      f.buffer.emplace_back(str);
      str.clear();
    }

  } else { // 超出
    for (auto i : views::iota(static_cast<int>(rows * y_pos),
                              rows)) { // 非空白
      col_func(cols, x_pos, w_percent, f, bg);
    }
  }

  return f;
}

#endif


#include "colors.hpp"
#include "fmt/base.h"
#include "linux_term.hpp"
#include "pipeline.hpp"
#include "rang.hpp"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <ranges>
#include <thread>
#include <vector>

#include "term_asnihack.hpp"

using frame = pipeline_renderer::frame;
using namespace std::ranges;
template <backcolor_t back>
frame form_rectangle(float w_percent, float h_percent, back bg) {
  frame f;
  f.buffer.emplace_back(color_unix(bg));
  auto height = static_cast<int>(h_percent * get_terminal_size_unix()->rows);
  for (auto i : views::iota(1, height)) {
    fmt::println("loop_h:{}", height);
    auto line_length =
        static_cast<int>(get_terminal_size_unix()->cols * w_percent);
    std::string str;
    for (auto c : views::iota(1, line_length)) {
      fmt::println("loop_w:{}", line_length);
      str.push_back(' ');
    }
    f.buffer.emplace_back(str);
    str.clear();
  }
  return f;
}

int main(int argc, char **argv) {

  pipeline_renderer pip;
  into_backup_screen();
  std::thread t1(
      [](pipeline_renderer &pip) {
        pipeline_renderer::frame f;
        f.buffer = {color_unix(rang::bgB::red), " ", "-", " "};
        auto f2 = form_rectangle(0.8, 0.9, rang::bgB::blue);
        time_control([](pipeline_renderer& pip,frame& f,frame& f2){
          pip.write(std::move(f));
          pip.write(std::move(f2));
        }, std::chrono::milliseconds(2),pip,f,f2);
        
      },
      std::ref(
          pip)); // thread中 假如使用了lambda,参数不会自动转换为ref,需要手动转换
  if (t1.joinable()) {
    t1.join();
  } else {
    t1.detach();
  }

  while (1) {

    time_control(
        [](pipeline_renderer &pip) {
          pip.submit_render(
              0,
              [](pipeline_renderer::frame frame) {
                // std::cout<<"render\n";
                auto x = get_terminal_size_unix()->rows;
                hide_cursor();

                atomic_submit();
                for (size_t i = 0; i < static_cast<size_t>(x); ++i) {
                  move_cursor(i, 0);
                  if (frame.status == pipeline_renderer::stage::ready) {
                    if (i < frame.buffer.size()) {
                      fmt::print("{}", frame.buffer[i]);
                    } else {
                      fmt::print("{}", color_unix(0));
                    }
                  }
                }
                atomic_flush();
                return std::chrono::steady_clock::now();
              },
              std::chrono::milliseconds(15));

          pip.submit_render(
              1,
              [](pipeline_renderer::frame frame) {
                // std::cout<<"render\n";
                auto x = get_terminal_size_unix()->rows;
                hide_cursor();

                atomic_submit();
                for (size_t i = 0; i < static_cast<size_t>(x); ++i) {
                  move_cursor(i, 0);
                  if (frame.status == pipeline_renderer::stage::ready) {
                    if (i < frame.buffer.size()) {
                      fmt::print("{}", frame.buffer[i]);
                    } else {
                      fmt::print("{}", color_unix(0));
                    }
                  }
                }
                atomic_flush();
                return std::chrono::steady_clock::now();
              },
              std::chrono::milliseconds(15));
        },
        std::chrono::milliseconds(18), pip);
  }

  return 0;
}

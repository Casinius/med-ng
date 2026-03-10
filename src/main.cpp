

#include "colors.hpp"
#include "events.hpp"
#include "fmt/base.h"
#include "linux_term.hpp"
#include "pipeline.hpp"
#include "rang.hpp"
#include "term_asnihack.hpp"
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <ranges>
#include <thread>
#include <vector>

using frame = pipeline_renderer::frame;
using namespace std::ranges;
template <backcolor_t back>
frame form_rectangle(float w_percent, float h_percent, back bg) {
  frame f;
  f.buffer.emplace_back(color_unix(bg));
  auto height = static_cast<int>(h_percent * get_terminal_size_unix()->rows);
  for (auto i : views::iota(1, 1 + height)) {
    
    auto line_length =
        static_cast<int>(get_terminal_size_unix()->cols * w_percent);
    std::string str;
    for (auto c : views::iota(1, 1 + line_length)) {
      
      
      str.push_back(' ');
    }
    f.buffer.emplace_back(str);
    str.clear();
  }
  return f;
}

int main(int argc, char **argv) {

  
  

  auto pipeline_main = std::make_shared<pipeline_renderer>();
  
  into_backup_screen();
  std::thread t1(
      [](std::shared_ptr<pipeline_renderer> pip) {
        
        pipeline_renderer::frame f;
        f.buffer = {color_unix(rang::bgB::red), " ", "-", " "};
        auto f2 = form_rectangle(0.97, 0.2, rang::bgB::blue);
        pip->write(f2);
        

        while (1) {
          
          f2 = form_rectangle(0.97, 0.2, rang::bgB::blue);
          std::fflush(stdout);
          time_control(
              [](auto pip, frame &f, frame &f2) {
                pip->write(f);
                pip->write(f2);
                
              },
              std::chrono::milliseconds(1), pip, f, f2);
        }
      },
      pipeline_main); 
                      

  
  
  
  t1.detach();
  
  /*
  auto f2 = form_rectangle(0.27, 0.2, rang::bgB::blue);
  pipeline_main->write(std::move(f2));
  */

  while (1) {

    time_control(
        [](std::shared_ptr<pipeline_renderer> pip) {
          std::fflush(stdout);
          pip->submit_render(
              [](const pipeline_renderer::frame &frame) {
                
                auto x = get_terminal_size_unix()->rows;
                hide_cursor();

                atomic_submit();
                for (size_t i = 0; i < static_cast<size_t>(x); ++i) {
                  move_cursor(i, 0);
                  if (i < frame.buffer.size()) {
                    fmt::print("{}", frame.buffer[i]);
                  } else {
                    fmt::print("{}", color_unix(0));
                  }
                }
                atomic_flush();
                return std::chrono::steady_clock::now();
              },
              std::chrono::milliseconds(15));
          std::fflush(stdout);
        },
        std::chrono::milliseconds(18), pipeline_main);
  }
  
  

  return 0;
}

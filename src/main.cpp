

#include "colors.hpp"
#include "fmt/base.h"
#include "linux_term.hpp"
#include "rang.hpp"
#include <chrono>
#include <cstddef>
#include <vector>
#include "pipeline.hpp"

#include "term_asnihack.hpp"

using frame =  pipeline_renderer::frame;

template <forecolor_t fore, backcolor_t back>
void form_rectangle(){
  frame f;
  for(auto& i:f.buffer){
    for(auto& c:{1,2,3,4}){
      
    }
  }
}
 

int main(int argc, char **argv) {

  pipeline_renderer pip;
  
  while (1) {

  time_control(
      [](pipeline_renderer &pip) {

        pipeline_renderer::frame f;
        f.buffer = {color_unix(rang::bgB::red), " ", "-", " "};
        pip.write(std::move(f));
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
      },
      std::chrono::milliseconds(18), pip);
  }

  return 0;
}



#include "event_system/events.hpp"
#include "fmt/base.h"
#include "graph.hpp"
#include "rang.hpp"
#include "render_system/colors.hpp"
#include "render_system/linux_term.hpp"
#include "render_system/pipeline.hpp"
#include "render_system/term_asnihack.hpp"

#include <chrono>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <thread>
#include <vector>
using frame = pipeline_renderer::frame;

void run_render(std::shared_ptr<events> event, std::shared_ptr<pipeline_renderer> pipeline_main) {
  while (true) {
    
    if (std::static_pointer_cast<render_ev>(event)->event_checkonce()) {
      std::fflush(stdout);
      clear_remain_color();
      pipeline_main->submit_render(
          [](const pipeline_renderer::frame& frame) {
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
          std::chrono::milliseconds(0));
      std::fflush(stdout);
    }
    
    
    time_control(
        [](std::shared_ptr<pipeline_renderer> pip) {
          std::fflush(stdout);
          pip->submit_render(
              [](const pipeline_renderer::frame& frame) {
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
              std::chrono::milliseconds(10));
          std::fflush(stdout);
        },
        std::chrono::milliseconds(13),false, pipeline_main);
  }
}

int main(int argc, char** argv) {
  auto pipeline_main = std::make_shared<pipeline_renderer>();
  auto events_main = std::make_shared<events>();
  into_backup_screen();
  std::thread t1(
      [](std::shared_ptr<pipeline_renderer> pip, auto ev_main) {
        pipeline_renderer::frame f;
        f.buffer = {color_unix(rang::bgB::red), " ", "-", " "};
        auto f2 = form_rectangle(0.2, 0.2, 0.1, 0.1, rang::bgB::blue);
        pip->write(f2);

        while (true) {

          f2 = form_rectangle(0.2, 0.2, 0.1, 0.1, rang::bgB::blue);

          time_control(
              [](std::shared_ptr<pipeline_renderer> pip, frame& f, frame& f2) {
                pip->write(f);
                pip->write(f2);
              },
              std::chrono::milliseconds(5),false, pip, f, f2);
        }
      },
      pipeline_main, events_main);

  std::thread t2(
      [](auto ev) {
        std::static_pointer_cast<render_ev>(ev)->event_launch(std::chrono::milliseconds(6));
      },
      events_main);
  t2.detach();
  t1.detach();

  run_render(events_main, pipeline_main);
  return 0;
}

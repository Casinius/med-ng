#include "../err_handle.hpp"
#include "fmt/core.h"

#include <chrono>
#include <expected>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#ifndef __PIPLINE__
#define __PIPLINE__
struct pipeline_renderer {
  enum class stage { idle, ready, rendered };
  struct frame {

    std::vector<std::string> buffer;
  };

  frame write_frame;
  frame render_frame;

  std::chrono::steady_clock clk;

  std::mutex mtx_;

public:
  std::expected<bool, err_handler> swap() {
    std::lock_guard lg(mtx_);
    render_frame.buffer = std::move(write_frame.buffer);
    return true;
  }
  void write(frame& f) {
    std::lock_guard lg(mtx_);
    write_frame.buffer = std::move(f.buffer);
  }

  void submit_render(auto render_fn, std::chrono::steady_clock::duration dur_deadline) {

    {
      swap().value_or(false);
    }
    std::chrono::steady_clock::time_point before_sub = clk.now();
    decltype(clk)::time_point new_point = render_fn(render_frame);
    std::chrono::steady_clock::duration dur = new_point - before_sub;

    if (dur < dur_deadline) {
      std::this_thread::sleep_for(dur_deadline - dur);
    }
  }
};

void time_control(auto fn, std::chrono::steady_clock::duration duration_deadline, auto&&... args) {
  std::chrono::steady_clock clk;
  std::chrono::steady_clock::duration dura;
  std::chrono::steady_clock::time_point before_sub = clk.now();
  fn(args...);
  std::chrono::steady_clock::time_point after = clk.now();
  dura = after - before_sub;

  if (dura < duration_deadline) {
    std::this_thread::sleep_for(duration_deadline - dura);
  }
}
#endif
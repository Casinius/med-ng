#include "fmt/core.h"
#include <chrono>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <thread>
#include <utility>

struct pipeline_renderer {
  enum class stage { idle,ready,rendered };
  struct frame {
 
    std::vector<std::string> buffer;

    stage status = stage::idle;

  };

  std::array<frame, 2> frames_; // 双缓冲
  std::chrono::steady_clock clk;
  
  
  std::mutex mtx_;
public:
  bool write(frame &&f) {
    {
      std::lock_guard<std::mutex> lg(mtx_);
      for (auto &slot : frames_) {
        
        if (slot.status == stage::idle) {
          slot.buffer = std::move(f.buffer);
          slot.status = stage::ready;
          return true; // 成功提交
        }
      }
    }
    return false; // 槽满，丢弃帧（或阻塞等待）
  }

  // 阻塞
  void submit_render(uint frame_idx, auto render_fn,std::chrono::steady_clock::duration dur_deadline) {
    std::lock_guard<std::mutex> lg(mtx_);
    if (frame_idx != 0 and frame_idx != 1) {
      return;
    }
    std::chrono::steady_clock::time_point before_sub = clk.now();
    // std::packaged_task<std::chrono::steady_clock::time_point(frame)>
    // task(render_fn);
    decltype(clk)::time_point new_point = render_fn(frames_[frame_idx]);
    std::chrono::steady_clock::duration dur = new_point - before_sub;
    frames_[frame_idx].status=stage::rendered;
    if (dur < dur_deadline){
      std::this_thread::sleep_for(dur_deadline-dur);
    }
    frames_[frame_idx].status=stage::idle;
  }
};

void time_control(auto fn,
                       std::chrono::steady_clock::duration duration_deadline,auto&&...args) {
  std::chrono::steady_clock clk;
  std::chrono::steady_clock::duration dura;
  std::chrono::steady_clock::time_point before_sub = clk.now();
  fn(args...);
  std::chrono::steady_clock::time_point after = clk.now();
  dura = after - before_sub;
  
  if(dura < duration_deadline) {
    std::this_thread::sleep_for(duration_deadline-dura);
  }
}
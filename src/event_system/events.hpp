
#include "../render_system/linux_term.hpp"
#include "../render_system/pipeline.hpp"
#include "fmt/base.h"

#include <chrono>
#include <condition_variable>
#include <fcntl.h>
#include <linux/input.h>
#include <mutex>
#include <stdlib.h>
#include <string>
#include <thread>
#include <unistd.h>
#ifndef __EVENTS__
#define __EVENTS__
struct event_linux_input {
  int fd;
  input_event ev;
  inline static std::once_flag global_sign;
  std::string event_name;
  explicit event_linux_input(std::string ev_name) : event_name(ev_name) {
    this->fd = open(event_name.c_str(), O_RDONLY);
    if (fd == -1) {
      perror("open failed");
    }
  }
  auto ev_loop() {

    ssize_t n = read(fd, &ev, sizeof(ev));
    if (n == -1) {
      perror("read failed");
      close(fd);
      return false;
    }

    // 仅打印非同步事件（SYN_REPORT 无实际数据）
    if (ev.type != EV_SYN) {
      printf("time: %ld.%06ld, type: %d, code: %d, value: %d\n", ev.time.tv_sec, ev.time.tv_usec,
             ev.type, ev.code, ev.value);
    }
    return true;
  }
  ~event_linux_input() {
    if (fd != -1) {
      close(this->fd);
    }
  }
};
struct events {
  std::mutex mtx_;
  std::condition_variable cv_;
  bool event_checkonce(auto...args);
  void event_launch(auto...args);
};


struct key_ev : events {
  
};
struct render_ev : events {
  bool is_ok{false};
  bool event_checkonce() {
    auto tmp = is_ok;
    {
      std::unique_lock lck(mtx_);
      is_ok = false;
    }
    return tmp;
  }

  void event_launch(std::chrono::milliseconds sleep_sec) {
    int tmp_h = get_terminal_size_unix()->rows;
    int tmp_w = get_terminal_size_unix()->cols;
    while (true) {

      while (true) {

        if (tmp_h != get_terminal_size_unix()->rows || tmp_w != get_terminal_size_unix()->cols) {
          tmp_h = get_terminal_size_unix()->rows;
          tmp_w = get_terminal_size_unix()->cols;
          break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_sec)); // 睡觉的时候不要握锁
      }
      {
        std::unique_lock lock(mtx_);
        is_ok = true;
      }

    }
  }
};
#endif

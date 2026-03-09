
#include <fcntl.h>
#include <linux/input.h>
#include <mutex>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unistd.h>
#ifndef __EVENTS__
#define __EVENTS__
struct event_linux_input {
  int fd;
  input_event ev;
  std::once_flag global_sign;

  event_linux_input(std::string event_name) {
    std::call_once(global_sign, [this, &event_name]() {
      this->fd = open(event_name.c_str(), O_RDONLY);
      if (fd == -1) {
        perror("open failed");
        return false;
      }
      return true;
    });
  }
  auto ev_loop() {
    while (1) {
      ssize_t n = read(fd, &ev, sizeof(ev));
      if (n == -1) {
        perror("read failed");
        close(fd);
        return false;
      }

      // 仅打印非同步事件（SYN_REPORT 无实际数据）
      if (ev.type != EV_SYN) {
        printf("time: %ld.%06ld, type: %d, code: %d, value: %d\n",
               ev.time.tv_sec, ev.time.tv_usec, ev.type, ev.code, ev.value);
      }
    }
  }
  ~event_linux_input() { close(this->fd); }
};
#endif

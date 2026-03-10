#include "fmt/base.h"
#include "fmt/core.h"
#ifndef __TERM_HACK__
#define __TERM_HACK__
inline void move_cursor(uint y, uint x) {
  fmt::print("\033[{};{}H", fmt::to_string(y), fmt::to_string(x));
}

inline void clear_remain_color() {
  fmt::print("\033[2J"); // 清屏，但**不重置颜色**！
}
inline void reset_cursor() {
  fmt::print("\033[H"); // 复位光标
}
inline void hide_cursor() {
  fmt::print("\033[?25l");
}
inline void into_backup_screen() {
  fmt::print("\033[?1049h"); // 进入备用屏幕 不会闪烁 不会增加行数 不要多次进入不然会闪
}
inline void atomic_submit() {
  fmt::print("\033[?2026h");
}
inline void atomic_flush() {
  fmt::print("\033[?2026l");
}
inline void clear_all(){
  fmt::print("\033c");
}
#endif
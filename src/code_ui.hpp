#ifndef __CODEUI__
#define __CODEUI__
#include "cbase.h"

inline constexpr auto optwin_code() {
  ftxui::WindowOptions win_opts_code;
  win_opts_code.title = "Editor";
  // win_opts_code.left = 0;
  // win_opts_code.top = 0;
  win_opts_code.width = 200;
  win_opts_code.height = 200;
  return win_opts_code;
}
inline constexpr auto optwin_proj() {
  ftxui::WindowOptions win_opts_proj;
  win_opts_proj.title = "project";
  // win_opts_proj.left = 0;
  // win_opts_proj.top = 0;
  // win_opts_proj.width = 20;
  win_opts_proj.height = 200;
  return win_opts_proj;
}
#endif
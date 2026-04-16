
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/mouse.hpp"
#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/flexbox_config.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/dom/selection.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/util/ref.hpp"

#include <chrono>
#include <climits>
#include <cstdio>
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <tuple>
#include <vector>

using namespace ftxui;
ButtonOption CompactButtonStyle() {
  ButtonOption opt;
  opt.transform = [](const EntryState& s) {
    auto element = text(s.label);

    if (s.active) {
      element = element | bold;
    }

    if (s.focused) {
      element = element | bgcolor(Color::Grey23);
    }

    // 紧凑内边距
    return hbox({text(" "), element, text(" ")});
  };
  return opt;
}
auto change_directory(std::vector<std::filesystem::path>& path_list, std::vector<std::string>& vsr,
                      std::filesystem::path& pth) {

  std::filesystem::directory_iterator list(pth);
  vsr.clear();
  path_list.clear();
  path_list.push_back("../");
  vsr.push_back("../");
  for (auto x : list) {
    if (x.is_directory()) {
      std::filesystem::path np = "▶ ";
      std::filesystem::path fn = x.path().filename();

      vsr.emplace_back(np.string() + fn.string());
    }
    if (x.is_regular_file()) {
      vsr.emplace_back(x.path().filename());
    }
    path_list.emplace_back(x);
  }
}

auto file_manager(std::vector<std::filesystem::path>& path_list, std::vector<std::string>& vsr,
                  int& selection, std::filesystem::path& pth) {
  std::filesystem::directory_iterator list(pth);
  path_list.push_back("../");
  vsr.push_back("../");
  for (auto x : list) {
    if (x.is_directory()) {
      std::filesystem::path np = "▶ ";
      std::filesystem::path fn = x.path().filename();

      vsr.emplace_back(np.string() + fn.string());
    }
    if (x.is_regular_file()) {
      vsr.emplace_back(x.path().filename());
    }
    path_list.emplace_back(x.path());
  }
  return Menu(&vsr, &selection);
}

int main() {
  auto screen = ScreenInteractive::FullscreenAlternateScreen();

  // 创建一个可拖拽窗口
  WindowOptions win_opts_proj;
  win_opts_proj.title = "project";
  // win_opts_proj.left = 0;
  // win_opts_proj.top = 0;
  // win_opts_proj.width = 20;
  win_opts_proj.height = 200;

  WindowOptions win_opts_code;
  win_opts_code.title = "Editor";
  // win_opts_code.left = 0;
  // win_opts_code.top = 0;
  win_opts_code.width = 200;
  win_opts_code.height = 200;
  int selected_obj = 0;

  auto window_code = Window(win_opts_code);
  int proj_selection = 0;

  std::vector<std::string> entries = {};
  std::vector<std::filesystem::path> pth_list = {};
  std::filesystem::path cur_path = "/home/cyan/文档/micro-ng";
  std::filesystem::path maxp_path = cur_path;
  auto window_proj = file_manager(pth_list, entries, proj_selection, cur_path);
  auto window_proj_r = Renderer(window_proj, [&] {
    return window_proj->Render() | center | frame // 添加滚动框架
           | focus;                               // 确保焦点在菜单上
  });
  auto dyn_window_proj = CatchEvent(window_proj_r, [&](Event event) {
    if (event == Event::Return) {
      auto p_path = pth_list.at(proj_selection);

      if (std::filesystem::exists(p_path)) {
        if (std::filesystem::is_directory(p_path)) {
          if (proj_selection != 0) {
            change_directory(pth_list, entries, p_path);
            cur_path = p_path;
          } else {

            auto p_path_upper = cur_path.parent_path();

            if (cur_path.compare(maxp_path)) {
              change_directory(pth_list, entries, p_path_upper);
              cur_path = p_path_upper;
            }
          }
        }
      }

      return true;
    }
    return false;
  });
  bool global_isshow_dropdown = false;

  WindowOptions win_opts_dropdown;
  win_opts_dropdown.title = "";
  win_opts_dropdown.left = 0;
  win_opts_dropdown.top = 0;
  win_opts_dropdown.width = 10;
  win_opts_dropdown.height = 20;

  auto file_button = Button(
      "file",
      [&] {
        global_isshow_dropdown = !global_isshow_dropdown;
      },
      CompactButtonStyle());

  auto file_dropdown = Window(win_opts_dropdown);

  auto top_bar_options = Container::Horizontal(
      {file_button, Button("exit", screen.ExitLoopClosure(), CompactButtonStyle())});

  int size = 20;

  auto repaint_window_proj = Renderer(dyn_window_proj, [&] {
    return window(text("Project"), hbox({dyn_window_proj->Render()})) | flex_shrink;
  });

  auto layout_main = ResizableSplitLeft(repaint_window_proj, window_code, &size);
  int dropdown_selection = 0;
  auto dyn_dropdown = Container::Horizontal({
      Button(
          "open folder..", []() {}, CompactButtonStyle()),
      Button(
          "open file", []() {}, CompactButtonStyle()),
  });
  auto layout = Container::Vertical({
      top_bar_options,
      dyn_dropdown | Maybe(&global_isshow_dropdown),
      layout_main,
  });

  screen.Loop(layout);
  return 0;
}
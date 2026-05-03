#include "cbase.h"
#include "code_ui.hpp"
#include "framework.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/screen/screen.hpp"

#include <filesystem>
#include <vector>

auto builtin_core_editor(ftxui::ScreenInteractive&& screen){

  auto window_code = Window(optwin_code());
  int proj_selection = 0;

  std::vector<std::string> entries = {};
  std::vector<std::filesystem::path> pth_list = {};
  std::filesystem::path cur_path = "/home/cyan/";
  std::filesystem::path maxp_path = cur_path;
  auto window_proj = file_manager(pth_list, entries, proj_selection, cur_path);
  auto window_proj_r = Renderer(window_proj, [&] {
    return window_proj->Render() | ftxui::center | ftxui::frame // 添加滚动框架
           | ftxui::focus;                               // 确保焦点在菜单上
  });
  auto dyn_window_proj = CatchEvent(window_proj_r, [&](ftxui::Event event) {
    if (event == ftxui::Event::Return) {
      auto p_path = pth_list.at(proj_selection);

      if (std::filesystem::exists(p_path)) {
        if (std::filesystem::is_directory(p_path)) {
          if (proj_selection == 0) {

            bool is_change = is_under_equivalent(cur_path, maxp_path);
            if (is_change && !std::filesystem::equivalent(cur_path,maxp_path)) {
              change_directory(pth_list, entries, cur_path.parent_path());
              cur_path = cur_path.parent_path();
            }
          } else {

            change_directory(pth_list, entries, p_path);
            cur_path = p_path;
          }
        }
      }

      return true;
    }
    return false;
  });
  bool global_isshow_dropdown = false;

  auto file_button = Button(
      "file",
      [&] {
        global_isshow_dropdown = !global_isshow_dropdown;
      },
      CompactButtonStyle());

  auto file_dropdown = Window(optwin_dropdown());

  auto top_bar_options = ftxui::Container::Horizontal(
      {file_button, Button("exit", screen.ExitLoopClosure(), CompactButtonStyle())});

  int size = 20;

  auto repaint_window_proj = Renderer(dyn_window_proj, [&] {
    return window(ftxui::text("Project"), ftxui::hbox({dyn_window_proj->Render()})) | ftxui::flex_shrink;
  });

  auto layout_main = ResizableSplitLeft(repaint_window_proj, window_code, &size);
  int dropdown_selection = 0;

  std::vector<std::string> extfm_entries = {};
  std::vector<std::filesystem::path> extfm_pathlist = {};
  int extfm_selection = 0;
  std::filesystem::path extfm_default_path = "/home";
  auto extern_fm = file_manager(extfm_pathlist, extfm_entries, extfm_selection, extfm_default_path);
  auto dyn_dropdown = ftxui::Container::Horizontal({
      Button(
          "open folder..",
          [&]() {
            auto screen_extfm = ftxui::ScreenInteractive::FullscreenAlternateScreen();
            auto vCon = ftxui::Container::Vertical(
                {Button("exit", screen.ExitLoopClosure(), CompactButtonStyle()), extern_fm});
            screen_extfm.Loop(vCon);
          },
          CompactButtonStyle()),
      Button(
          "open file",
          []() {

          },
          CompactButtonStyle()),
  });
  auto layout = ftxui::Container::Vertical({
      top_bar_options,
      dyn_dropdown | ftxui::Maybe(&global_isshow_dropdown),
      layout_main,
  });
  return layout;
  
}
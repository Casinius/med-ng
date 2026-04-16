
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/flexbox_config.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/dom/selection.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/util/ref.hpp"

#include <chrono>
#include <climits>
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
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

auto file_manager(std::vector<std::string>& vsr,int& selection,std::filesystem::path pth){
  std::filesystem::directory_iterator list(pth);
  for (auto x:list) {
    if(x.is_directory()){
      vsr.emplace_back(x.path().filename());
    }
    if (x.is_regular_file()) {
      vsr.emplace_back(x.path().filename());
    }
  }
  return Menu(&vsr,&selection);
} 

int main() {
  auto screen = ScreenInteractive::FullscreenAlternateScreen();

  // 创建一个可拖拽窗口
  WindowOptions win_opts_proj;
  win_opts_proj.title = "project";
  //win_opts_proj.left = 0;
  //win_opts_proj.top = 0;
  //win_opts_proj.width = 20;
  win_opts_proj.height = 200;
  
  WindowOptions win_opts_code;
  win_opts_code.title = "editor";
  //win_opts_code.left = 0;
  //win_opts_code.top = 0;
  win_opts_code.width = 200;
  win_opts_code.height = 200;
  int selected_obj = 0;
  
  auto window_code = Window(win_opts_code);
  int proj_selection = 0;

  std::vector<std::string> entries = {
  };
  auto window_proj = file_manager(entries, proj_selection,"/home/cyan");
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
  auto layout_main = ResizableSplitLeft(window_proj,window_code, &size);
  
  
  
  
  auto layout = Container::Vertical({
    top_bar_options,
    layout_main,
  });
  
  screen.Loop(layout);
  return 0;
}
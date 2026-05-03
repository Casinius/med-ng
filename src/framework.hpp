#include "cbase.h"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/screen.hpp"

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <iterator>
#include <memory>
#include <mio/mmap.hpp>
#include <numeric>
#include <print>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>
#ifndef __FRAMEWORK__
#define __FRAMEWORK__
inline ftxui::ButtonOption CompactButtonStyle() {
  ftxui::ButtonOption opt;
  opt.transform = [](const ftxui::EntryState& s) {
    auto element = ftxui::text(s.label);

    if (s.active) {
      element = element | ftxui::bold;
    }

    if (s.focused) {
      element = element | ftxui::bgcolor(ftxui::Color::Grey23);
    }

    // 紧凑内边距
    return ftxui::hbox({ftxui::text(" "), element, ftxui::text(" ")});
  };
  return opt;
}
inline auto change_directory(std::vector<std::filesystem::path>& path_list,
                             std::vector<std::string>& vsr, std::filesystem::path pth) {

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

inline auto file_manager(std::vector<std::filesystem::path>& path_list,
                         std::vector<std::string>& vsr, int& selection,
                         std::filesystem::path& pth) {
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
  return ftxui::Menu(&vsr, &selection);
}

inline constexpr auto optwin_dropdown() {
  ftxui::WindowOptions win_opts_dropdown;
  win_opts_dropdown.title = "";
  win_opts_dropdown.left = 0;
  win_opts_dropdown.top = 0;
  win_opts_dropdown.width = 10;
  win_opts_dropdown.height = 20;
  return win_opts_dropdown;
}
bool is_under_equivalent(const std::filesystem::path& child, const std::filesystem::path& parent) {
  std::error_code ec;
  std::filesystem::path abs_child = std::filesystem::weakly_canonical(child);

  for (std::filesystem::path p = abs_child; !p.empty(); p = p.parent_path()) {
    if (std::filesystem::equivalent(p, parent, ec)) {
      return true;
    }
    if (ec) {
      ec.clear(); // 忽略不存在等错误，继续向上
    }
    if (p == p.parent_path()) {
      break;
    }
  }
  return false;
}

namespace text_editor {
int mio_errhandle(const std::error_code& error) {
  const auto& errmsg = error.message();
  std::println("error mapping file {},exiting...",errmsg.c_str());
  return error.value();
}
struct Document_FSLayer {
  mio::mmap_sink mmap;
  auto map_file(std::filesystem::path pth){
    std::error_code error;
    this->mmap = mio::make_mmap_sink(pth.c_str(), 0, mio::map_entire_file, error);
    if (error) {
      return mio_errhandle(error);
    }

  }
  void insert(std::string_view sv,auto begin){
    
  }
  ~Document_FSLayer(){
    this->mmap.unmap();
  }
};

struct Document_Vis{
  using line_t = uint;
  std::vector<std::pair<line_t,std::string>> rw_buf;
  std::vector<std::string> list_diagnostic;
  std::vector<std::string> list_recommand;
  Document_Vis(){
    
  }
  ~Document_Vis(){

  }
  
};

struct Editor_Component : public ftxui::ComponentBase{

  std::shared_ptr<ftxui::Element> file_;
  Document_Vis dvis;
  Editor_Component(){

  }
  ftxui::Element Render() {

  }
  ~Editor_Component(){

  }
};

} // namespace text_editor
#endif
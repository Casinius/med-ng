#ifndef __FRAME_BATCH__
#define __FRAME_BATCH__
#include "pipeline.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <mutex>
#include <ranges>
#include <vector>
namespace views = std::ranges::views;
pipeline_renderer::frame frame_batch(std::vector<pipeline_renderer::frame> flist) {
  pipeline_renderer::frame resf;
  bool once_flag = false;
  if (once_flag == false) {
    for (auto idx : views::iota(static_cast<size_t>(0), flist.at(0).buffer.size() - 1)) {
        resf.buffer.emplace_back(flist[0].buffer[idx]);
    }
    once_flag = true;
  }
  
  for (auto idx : views::iota(static_cast<size_t>(1), flist.size() - 1)){
    for (auto idx_row:views::iota(static_cast<size_t>(0), std::max(flist.at(idx).buffer.size(),resf.buffer.size())-1)) {
        if (resf.buffer.size() > flist.at(idx).buffer.size()) {
            if (resf.buffer == flist.at(idx).buffer) {
            
            }else{
                
            }
        }
    }
  }
  return resf;
}

#endif
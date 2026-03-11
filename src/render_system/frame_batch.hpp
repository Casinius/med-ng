#ifndef __FRAME_BATCH__
#define __FRAME_BATCH__
#include "pipeline.hpp"
#include <vector>

pipeline_renderer::frame frame_batch(std::vector<pipeline_renderer::frame> flist){
    pipeline_renderer::frame resf;
    for (auto& i:flist) {
        for (auto& e:i.buffer) {
            
        }
    }
}

#endif
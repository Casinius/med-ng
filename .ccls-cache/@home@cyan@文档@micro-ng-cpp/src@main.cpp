

#include <concepts>
#include <type_traits>
#include <variant>
#include <vector>
#include "linux_term.hpp"
#include "colors.hpp"
#include "rang.hpp"
struct draw_rectangle{
    float w_percent{0};
    float y_percent{0};

};


struct term_ui{
    uint width{0};
    uint height{0};
    template<typename Fn,typename...Args>
    void run(Fn callable,Args...arg){
        while (1) {
            width=get_terminal_size_unix()->cols;
            height=get_terminal_size_unix()->rows;
            callable(width,height,arg...);
        }
    }
};

int main(int argc, char **argv) {
    
    fmt::print("{}{}",color_unix(rang::bgB::red),color_unix(rang::fgB::blue));
    term_ui ui;
    
    ui.run([](uint w,uint h,int a){
        
    });
    return 0;
}

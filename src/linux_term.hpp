#include "base_macro.h"

#ifdef OS_LINUX
#include <sys/ioctl.h>
#include <unistd.h>
#include <optional>
#include <utility>

struct TerminalSize {
    int rows;
    int cols;
};

std::optional<TerminalSize> get_terminal_size_unix() {
    struct winsize w;
    
    // STDOUT_FILENO = 1, 也可用 STDERR_FILENO (2) 或 stdin (0)
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return TerminalSize{w.ws_row, w.ws_col};
    }
    return std::nullopt;
}
#endif
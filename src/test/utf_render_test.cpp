#include "../cbase.h"
using namespace ftxui;
Element RenderHighlightedLine(const std::string& line,
                              const std::vector<std::tuple<size_t, size_t, Color>>& tokens)
{
    Elements segments;
    size_t pos = 0;

    for (const auto& [start, end, col] : tokens) {
        // 普通文本段
        if (start > pos) {
            segments.push_back(text(line.substr(pos, start - pos)));
        }
        // 高亮 token 段 —— substr 返回 std::string，无歧义
        segments.push_back(text(line.substr(start, end - start)) | color(col));
        pos = end;
    }
    // 尾部普通文本
    if (pos < line.size()) {
        segments.push_back(text(line.substr(pos)));
    }

    return hbox(std::move(segments));
}

int main()
{
    // "auto 测试 = 123;" 的 UTF-8 字节布局：
    // a u t o [space] 测(3B) 试(3B) [space] = [space] 1 2 3 ;
    // 0 1 2 3   4      5 6 7   8 9 10  11    12  13    14...
    std::string code = "auto 测试 = 123;";

    std::vector<std::tuple<size_t, size_t, Color>> tokens = {
        {0,  4,  Color::Blue},   // "auto"
        {5,  11, Color::Cyan},   // "测试" (5~10, 共6字节)
        {14, 17, Color::Green},  // "123"
    };

    auto document = vbox({
        RenderHighlightedLine(code, tokens),
        text(""),  // 空行
        text("按回车退出") | color(Color::GrayDark),
    });

    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    return 0;
}
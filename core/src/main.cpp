#include "App.hpp"
#include <print>

int main(int argc, char** argv) {
    constexpr int WINDOW_WIDTH{800};
    constexpr int WINDOW_HEIGHT{600};
    try {
        App app(WINDOW_WIDTH, WINDOW_HEIGHT, "Tannery");
        app.run();
    } catch (const std::exception& e) {
        std::println(stderr, "Fatal error: {}", e.what());
        return 1;
    }
    return 0;
}

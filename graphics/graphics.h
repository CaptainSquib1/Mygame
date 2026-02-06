#pragma once

#include <SDL3/SDL.h>
#include <string>

class Color {
public:
    int red{255}, green{0}, blue{0}, alpha{255};
};

class Graphics {
public:
    Graphics(const std::string& title, int window_width, int window_height);
    void clear();
    void draw(const SDL_FRect& rect, const Color& color);
    void update();

    const int width, height;

private:
    std::string title;

    SDL_Window* window;
    SDL_Renderer* renderer;
};
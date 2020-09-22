#include "fps_counter.hpp"

unsigned int FpsCounter::frames = 0;
unsigned int FpsCounter::fps = 0;
sf::Clock FpsCounter::clock;


void FpsCounter::Update(void) {
    if (clock.getElapsedTime().asSeconds() > 1.0f) {
            fps = frames;
            frames = 0;
            clock.restart();
        }
    frames++;
}
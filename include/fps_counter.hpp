#pragma once
#include <SFML/System.hpp>

class FpsCounter {

private:
	FpsCounter() {};
	static unsigned int frames;
	static unsigned int fps;
	static sf::Clock clock;

public:

    static void Update(void);
    static unsigned int GetFps(void) { return fps; };
    
};
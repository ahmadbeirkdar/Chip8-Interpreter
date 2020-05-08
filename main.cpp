#include "Chip8.h"
#include "Platform.hpp"
#include <iostream>
#include <string>


int main(int argc, char** argv)
{
    const unsigned int VIDEO_HEIGHT = 32;
    const unsigned int VIDEO_WIDTH = 64;

    std::cout << "Enter ROM file path: " << std::endl;
    std::string rom;
	std::cin >> rom;

    int scale;
    std::cout << "Enter scale: " << std::endl;
    std::cin >> scale;



	Platform platform("CHIP8 Interpreter", VIDEO_WIDTH * scale, VIDEO_HEIGHT * scale, VIDEO_WIDTH, VIDEO_HEIGHT);

	Chip8 chip8;
	chip8.LoadGame(rom);

	int videoPitch = sizeof(chip8.display[0]) * VIDEO_WIDTH;

	bool quit = false;

	while (!quit)
	{
		quit = platform.ProcessInput(chip8.keys);

			chip8.emulate();
			platform.Update(chip8.display, videoPitch);

	}

	return 0;
}

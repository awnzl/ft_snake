#!/bin/bash

if brew ls --versions sfml > /dev/null; then
	echo "SFML installed";
else
	brew install SFML;
fi

if brew ls --versions cimg > /dev/null; then
	echo "CImg installed";
else
    brew install cimg
fi

if brew ls --versions glfw > /dev/null; then
	echo "GLFW installed";
else
	brew install glfw;
fi

if brew ls --versions sdl2_mixer > /dev/null; then
	echo "sdl2_mixer installed";
else
	brew install sdl2_mixer;
fi
#!/bin/bash

if brew ls --versions sfml > /dev/null; then
	echo "SFML installed";
else
	brew install SFML;
fi

if brew ls --versions sdl2 > /dev/null; then
	echo "SDL2 installed";
else
    brew install sdl2
fi

if brew ls --versions glfw > /dev/null; then
	echo "GLFW installed";
else
	brew install glfw;
fi

if brew ls --versions freealut > /dev/null; then
	echo "freealut installed";
else
	brew install freealut;
fi
#!/bin/bash

if brew ls --versions sfml > /dev/null; then
	echo "sfml installed";
else
	brew install SFML;
fi

export CPLUS_INCLUDE_PATH=/Users/avenzel/.brew/include
export LD_LIBRARY_PATH=/User/avenzel/.brew/Cellar/sfml/2.4.2_1/lib
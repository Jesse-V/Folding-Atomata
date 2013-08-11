#!/bin/sh
cd src
cmake .
if (make) then
	sudo make install
fi

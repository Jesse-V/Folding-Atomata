#!/bin/sh
cmake .
if (make) then
    ./ivoxely
fi

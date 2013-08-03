#!/bin/sh
cmake .
if (make) then
    ./atomata
fi

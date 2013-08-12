#!/bin/sh
cmake .
$cpus = $(grep siblings /proc/cpuinfo | head -1 | cut -d : -f 2)
if (make -j $cpus) then
    ./FoldingAtomata
fi

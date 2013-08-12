#!/bin/sh
cmake .
if (make -j 4) then
    ./FoldingAtomata
fi

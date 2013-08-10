#!/bin/sh
cmake .
if (make) then
    ./FoldingAtomata
fi

#!/bin/sh

#build
sudo apt-get install cmake libglew-dev freeglut3-dev build-essential
cd src
cmake .
cpus=$(grep -c ^processor /proc/cpuinfo)

#attempt to compile
if (make -j $cpus) then

    #setup
    cp -rl ../debian/ debian/
    cd debian/extra_includes/
    gzip --best -c manpage > FoldingAtomata.1.gz
    cd ../../

    #install
    sudo make install
    echo "Successfully installed Folding Atomata on the filesystem."

    #clean
    echo "Cleaning up..."
    rm -rf debian/
    ./clean.sh
    echo "... done. Stop."
fi

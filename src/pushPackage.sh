#!/bin/sh

./clean.sh

cd ..
tar -czf foldingatomata_0.05.orig.tar.gz src/ #http://xkcd.com/1168/
cd src/
echo "Successfully made tarball."

dpkg-buildpackage -kC20BEC80
echo "Successfully made Debian package."

dput ppa:jvictors/testing ../foldingatomata_0.05-0ppa1_amd64.changes
echo "Successfully uploaded packages to Launchpad PPA."

cd ..
rm -f foldingatomata_0.05.orig.tar.gz foldingatomata_0.05-0ppa1.debian.tar.gz
rm -f foldingatomata_0.05-0ppa1.dsc foldingatomata_0.05-0ppa1_amd64.changes
rm -f foldingatomata_0.05-0ppa1_amd64.ppa.upload
cd src/
rm -rf obj-x86_64-linux-gnu/
rm -rf debian/foldingatomata/
echo "Successfully cleaned package build files and folders."

echo "Done pushing packages. Check your email for Launchpad build reports."

#!/bin/sh

./clean.sh

name="foldingatomata_0.4.3"

cd ..
tar -czf ${name}.orig.tar.gz src/ #http://xkcd.com/1168/
cd src/
echo "Successfully made tarball."

dpkg-buildpackage -S -sa -kC20BEC80
echo "Successfully made signed Debian package."

cd ..
dput ppa:jvictors/testing ${name}_source.changes

rm -f ${name}.orig.tar.gz ${name}.debian.tar.gz
rm -f ${name}.dsc ${name}_amd64.changes ${name}_source.changes.changes
rm -f ${name}_source.ppa.upload
echo "Successfully cleaned package build files and folders." #debuild clean

echo "Done pushing packages. Check your email for Launchpad build reports."

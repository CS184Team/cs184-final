#!/usr/bin/env bash
mkdir build
mkdir xyz
cd build
cmake ..
make
curl -O http://graphics.stanford.edu/pub/3Dscanrep/happy/happy_recon.tar.gz
tar -zxvf happy_recon.tar.gz
rm happy_recon.tar.gz
./ply2xyz happy_recon/happy_vrip.ply ../xyz/buddha.xyz
rm -rf happy_recon
./xyzview ../xyz/buddha.xyz
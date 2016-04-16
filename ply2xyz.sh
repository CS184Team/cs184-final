#!/usr/bin/env bash
mkdir build
mkdir xyz
cd build
cmake ..
make

#Stanford Models

#Bunny, > 1.1 MB
curl -O http://graphics.stanford.edu/pub/3Dscanrep/bunny.tar.gz
tar -zxvf bunny.tar.gz
rm bunny.tar.gz
./ply2xyz bunny/reconstruction/bun_zipper.ply ../xyz/bunny.xyz
rm -rf bunny

#Drill, > 58 KB
curl -O http://graphics.stanford.edu/pub/3Dscanrep/drill.tar.gz
tar -zxvf drill.tar.gz
rm drill.tar.gz
./ply2xyz drill/reconstruction/drill_shaft_vrip.ply ../xyz/drill.xyz
rm -rf drill

#Happy Buddha, > 16.6 MB
curl -O http://graphics.stanford.edu/pub/3Dscanrep/happy/happy_recon.tar.gz
tar -zxvf happy_recon.tar.gz
rm happy_recon.tar.gz
./ply2xyz happy_recon/happy_vrip.ply ../xyz/buddha.xyz
rm -rf happy_recon

#Dragon, > 13.1 MB
curl -O http://graphics.stanford.edu/pub/3Dscanrep/dragon/dragon_recon.tar.gz
tar -zxvf dragon_recon.tar.gz
rm dragon_recon.tar.gz
./ply2xyz dragon_recon/dragon_vrip.ply ../xyz/dragon.xyz
rm -rf dragon_recon

#Armadillo, > 4.3 MB
curl -O http://graphics.stanford.edu/pub/3Dscanrep/armadillo/Armadillo.ply.gz
gzip -d Armadillo.ply.gz
./ply2xyz Armadillo.ply ../xyz/armadillo.xyz
rm Armadillo.ply

#Lucy, > 349.4 MB
# curl -O http://graphics.stanford.edu/data/3Dscanrep/lucy.tar.gz
# tar -zxvf lucy.tar.gz
# rm lucy.tar.gz
# ./ply2xyz lucy.ply ../xyz/lucy.xyz
# rm lucy.ply

# XYZ RGB Models

#Asian Dragon, > 90.5 MB
# curl -O http://graphics.stanford.edu/data/3Dscanrep/xyzrgb/xyzrgb_dragon.ply.gz
# gzip -d xyzrgb_dragon.ply.gz
# ./ply2xyz xyzrgb_dragon.ply ../xyz/asiandragon.xyz
# rm xyzrgb_dragon.ply

#Vellum Manuscript, > 55.8 MB
# curl -O http://graphics.stanford.edu/data/3Dscanrep/xyzrgb/xyzrgb_manuscript.ply.gz
# gzip -d xyzrgb_manuscript.ply.gz
# ./ply2xyz xyzrgb_manuscript.ply ../xyz/manuscript.xyz
# rm xyzrgb_manuscript.ply

#Thai Statue, > 127.6 MB
# curl -O http://graphics.stanford.edu/data/3Dscanrep/xyzrgb/xyzrgb_statuette.ply.gz
# gzip -d xyzrgb_statuette.ply.gz
# ./ply2xyz xyzrgb_statuette.ply ../xyz/thaistatue.xyz
# rm xyzrgb_statuette.ply
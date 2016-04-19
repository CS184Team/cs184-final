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
./ply2txt bunny/reconstruction/bun_zipper.ply ../xyz/bunny.txt
rm -rf bunny

#Drill, > 58 KB
curl -O http://graphics.stanford.edu/pub/3Dscanrep/drill.tar.gz
tar -zxvf drill.tar.gz
rm drill.tar.gz
./ply2txt drill/reconstruction/drill_shaft_vrip.ply ../xyz/drill.txt
rm -rf drill

#Happy Buddha, > 16.6 MB
curl -O http://graphics.stanford.edu/pub/3Dscanrep/happy/happy_recon.tar.gz
tar -zxvf happy_recon.tar.gz
rm happy_recon.tar.gz
./ply2txt happy_recon/happy_vrip.ply ../xyz/buddha.txt
rm -rf happy_recon

#Dragon, > 13.1 MB
curl -O http://graphics.stanford.edu/pub/3Dscanrep/dragon/dragon_recon.tar.gz
tar -zxvf dragon_recon.tar.gz
rm dragon_recon.tar.gz
./ply2txt dragon_recon/dragon_vrip.ply ../xyz/dragon.txt
rm -rf dragon_recon

# Armadillo, > 4.3 MB
curl -O http://graphics.stanford.edu/pub/3Dscanrep/armadillo/Armadillo.ply.gz
gzip -d Armadillo.ply.gz
./ply2txt Armadillo.ply ../xyz/armadillo.txt
rm Armadillo.ply

# Lucy, > 349.4 MB
curl -O http://graphics.stanford.edu/data/3Dscanrep/lucy.tar.gz
tar -zxvf lucy.tar.gz
rm lucy.tar.gz
./ply2txt lucy.ply ../xyz/lucy.txt
rm lucy.ply

# XYZ RGB Models

#Asian Dragon, > 90.5 MB
curl -O http://graphics.stanford.edu/data/3Dscanrep/xyzrgb/xyzrgb_dragon.ply.gz
gzip -d xyzrgb_dragon.ply.gz
./ply2txt xyzrgb_dragon.ply ../xyz/asiandragon.txt
rm xyzrgb_dragon.ply

#Vellum Manuscript, > 55.8 MB
curl -O http://graphics.stanford.edu/data/3Dscanrep/xyzrgb/xyzrgb_manuscript.ply.gz
gzip -d xyzrgb_manuscript.ply.gz
./ply2txt xyzrgb_manuscript.ply ../xyz/manuscript.txt
rm xyzrgb_manuscript.ply

#Thai Statue, > 127.6 MB
curl -O http://graphics.stanford.edu/data/3Dscanrep/xyzrgb/xyzrgb_statuette.ply.gz
gzip -d xyzrgb_statuette.ply.gz
./ply2txt xyzrgb_statuette.ply ../xyz/thaistatue.txt
rm xyzrgb_statuette.ply
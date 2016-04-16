## Installation
```
$ git clone https://github.com/MichaelSnowden/cs184-final
$ cd cs184-final
$ chmod 777 install.sh
$ ./install.sh
```

The install script looks like this:
```
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
```

Essentially, it downloads the buddha point cloud, converts it to a `.xyz` file, and renders it.

You can also run the familiar
```
$ ./meshedit ../dae/cow.dae
```

To add more point cloud files , you will need to download them from [this webpage](http://graphics.stanford.edu/data/3Dscanrep/).
The files need to be kept outside of source control because they are way too big (>100 MB).

Once you have downloaded them, you can will need to run the two new executables, `ply2xyz` and `xyzview`.

```
$ ./ply2xyz ../ply/dragon.ply ../ply/dragon.xyz
$ ./xyzview ../ply/dragon.xyz
```

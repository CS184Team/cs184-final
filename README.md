## Installation
```
$ git clone https://github.com/MichaelSnowden/cs184-final
$ cd cs184-final
$ mkdir build
$ mkdir txt
$ cd build
$ cmake ..
$ make
$ curl -O http://graphics.stanford.edu/pub/3Dscanrep/happy/happy_recon.tar.gz
$ tar -zxvf happy_recon.tar.gz
$ rm happy_recon.tar.gz
$ ./ply2txt happy_recon/happy_vrip.ply ../xyz/buddha.xyz
rm -rf happy_recon
./view ../txt/buddha.txt
```

Essentially, it downloads the buddha point cloud, converts it to a `.txt` file, and renders it.

You can also run the familiar
```
$ ./meshedit ../dae/cow.dae
```

To add more point cloud files , you will need to download them from [this webpage](http://graphics.stanford.edu/data/3Dscanrep/).
The files need to be kept outside of source control because they are way too big (>100 MB).

Once you have downloaded them, you can will need to run the two new executables, `ply2txt` and `xyzview`.

```
$ ./ply2txt ../ply/dragon.ply ../txt/dragon.txt
$ ./view ../txt/dragon.txt
```

# cs184-final
## Usage
```
$ git clone https://github.com/MichaelSnowden/cs184-final
$ cd cs184-final
$ mkdir build
$ cd build
$ cmake ..
$ make
```

From there, you can run the familiar
```
$ ./meshedit ../dae/cow.dae
```


To proceed, you will need to download some `.ply` files from [this webpage](http://graphics.stanford.edu/data/3Dscanrep/).
The files need to be kept outside of source control because they are way too big (>100 MB).

Or, you can run either of the two new executables, `ply2xyz` and `xyzview`.

```
$ ./ply2xyz ../ply/dragon.ply ../ply/dragon.xyz
$ ./xyzview ../ply/dragon.xyz
```
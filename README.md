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

Or, you can run either of the two new executables, `ply2xyz` and `xyzview`.

```
$ ./ply2xyz ../ply/dragon.ply ../ply/dragon.xyz
$ ./xyzview ../ply/dragon.xyz
```
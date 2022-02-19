## Summary

Makefile based build processes are described below.

## CMake build (recommended)

### setup your environment

On SCOREC RHEL7 one possible configuration is:

```
module load gcc mpich cmake cuda/10.2
```

### configure and build

``
cmake -S ./egads -B buildEGLcuda -DCMAKE_CUDA_ARCHITECTURES=75 -DCMAKE_INSTALL_PREFIX=$PWD/buildEGLcuda/install
cmake --build buildEGLcuda --target install -j8
```

#### debug build
```
cmake -S ./egads -B buildEGLcudaDbg -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CUDA_FLAGS_DEBUG="-g -G" -DCMAKE_CUDA_ARCHITECTURES=75 -DCMAKE_INSTALL_PREFIX=$PWD/buildEGLcudaDbg/install
cmake --build buildEGLcudaDbg --target install -j8
```

## Makefile build

### setup your environment

On SCOREC RHEL7 one possible configuration is:

```
module load gcc mpich cmake cuda/10.2
```

### configure

```
cd config
./makeEnv /path/to/openCascade/install
cd ../
#edit ESPenv.sh and replace LINUX64 with LINUX64_cuda
source ESPenv.sh
#edit include/LINUX64_cuda and set the appropriate cuda architecture flag for your system (currently set to sm_75)
```

Note, the opencascade install is not used, but the `makeEnv` script requires it, AFAIK.

### build egadslite

```
cd lite
make -f Makefile.cuda clean
make -f Makefile.cuda
```

### build liteTest

```
make -f liteTestCuda.make clean
make -f liteTestCuda.make
```

### run liteTest

Expected output from liteTest (host build)

```
$ ../bin/liteTest.host ../examples/box.egadslite 
 EG_open          = 0
 EG_loadModel     = 0  ../examples/box.egadslite
 
MODEL 1fc9150 0
  BBox = -12.700026 -12.700026 -0.000000  12.700026 12.700026 25.400051
  BODY 1fc9250 0
    BBox = -12.700026 -12.700026 -0.000000  12.700026 12.700026 25.400051
    Attr: _body = 1 
    Attr: _brch = 1 
    Attr: _tParams = 1.099854 0.043994 15.000000 
    SHELL 1fcba40 1  sense = 1
...
```

Executing `liteTestCuda` currently produces the following output:

```
$ ./bin/liteTest.cuda examples/box.egadslite
allocating new context
allocating new strings
copying strings
have new context
 EG_open          = 0
 EG_loadModel     = 0  ../examples/box.egadslite
cuda topo magicnum 98789 magic 98789
stat 0 oclass 26 mtype 0 nbodies 1
0 Node EG_getBodyTopos 8
0 EDGE EG_getBodyTopos 12
0 LOOP EG_getBodyTopos 6
0 LOOP::NODE EG_getBodyTopos 6
0 LOOP::FACE EG_getBodyTopos 6
1 LOOP::NODE EG_getBodyTopos 6
1 LOOP::FACE EG_getBodyTopos 6
2 LOOP::NODE EG_getBodyTopos 6
2 LOOP::FACE EG_getBodyTopos 6
3 LOOP::NODE EG_getBodyTopos 6
3 LOOP::FACE EG_getBodyTopos 6
4 LOOP::NODE EG_getBodyTopos 6
4 LOOP::FACE EG_getBodyTopos 6
5 LOOP::NODE EG_getBodyTopos 6
5 LOOP::FACE EG_getBodyTopos 6
0 FACE EG_getBodyTopos 6
0 SHELL EG_getBodyTopos 1
 
 Object Scan      = 54 35
done getTopo
```

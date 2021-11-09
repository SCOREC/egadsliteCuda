## Summary

CMake and Makefile based build processes are described below.
Both approaches produce `libegadslite`, but fail during linking of `liteTest`, a simple test case, with multiple
undefined references that seem to indicate a problem with library creation, the linking command, or both.

The CMake approach abstracts away the details of linking C/C++ with a CUDA
library.  Given that, and my familiarity with CMake, I have more confidence in
it than the Makefile based approach.

## CMake build

### setup your environment

On SCOREC RHEL7 one possible configuration is:

```
module load gcc mpich cmake cuda/10.2
```

### run cmake

```
mkdir build
cd build
cmake /path/to/source -DCMAKE_CUDA_ARCHITECTURES=75
make -j4
```

The final step of the build process is to link the `liteTest` binary. 

When using a static `libegadslite` library (`-DBUILD_SHARED_LIBS=OFF`, the default), this fails with multiple undefined references, i.e.:

```
[ 95%] Linking CUDA device code CMakeFiles/liteTest.dir/cmake_device_link.o
[100%] Linking CUDA executable liteTest
/lib/../lib64/crt1.o: In function `_start':
(.text+0x20): undefined reference to `main'
libegadslite.a(liteBase.c.o): In function `EG_loadModel':
tmpxft_000067c1_00000000-5_liteBase.cudafe1.cpp:(.text+0x34a4): undefined reference to `EG_importModel'
libegadslite.a(liteGeom.c.o): In function `EG_evaluatX':
tmpxft_000067c0_00000000-5_liteGeom.cudafe1.cpp:(.text+0x145e): undefined reference to `EG_evaluateGeom'
tmpxft_000067c0_00000000-5_liteGeom.cudafe1.cpp:(.text+0x14d7): undefined reference to `EG_evaluateGeom'
libegadslite.a(liteGeom.c.o): In function `EG_invEvaLimits':
tmpxft_000067c0_00000000-5_liteGeom.cudafe1.cpp:(.text+0x1720): undefined reference to `EG_invEvaGeomLimits'
tmpxft_000067c0_00000000-5_liteGeom.cudafe1.cpp:(.text+0x1836): undefined reference to `EG_invEvaGeomLimits'
tmpxft_000067c0_00000000-5_liteGeom.cudafe1.cpp:(.text+0x190d): undefined reference to `EG_invEvaGeomLimits'
tmpxft_000067c0_00000000-5_liteGeom.cudafe1.cpp:(.text+0x196a): undefined reference to `EG_inFaceX'
libegadslite.a(liteGeom.c.o): In function `EG_invEvaluateGuess':
tmpxft_000067c0_00000000-5_liteGeom.cudafe1.cpp:(.text+0x1e36): undefined reference to `EG_invEvaluateGeomGuess'
tmpxft_000067c0_00000000-5_liteGeom.cudafe1.cpp:(.text+0x1ed3): undefined reference to `EG_invEvaluateGeomGuess'
tmpxft_000067c0_00000000-5_liteGeom.cudafe1.cpp:(.text+0x1f31): undefined reference to `EG_invEvaluateGeomGuess'
libegadslite.a(liteTopo.c.o): In function `EG_inFace':
tmpxft_000067df_00000000-5_liteTopo.cudafe1.cpp:(.text+0x29c8): undefined reference to `EG_inFaceX'
...
```

When using a shared `libegadslite` library (`-DBUILD_SHARED_LIBS=ON`):

```
[100%] Linking CXX executable liteTest
/lib/../lib64/crt1.o: In function `_start':
(.text+0x20): undefined reference to `main'
libegadslite.so: undefined reference to `EG_inFaceX'
libegadslite.so: undefined reference to `EG_getEdgeUV(egObject const*, egObject const*, int, double, double*)'
libegadslite.so: undefined reference to `EG_getRange(egObject const*, double*, int*)'
libegadslite.so: undefined reference to `EG_initTessBody(egObject*, egObject**)'
libegadslite.so: undefined reference to `EG_addStrAttrDev(egObject*, char const*, char const*)'
...
```

Note, these are essentially identical to the undefined refs when building with Makefiles.

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
```

### build egadslite

```
cd lite
make -f Makefile.cuda clean
make -f Makefile.cuda
```

### build liteTest

```
make -f liteTest.cuda clean
make -f liteTest.cuda
```

This fails with multiple undefined references, i.e.:
```
egads/lib/libegadslite.so: undefined reference to `EG_inFaceX'
egads/lib/libegadslite.so: undefined reference to `EG_getEdgeUV(egObject const*, egObject const*, int, double, double*)'
egads/lib/libegadslite.so: undefined reference to `EG_getRange(egObject const*, double*, int*)'
egads/lib/libegadslite.so: undefined reference to `EG_initTessBody(egObject*, egObject**)'
egads/lib/libegadslite.so: undefined reference to `EG_addStrAttrDev(egObject*, char const*, char const*)'
egads/lib/libegadslite.so: undefined reference to `EG_setTessEdge(egObject*, int, int, double const*, double const*)'
```

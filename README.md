## Summary

Makefile based build processes are described below.

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
make -f liteTest.cuda clean
make -f liteTest.cuda
```

### run liteTest

Executing liteTest results in a segfault after loading a box model:

```
$ ./bin/liteTest examples/box.egadslite
allocating new context
allocating new strings
copying strings
have new context
 EG_open          = 0
 EG_loadModel     = 0  examples/box.egadslite
Segmentation fault (core dumped) 
```

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

Expected output (from host build)

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

Executing the cuda build of liteTest results in a segfault after loading a box model:

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

`gdb` indicates that the segfault occurs when the host (cpu) calls
`EG_getTopology(...)` and attempts to access a member of the model
(`topo->magicnumber`).  Prior to this call, the model object pointer
is returned from a host call to `EG_loadModel(...)`.
`EG_loadModel(...) calls `EG_importModel(...)`, which after loading the model, calls:

```
EG_SET_OBJECT_PTR(&(context->topObj), &obj);
*model = obj;
```

`EG_SET_OBJECT_PTR(...)` is a macro which sets a pointer stored on the device, the first
argument, to an address stored in a pointer on the host, the second argument.
Note, at this point in `EG_importModel` `obj` is a pointer to the model on the
device which was created from a pointer to the model on the host, `obj_h`.  As
expected, accessing `obj_h->magicnumber` is successful.

Should `EG_getTopology` be called on the device (from a cuda kernel) or should
the model object be copied to the host prior to the `EG_getTopology` call?


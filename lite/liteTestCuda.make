#
IDIR = $(ESP_ROOT)/include
include $(IDIR)/$(ESP_ARCH)
LDIR = $(ESP_ROOT)/lib
ifdef ESP_BLOC
ODIR = $(ESP_BLOC)/obj
TDIR = $(ESP_BLOC)/test
else
ODIR = $(ESP_ROOT)/obj_cuda
TDIR = $(ESP_ROOT)/bin
endif


$(TDIR)/liteTest:	$(ODIR)/liteTest.o
	if [ ! -d $(TDIR) ]; then mkdir $(TDIR); fi
	$(CC) -o $(TDIR)/liteTest.cuda $(ODIR)/liteTest.o $(LDIR)/libegadslitestaticCuda.a \
		$(RPATH) -lpthread -lm -lcudart

$(ODIR)/liteTest.o:	liteTestCuda.c
	$(CC) -c $(COPTS) $(DEFINE) -I$(IDIR) liteTestCuda.c -o $(ODIR)/liteTest.o

clean:
	-rm $(ODIR)/liteTest.o

cleanall:	clean
	-rm $(TDIR)/liteTest

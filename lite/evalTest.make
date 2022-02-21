#
IDIR = $(ESP_ROOT)/include
include $(IDIR)/$(ESP_ARCH)
LDIR = $(ESP_ROOT)/lib
ifdef ESP_BLOC
ODIR = $(ESP_BLOC)/obj
TDIR = $(ESP_BLOC)/test
else
ODIR = .
TDIR = $(ESP_ROOT)/bin
endif


$(TDIR)/evalTest:	$(ODIR)/evalTest.o
	$(CC) -o $(TDIR)/evalTest $(ODIR)/evalTest.o -L$(LDIR) -legadslite \
		$(RPATH) -lpthread -lm

$(ODIR)/evalTest.o:	evalTest.c
	$(CC) -c $(COPTS) $(DEFINE) -I$(IDIR) evalTest.c -o $(ODIR)/evalTest.o

clean:
	-rm $(ODIR)/evalTest.o

cleanall:	clean
	-rm $(TDIR)/evalTest

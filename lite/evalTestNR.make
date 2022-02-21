#
IDIR = $(ESP_ROOT)/src/EGADS/include
include $(IDIR)/$(ESP_ARCH)
LDIR = $(ESP_ROOT)/lib
ifdef ESP_BLOC
ODIR = $(ESP_BLOC)/obj
TDIR = $(ESP_BLOC)/test
else
ODIR = .
TDIR = $(ESP_ROOT)/bin
endif


$(TDIR)/evalTest:	$(ODIR)/evalTestNR.o
	$(CC) -o $(TDIR)/evalTestNR $(ODIR)/evalTestNR.o -L$(LDIR) -legadsliteNR \
		$(RPATH) -lpthread -lm

$(ODIR)/evalTestNR.o:	evalTest.c
	$(CC) -c $(COPTS) $(DEFINE) -I$(IDIR) evalTest.c -o $(ODIR)/evalTestNR.o

clean:
	-rm $(ODIR)/evalTest.o

cleanall:	clean
	-rm $(TDIR)/evalTestNR

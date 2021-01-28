MAGMADIR     ?= /usr/local/magma
CUDADIR      ?= /usr/local/cuda
MKLROOT 	 ?= /opt/intel/oneapi/mkl/latest

MAGMA_CFLAGS    := 	-DADD_ \
                	-I$(MAGMADIR)/include \
                    -I$(MAGMADIR)/sparse/include \
                    -I$(CUDADIR)/include

MKL_CFLAGS 		:= 	-I$(MKLROOT)/include

CXX_FLAGS = -O0 -g3 $(MAGMA_CFLAGS) $(MKL_CFLAGS)

MKL_LIBS 	 	:= 	 -Wl,--start-group ${MKLROOT}/lib/intel64/libmkl_intel_lp64.a ${MKLROOT}/lib/intel64/libmkl_intel_thread.a ${MKLROOT}/lib/intel64/libmkl_core.a -Wl,--end-group -liomp5 -lpthread -lm -ldl

MAGMA_LIBS      := 	-L$(MAGMADIR)/lib -lmagma_sparse -lmagma \
                    -L$(CUDADIR)/lib64 -lcublas -lcudart -lcusparse 			

LD_LIBS = $(MKL_LIBS) $(MAGMA_LIBS)

CXX = g++

OUTPUT = output.txt

EXECUTABLE = test



$(EXECUTABLE): main.o 
	$(CXX) main.o -o $(EXECUTABLE) $(LD_LIBS)

main.o: main.cpp
	$(CXX) -c $(CXX_FLAGS) main.cpp 

.PHONY: clean run run_out

clean:
	rm *.o $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

run_out: $(EXECUTABLE)
	./$(EXECUTABLE) > $(OUTPUT)
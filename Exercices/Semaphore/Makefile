CC=gcc #Compiler
EDL=gcc #Linker
AR=ar #Archiver
CCFLAGS=-Wall -m32 #Compiler options
EDLFLAGS=-Wall -m32 #Linker options
EXE=test #Binary name
LIB=libgthread.a #Static library name
ECHO=@echo

EOBJ=test.o 
LOBJ=gthread.o

$(EXE): $(EOBJ)
	@echo building $<
	$(EDL)  -o $(EXE) $(EDLFLAGS) $(EOBJ) $(LIB)
	@echo done

$(LIB): $(LOBJ)
	@echo building $<
	$(AR) rcs $(LOBJ)
	@echo done

%.o : %.c *.h
	@echo building $< ...
	$(CC) $(CCFLAGS) -c -D $(DEFINES) $<
	@echo done
	
clean: 
	@echo -n cleaning repository... 
	@rm -rf *.o
	@rm -rf *.so*
	@echo cleaned.

CXX       := clang++
INCLUDES  := $(shell root-config --cflags)
LIBS  := $(shell root-config --glibs) 
OBJ = main.o fitTree.o SimpleHistSVC.o
INC = fitTree.h SimpleHistSVC.h

%.o: %.C $(INC)
	$(CXX) $(INCLUDES)  -c -o $@ $< $(LIBS)

main.exe : $(OBJ)
	$(CXX) -o $@ $^ $(INCLUDES) $(LIBS)

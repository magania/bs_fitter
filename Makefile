MAKEFLAGS = --no-print-directory -r -s
CCFLAGS= -O3 -g -fPIC

INCLUDE = $(shell root-config --cflags) -Iinclude
LIBS    = $(shell root-config --libs) -lMinuit -lRooFit -lRooFitCore
ROOTLIB = $(shell root-config --libdir)
LIBSTDC = $(shell g++ -m32 -print-file-name=libstdc++.a)
MYLIBS =  obj/Efficiency.o obj/RooBsTimeAngle.o obj/TransAngles.o obj/TransAnglesEfficiency.o

all: static

$(MYLIBS): obj/%.o : src/%.cc include/%.h
	@echo $(CXX) $(CCFLAGS) $(INCLUDE)  -c $< -o $@
	$(CXX) $(CCFLAGS) $(INCLUDE) -c $< -o $@
#	@echo $(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) -shared $@ -o $(@:obj/%.o=lib/lib%.so)
#	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) -shared $@ -o $(@:obj/%.o=lib/lib%.so)

obj/Dict.o: $(MYLIBS)
	@echo rootcint -f Dict.cc  -c include/*
	rootcint -f Dict.cc  -c include/BsFitter.h include/Efficiency.h include/RooBsTimeAngle.h include/TransAnglesEfficiency.h include/TransAngles.h
	@echo $(CXX) $(CCFLAGS) $(INCLUDE) -c Dict.cc -o obj/Dict.o
	$(CXX) $(CCFLAGS) $(INCLUDE) -c Dict.cc -o obj/Dict.o
#	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) -shared obj/Dict.o -o lib/libDict.so
	
obj/BsFitter.o: src/BsFitter.cc include/BsFitter.h $(MYLIBS) obj/Dict.o
	@echo $(CXX) $(CCFLAGS) $(INCLUDE) -c $< -o obj/BsFitter.o
	$(CXX) $(CCFLAGS) $(INCLUDE)  -c $< -o obj/BsFitter.o
	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) -shared obj/Dict.o $(MYLIBS) obj/BsFitter.o -o lib/libBsFitter.so
#	$(CXX) $(CCFLAGS) $(INCLUDE) $(DGAUSS) -c $< -o obj/BsFitter_gauss.o
#	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) -shared obj/Dict.o $(LIBRARY) obj/BsFitter_gauss.o -o lib/libBsFitter_gauss.so

bin/bs: src/bs.cc $(MYLIBS) obj/BsFitter.o obj/Dict.o
	@echo $(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) $^ -o bin/bs
	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) $^ -o bin/bs

bin/fitter: src/fitter.cc $(MYLIBS) obj/BsFitter.o obj/Dict.o
	@echo $(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) $^ -o bin/fitter
	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) $^ -o bin/fitter

static: src/bs.cc $(MYLIBS) obj/BsFitter.o obj/Dict.o
	@echo $(CXX) $(CCFLAGS) $(INCLUDE) -c $< -o obj/bs.o
	$(CXX) $(CCFLAGS) $(INCLUDE) -c $< -o obj/bs.o
	@echo $(CXX) $(CCFLAGS) -o bin/BS obj/* $(ROOTLIB)/../roofit/libRooFit.a $(LIBS)
	$(CXX) -m32 -static $(CCFLAGS) -o bin/BS obj/*  \
	     $(LIBSTDC) \
	     $(ROOTLIB)/../math/mathcore/src/TComplex.o \
         $(ROOTLIB)/../math/minuit/src/TMinuit.o \
         $(ROOTLIB)/../tree/tree/src/TTree.o \
         $(ROOTLIB)/libRoot.a \
         $(ROOTLIB)/libpcre.a \
         $(ROOTLIB)/libAfterImage.a \
         $(ROOTLIB)/libfreetype.a \
         $(ROOTLIB)/../roofit/libRooFit.a \
         -ldl -lpthread -static-libgcc
clean:
	rm $(MYLIBS)
	rm Dict.cc Dict.h
	rm bin/bs

		 #$(ROOTLIB)/../graf2d/graf/src/*.o \
	     #$(ROOTLIB)/../libMissing.a \
         #$(ROOTLIB)/../math/mathcore/src/TComplex.o \
         #$(ROOTLIB)/../math/minuit/src/TMinuit.o \
         #$(ROOTLIB)/../tree/tree/src/TTree.o \
	

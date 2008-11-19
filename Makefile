MAKEFLAGS = --no-print-directory -r -s
CCFLAGS= -g -fPIC

INCLUDE = $(shell root-config --cflags) -Iinclude
LIBS    = $(shell root-config --libs) -lRooFit -lRooFitCore

MYLIBS = obj/TransAngles.o obj/RooBsTimeAngle.o obj/F.o obj/TimeAngle.o
FITTERS = BsFitter

DFLAGS = -DRES_TRUE
#DFLAGS = -DRES_GAUSS

all: bin/fitter

$(MYLIBS): obj/%.o : src/%.cc include/%.h
	@echo $(CXX) $(CCFLAGS) $(INCLUDE)  -c $< -o $@
	$(CXX) $(CCFLAGS) $(INCLUDE) -c $< -o $@
	@echo $(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) -shared $@ -o $(@:obj/%.o=lib/lib%.so)
	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) -shared $@ -o $(@:obj/%.o=lib/lib%.so)

obj/BsFitter.o: src/BsFitter.cc include/BsFitter.h obj/$(LIBRARY)
	rootcint -f Dict.cc  -c include/* 
	$(CXX) $(CCFLAGS) $(INCLUDE) -c Dict.cc -o obj/Dict.o
	@echo $(CXX) $(CCFLAGS) $(INCLUDE) $(DFLAGS) -c $< -o obj/BsFitter.o
	$(CXX) $(CCFLAGS) $(INCLUDE) $(DFLAGS) -c $< -o obj/BsFitter.o
#	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) -shared obj/Dict.o $(LIBRARY) obj/BsFitter_nores.o -o lib/libBsFitter_nores.so
#	$(CXX) $(CCFLAGS) $(INCLUDE) $(DGAUSS) -c $< -o obj/BsFitter_gauss.o
#	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) -shared obj/Dict.o $(LIBRARY) obj/BsFitter_gauss.o -o lib/libBsFitter_gauss.so

bin/fitter: src/fitter.cc $(MYLIBS) obj/BsFitter.o obj/Dict.o
	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) $^ -o bin/fitter

piedritas: piedritas.cc
	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) -Llib -lTimeAngle -lF  $< -o piedritas

bin/toy: toy.cc  $(MYLIBS) obj/BsFitter.o obj/Dict.o
	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS)  $^ -o bin/toy

toymc: toymc.cc
	$(CXX) $(CCFLAGS) $(INCLUDE) $(LIBS) -Llib -lRooBsTimeAngle -lTransAngles -lBsFitter_nores  $< -o toymc

clean:
	rm $(MYLIBS)
	rm Dict.cc Dict.h
	rm bin/fitter
MAKEFLAGS = --no-print-directory -r -s
CCFLAGS= -O3 -g -fPIC

INCLUDE = $(shell root-config --cflags) -Iinclude
LIBS    = $(shell root-config --libs) -lMinuit -lRooFit -lRooFitCore

MYLIBS =  obj/Efficiency.o obj/RooBsTimeAngle.o obj/TransAngles.o obj/TransAnglesEfficiency.o

#DFLAGS = -DRES_TRUE
#DFLAGS = -DRES_GAUSS
#DFLAGS = -DRES_GAUSS -DEFFICIENCY


all: bin/bs

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
	
clean:
	rm $(MYLIBS)
	rm Dict.cc Dict.h
	rm bin/bs
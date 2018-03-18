CXX=g++
CXXFLAGS=-Wall -fno-rtti
CXXFLAGS+=-I$(shell $(CXX) -print-file-name=plugin)/include

all: dummy unused cfg

dummy: dummy.c
	$(CXX) $(CXXFLAGS) -shared -fPIC -o dummy.so $^

cfg: cfg.c
	$(CXX) $(CXXFLAGS) -shared -fPIC -o cfg.so $^

unused: warn_unused.cc
	$(CXX) $(CXXFLAGS) -shared -fPIC -o unused.so $^

clean:
	-rm -f *.o *.so
.PHONY: all

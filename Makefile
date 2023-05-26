CXXFLAGS = -g -Wall -std=c++17 -O3
LDLIBS =

PRGM  = Main
SRCS := $(wildcard pieces/*.cpp) $(wildcard *.cpp) 
SRCS := $(wildcard pieces/*.cpp) $(wildcard *.cpp) 
HDRS := $(wildcard *.h) $(wildcard pieces/*.h)
OBJSH := $(HDRS:.h=.o)
DEPSH := $(OBJSH:.o=.d)

OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

run: ./Main

build: $(PRGM)

$(PRGM): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(OBJS) $(OBJSH) $(DEPS) $(DEPSH)
	rm -rf $(PRGM)

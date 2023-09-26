LIBRARY			= budoux.dll
CXX				=  clang++
CXXFLAGS		= -I . -Wall -Ofast -std=c++17 -I include
LD				=  clang++
LDFLAGS			= -shared -lmsvcrt -Xlinker /NODEFAULTLIB:LIBCMT
OBJS			= saori.o budoux.o \
				  windows/dll.o windows/util.o \
				  sstp/header.o sstp/request.o sstp/response.o
ALL				= all

.SUFFIXES: .cc .o

.PHONY: all
$(ALL): $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(LD) $(LDFLAGS) -o $(LIBRARY) $(OBJS)

.cc.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -f *.o windows/*.o sstp/*.o *.exp *.lib *.exe *.dll

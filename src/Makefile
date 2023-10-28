

# CC:=clang++
src:=main
CC:=g++
LDFLAGS:=-L./glpk_lib/lib/
LIBS:= -lfunc
INCLUDE:= -I./glpk_lib/include/

$(src):$(src).cpp hypergraph.cpp td.cpp twbound.cpp SCsolver.cpp

exe:
	$(CC) -o $(src) $(src).cpp hypergraph.cpp td.cpp twbound.cpp SCsolver.cpp -O2 -std=c++11 $(LDFLAGS) $(INCLUDE) -lglpk

mac_exe:
	$(CC) -o $(src) $(src).cpp hypergraph.cpp td.cpp twbound.cpp SCsolver.cpp -O2 -std=c++11 -lglpk

debug:
	$(CC) -o $(src) $(src).cpp hypergraph.cpp td.cpp twbound.cpp SCsolver.cpp -g -Wall -std=c++11 $(LDFLAGS) $(INCLUDE) -lglpk

mac_debug:
	$(CC) -o $(src) $(src).cpp hypergraph.cpp td.cpp twbound.cpp SCsolver.cpp -g -Wall -std=c++11 -lglpk

clean:
	rm -f $(src) *.o
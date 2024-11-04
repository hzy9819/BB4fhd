

# CC:=clang++
src:=main
CC:=g++
LDFLAGS:=-L../../lib/glpk-4.55/lib/
LIBS:= -lfunc
INCLUDE:= -I../../lib/glpk-4.55/include/

$(src):$(src).cpp hypergraph.cpp td.cpp twbound.cpp SCsolver.cpp

exe:
	$(CC) -o $(src) $(src).cpp hypergraph.cpp td.cpp twbound.cpp SCsolver.cpp -O2 -std=c++17 $(LDFLAGS) $(INCLUDE) -lglpk

mac_exe:
	$(CC) -o $(src) $(src).cpp hypergraph.cpp td.cpp twbound.cpp SCsolver.cpp -O2 -std=c++17 -lglpk

debug:
	$(CC) -o $(src) $(src).cpp hypergraph.cpp td.cpp twbound.cpp SCsolver.cpp -g -Wall -std=c++17 $(LDFLAGS) $(INCLUDE) -lglpk

mac_debug:
	$(CC) -o $(src) $(src).cpp hypergraph.cpp td.cpp twbound.cpp SCsolver.cpp -g -Wall -std=c++17 -lglpk

clean:
	rm -f $(src) *.o
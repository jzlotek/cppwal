CXX = g++
EXE = cwal

FILES:=$(wildcard src/*.cpp)
FILES:=$(FILES:.cpp=.o)

$(EXE): $(FILES)
	$(CXX) -o $(EXE) $(FILES)

clean:
	rm -fv src/*.o
	rm ./$(EXE)

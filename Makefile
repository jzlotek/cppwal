CXX = g++
EXE = cwal

FILES:=$(wildcard src/*.cpp)
FILES:=$(FILES:.cpp=.o)

$(EXE): $(FILES)
	$(CXX) -o $(EXE) $(FILES)
#	if[! -d "./schemes"]; then\
#		mkdir ./schemes\
#	fi

clean:
	rm -fv src/*.o

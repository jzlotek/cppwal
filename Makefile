CXX = g++
EXE = cwal

FILES:=$(wildcard src/*.cpp)
FILES:=$(FILES:.cpp=.o)

install: $(EXE)
	sudo cp $(EXE) /usr/local/bin/$(EXE)

build: $(EXE)

folders:
	mkdir -p $(HOME)/.cache/cwal && mkdir -p $(HOME)/.cache/cwal/schemes

$(EXE): $(FILES) folders
	$(CXX) -o $(EXE) $(FILES)

clean:
	rm -fv src/*.o
	rm ./$(EXE)

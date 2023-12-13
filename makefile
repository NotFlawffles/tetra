CXX = c++
CXXFLAGS = -Wall -Wextra -pedantic -ggdb -std=c++20 -o
OBJECT = tetra
SRC = $(shell find src/ -type f -name *.cpp)
INSTALLATION_TARGET = /usr/bin/tetra
LIBS = -lSDL2main -lSDL2

default: $(SRC)
	@$(CXX) $(CXXFLAGS) $(OBJECT) $(SRC) $(LIBS)

clean: $(OBJECT)
	@rm $(OBJECT)

install: $(INSTALLATION_TARGET)
	@make default
	@sudo mv $(OBJECT) $(INSTALLATION_TARGET)

uninstall: $(INSTALLATION_TARGET)
	@sudo rm $(INSTALLATION_TARGET)

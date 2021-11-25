CXX		=	g++ -std=c++11 -O3
EXE		=  Lab4 
OBJ_DIR	=	objs

CFLAGS = -g -O3

all: $(EXE)

$(EXE): $(OBJ_DIR)/main.o $(OBJ_DIR)/legalizer.o $(OBJ_DIR)/util.o
	$(CXX) $^ $(CFLAGS) -o $@
	
$(OBJ_DIR)/main.o: main.cpp legalizer.h | obj_dir
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/util.o: util.cpp util.h | obj_dir
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/legalizer.o: legalizer.cpp legalizer.h | obj_dir
	$(CXX) $(CFLAGS) -c $< -o $@


obj_dir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf *.gch objs/

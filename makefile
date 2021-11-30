CXX		=	g++ -std=c++11
EXE		=  pa
OBJ_DIR	=	objs
SRC_DIR = srcs
CLIPPER_DIR = clipper

CFLAGS = -g

all: $(EXE)

$(EXE): $(OBJ_DIR)/main.o $(OBJ_DIR)/plan.o $(OBJ_DIR)/util.o $(OBJ_DIR)/clipper.o
	$(CXX) $^ $(CFLAGS) -o $@
	
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/plan.h | obj_dir
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/util.o: $(SRC_DIR)/util.cpp $(SRC_DIR)/util.h | obj_dir
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/plan.o: $(SRC_DIR)/plan.cpp $(SRC_DIR)/plan.h | obj_dir
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/clipper.o: $(CLIPPER_DIR)/clipper.cpp | obj_dir
	$(CXX) $(CCFLAGS) -c $< -o $@

obj_dir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf *.gch objs/

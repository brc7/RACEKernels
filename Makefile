# How to use 
# To make tests: make tests 
# To make all experiment binaries: make binaries

CXX = /usr/local/Cellar/gcc/9.1.0/bin/g++-9
CFLAGS = -O3 -std=c++11 -fopenmp

TESTS = tests_RACE.cpp tests_L2Hash.cpp tests_io.cpp tests_HBS.cpp tests_SKA.cpp tests_util.cpp tests_RangelessRACE.cpp tests_SRPHash.cpp
TESTS_DIR = test/

SRCS = RACE.cpp MurmurHash.cpp L2Hash.cpp io.cpp HBS.cpp SKA.cpp util.cpp RangelessRACE.cpp MinHash.cpp SRPHash.cpp L1Hash.cpp
SRCS_DIR = src/

BUILD_DIR = build/
BIN_DIR = bin/
INC := -I include

# List of target executables
TARGETS = KDETool.cpp HBSGenerate.cpp RSGenerate.cpp WSSTool.cpp RACEGenerate.cpp RACETool.cpp KCentersTool.cpp SKAGenerate.cpp
# covtype.cpp glove300d.cpp glove50d.cpp mnist.cpp
TARGETS_DIR = targets/

# Everything beyond this point is determined from previous declarations, don't modify
OBJECTS = $(addprefix $(BUILD_DIR), $(SRCS:.cpp=.o))
BINARIES = $(addprefix $(BIN_DIR), $(TARGETS:.cpp=))

$(BUILD_DIR)%.o: $(SRCS_DIR)%.cpp
	$(CXX) $(INC) -c $(CFLAGS) $< -o $@

# Tests-main is compiled once and then never modified ever again for fast test compilation 
$(addprefix $(TESTS_DIR), tests_main.o): $(addprefix $(TESTS_DIR), tests_main.cpp)
	$(CXX) -c $(CFLAGS) $(addprefix $(TESTS_DIR), tests_main.cpp) -o $(addprefix $(TESTS_DIR), tests_main.o)

tests: $(addprefix $(TESTS_DIR), tests_main.o) $(addprefix $(TESTS_DIR), tests_main.cpp) $(addprefix $(TESTS_DIR), $(TESTS)) $(OBJECTS)
	$(CXX) $(INC) $(CFLAGS) $(addprefix $(TESTS_DIR), tests_main.o) $(OBJECTS) $(addprefix $(TESTS_DIR), $(TESTS)) -o $(addprefix $(BIN_DIR), tests) 

binaries: $(BINARIES)
targets: $(BINARIES)
exp: $(BINARIES)

$(BINARIES): $(addprefix $(TARGETS_DIR), $(TARGETS)) $(OBJECTS)
	$(CXX) $(INC) $(CFLAGS) $(OBJECTS) $(addsuffix .cpp,$(@:$(BIN_DIR)%=$(TARGETS_DIR)%)) -o $@

clean:
	rm -f $(OBJECTS); 
	rm -f $(BINARIES); 

.PHONY: clean targets binaries exp


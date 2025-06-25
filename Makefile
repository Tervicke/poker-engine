# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Get SFML flags dynamically
SFML_FLAGS := $(shell pkg-config --cflags --libs sfml-graphics)
#Get the tinyxml flags 
TINYXML_FLAGS := $(shell pkg-config --cflags --libs tinyxml2)

# Sources
SRCS = main.cpp card.cpp deck.cpp player.cpp game.cpp handrank.cpp Eval.cpp
OBJS = $(SRCS:.cpp=.o)

# Directories
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN = $(BUILD_DIR)/poker

# Object files in build/obj/
OBJS := $(addprefix $(OBJ_DIR)/, $(notdir $(OBJS)))

# Default build target
all: $(BIN) copy-assets

copy-assets:
	@mkdir -p $(BUILD_DIR)/assets
	@cp -r assets/* $(BUILD_DIR)/assets/	
	
# Link the final executable
$(BIN): $(OBJS)
	$(MAKE) copy-assets
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_FLAGS) $(TINYXML_FLAGS)

# Compile each source to object
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(SFML_FLAGS) $(TINYXML_FLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Run
run: all
	./$(BIN)

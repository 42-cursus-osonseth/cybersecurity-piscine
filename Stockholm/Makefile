CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -Iinclude -MMD -MP
LDFLAGS := -lsodium

SRC_DIR := src
OBJ_DIR := obj
BIN := stockholm

SRCS := $(SRC_DIR)/main.cpp \
        $(SRC_DIR)/FileManager.cpp \
        $(SRC_DIR)/Parser.cpp \
		$(SRC_DIR)/CryptoManager.cpp

OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

-include $(DEPS)

key:
	$(CXX) -std=c++17 -Wall -Wextra -Werror  asymKeygen.cpp -o keygen $(LDFLAGS)

.PHONY: clean fclean re

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(BIN)

re: fclean all

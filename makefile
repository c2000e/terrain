APP_SRC_DIR := src
EXT_SRC_DIR := extern/src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/app

SRC := $(wildcard $(APP_SRC_DIR)/*.c) $(wildcard $(EXT_SRC_DIR)/*.c)
SRC := $(notdir $(SRC))

OBJ := $(SRC:%.c=$(OBJ_DIR)/%.o)

$(info OBJ $(OBJ))

CPPFLAGS := -Iinclude -Iextern/include -MMD -MP
CFLAGS   := -Wall
LDFLAGS  := -Lextern/lib
LDLIBS   := -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit

vpath %.c $(APP_SRC_DIR) $(EXT_SRC_DIR)

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)

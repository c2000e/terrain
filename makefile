
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
LIB_DIR := lib
BIN_DIR := bin

ENGINE_SRC_DIR := $(SRC_DIR)/engine
ENGINE_INC_DIR := $(INC_DIR)/engine
ENGINE_OBJ_DIR := $(OBJ_DIR)/engine

ENGINE_SRC := $(wildcard $(ENGINE_SRC_DIR)/*.c)
ENGINE_OBJ := $(ENGINE_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
ENGINE_LIB := $(BIN_DIR)/libengine.a

APP_SRC_DIR := $(SRC_DIR)/app
APP_OBJ_DIR := $(OBJ_DIR)/app

APP_SRC := $(wildcard $(APP_SRC_DIR)/*.c)
APP_OBJ := $(APP_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
APP_EXE := $(BIN_DIR)/app

RES_DIR := resources
SHD_DIR := shaders
REL_DIR := release

SHD := $(wildcard $(SHD_DIR)/*.vs) $(wildcard $(SHD_DIR)/*.fs)

CPPFLAGS := -I$(INC_DIR) -MMD -MP
CFLAGS   := -Wall
LDFLAGS  := -L$(LIB_DIR) -L$(BIN_DIR)
LDLIBS   := -lengine -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit

.PHONY: all clean

all: $(APP_EXE) $(ENGINE_LIB)

$(APP_EXE): $(APP_OBJ) $(ENGINE_LIB) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $(APP_OBJ) $(LDLIBS) -o $@

$(APP_OBJ_DIR)/%.o: $(APP_SRC_DIR)/%.c | $(APP_OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(ENGINE_LIB): $(ENGINE_OBJ) | $(BIN_DIR)
	ar -r $@ $^

$(ENGINE_OBJ_DIR)/%.o: $(ENGINE_SRC_DIR)/%.c | $(ENGINE_OBJ_DIR)
	$(CC) -I$(ENGINE_INC_DIR) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR) $(APP_OBJ_DIR) $(ENGINE_OBJ_DIR):
	mkdir -p $@

release: $(EXE) $(SHD)
	mkdir -p $(REL_DIR)
	cp -r $(BIN_DIR)   $(REL_DIR)
	cp -r $(SHD_DIR)   $(REL_DIR)
	cp    $(RES_DIR)/* $(REL_DIR)
	chmod +x $(REL_DIR)/run.sh

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(ENGINE_OBJ:.o=.d)

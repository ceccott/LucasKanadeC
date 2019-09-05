CC = gcc

CC_FLAGS= -O2

# colors for terminal print
GREEN_=\033[92m
_GREEN=\033[0m

EXE = lkexe

INC_DIRS =inc . test
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(EXE)

print:
		@echo $(SRCS)

$(EXE): $(OBJS)
		@echo "$(GREEN_)linking target$(_GREEN)"
		mkdir -p bin
		$(CC) $(CC_FLAGS) $^ -o $(BIN_DIR)/$@  $(foreach L,$(LIBS),-l$L)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
		@echo "$(GREEN_)compiling target$(_GREEN)"
		mkdir -p obj
		$(CC) $(CC_FLAGS) -c $< -o $@ $(foreach D,$(INC_DIRS),-I$D)

clean:
		@echo "$(GREEN_)cleaning$(_GREEN)"
		rm -rf $(OBJS) $(BIN_DIR)/*

clear:
		@echo "$(GREEN_)clearing outputs$(_GREEN)"
		rm -rf $(BIN)/*.txt $(BIN)/*.pgm ./*.txt ./*.pgm ./out_debug

run:
		./$(BIN_DIR)/$(EXE)

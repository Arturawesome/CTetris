CC := gcc
CFLAGS := -D_POSIX_C_SOURCE=200809L -I. -Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L -I.
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LCOV_FLAGS = --no-external
# Исключаем ТОЛЬКО тестовые файлы
REMOVE_PATTERNS = "*/tests/*" "*test*.c"
CVG_NAME = coverage.info
CVG_DIR = coverage
REPORT_DIR = $(CVG_DIR)/report

OS = $(shell uname)

LDFLAGS = -lncurses -L. -ltetris -lgui
ARFLAGS = rcs
CHECK_LIB = $(shell pkg-config --cflags --libs check)

TETRIS_BIN = tetris_game
TETRIS_SRC = brick_game.c
TETRIS_OBJ = $(TETRIS_SRC:.c=.o)

TETRIS_LIB = libtetris.a
TETRIS_LIB_SRC = $(wildcard brick_game/tetris/src/*.c)
TETRIS_LIB_OBJ = $(TETRIS_LIB_SRC:.c=.o)

GUI_LIB = libgui.a
GUI_LIB_SRC = $(wildcard gui/cli/src/*.c)
GUI_LIB_OBJ = $(GUI_LIB_SRC:.c=.o)

TEST_SRC = $(wildcard tests/*.c)
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_BIN = test

.PHONY: all clean cleanObject cleanExecutable

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(TETRIS_LIB): $(TETRIS_LIB_OBJ)
	@$(AR) $(ARFLAGS) $(TETRIS_LIB) $(TETRIS_LIB_OBJ)
	@ranlib $(TETRIS_LIB)

$(GUI_LIB): $(GUI_LIB_OBJ)
	@$(AR) $(ARFLAGS) $(GUI_LIB) $(GUI_LIB_OBJ)
	@ranlib $(GUI_LIB)

$(TETRIS_BIN): $(TETRIS_OBJ) $(TETRIS_LIB) $(GUI_LIB)
	@$(CC) $(CFLAGS) $(TETRIS_OBJ) $(LDFLAGS) -o $@

$(TEST_BIN): $(TEST_SRC) $(TETRIS_LIB) $(GUI_LIB)
	@$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_SRC) $(LDFLAGS) -o $(TEST_BIN) $(CHECK_LIB)

.PHONY: gcov_report
gcov_report: CFLAGS += $(GCOV_FLAGS)
gcov_report:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) $(GCOV_FLAGS)" $(TEST_BIN)
	./$(TEST_BIN)
	mkdir -p $(CVG_DIR)
	lcov -d . -c -o $(CVG_DIR)/$(CVG_NAME)
	lcov --remove $(CVG_DIR)/$(CVG_NAME) */tests/*  -o $(CVG_DIR)/filtered_$(CVG_NAME)
	genhtml $(CVG_DIR)/filtered_$(CVG_NAME) -o $(REPORT_DIR)


rebuild: clean all
all: tetris $(TEST_BIN)

.PHONY: tetris
tetris: $(TETRIS_BIN)

cleanObject:
	rm -f *.o

cleanExecutable:
	rm -f $(TETRIS_BIN) $(TEST_BIN)
	
cleanLibs:
	rm -f $(TETRIS_LIB) $(TETRIS_BIN) $(GUI_LIB)
	
cleanObj:
	rm -f $(TETRIS_OBJ) $(TETRIS_LIB_OBJ) $(GUI_LIB_OBJ)
	
clean_gcov:
	rm -rf $(CVG_DIR) $(REPORT_DIR)
	rm -f *.gcda *.gcno *.gcov *.info
	rm -f brick_game/tetris/src/*.gcda brick_game/tetris/src/*.gcno brick_game/tetris/src/*.gcov brick_game/tetris/src/*.info
	rm -f gui/cli/src/*.gcda gui/cli/src/*.gcno gui/cli/src/*.gcov gui/cli/src/*.info
	
clean: cleanObject cleanExecutable cleanObj cleanLibs clean_gcov

#ifndef CLI_TERTIS
#define CLI_TERTIS

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../brick_game/tetris/include/figures.h"

#define FIG_COL_SCALE 2
#define FIG_ROW_SCALE 1

#define SIZE_MENU_WINDOW_COL 20
#define SIZE_MENU_WINDOW_ROW 21
#define POS_MENU_WINDOW_COL 0
#define POS_MENU_WINDOW_ROW 0

#define SIZE_FIELD_WINDOW_COL 11
#define SIZE_FIELD_WINDOW_ROW 21
#define POS_FIELD_WINDOW_COL (POS_MENU_WINDOW_COL + SIZE_MENU_WINDOW_COL)
#define POS_FIELD_WINDOW_ROW POS_MENU_WINDOW_ROW

#define SIZE_INFO_WINDOW_COL 20
#define SIZE_INFO_WINDOW_ROW 21
#define POS_INFO_WINDOW_COL \
  (POS_FIELD_WINDOW_COL + SIZE_FIELD_WINDOW_COL * FIG_COL_SCALE)
#define POS_INFO_WINDOW_ROW POS_MENU_WINDOW_ROW

#define POS_NEXT_FIG_STRING_COL 2
#define POS_NEXT_FIG_STRING_ROW 2
#define POS_NEXT_FIG_ROW (POS_NEXT_FIG_STRING_ROW + 1)
#define POS_NEXT_FIG_COL POS_NEXT_FIG_STRING_COL

#define SIZE_NEXT_WIN_ROW 2
#define SIZE_NEXT_WIN_COL 4

#define FIG_SIZE_NEXT_COL 20
#define FIG_SIZE_NEXT_ROW 2

#define NEXT_FIG_POS_COL 3

void init_ncurses();

WINDOW *init_field_window();

void refresh_field_window(WINDOW *filed_window, int **field, CurFig_info *fig);
void refresh_info_window(WINDOW *info_window, int **next, int score,
                         int msx_score, int level);
void new_refresh_info_window(int **next, int score, int msx_score, int level);
void new_refresh_field_window(int **field, const CurFig_info *curr_fig);
void new_refresh_window();
void new_init_menu_window();
#endif

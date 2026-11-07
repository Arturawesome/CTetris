#include "../include/cli.h"

void init_ncurses() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  refresh();
}

void new_refresh_window() {
  WINDOW *win =
      newwin(SIZE_MENU_WINDOW_ROW,
             SIZE_MENU_WINDOW_COL + SIZE_FIELD_WINDOW_COL * FIG_COL_SCALE +
                 SIZE_INFO_WINDOW_COL,
             POS_MENU_WINDOW_ROW, POS_MENU_WINDOW_COL);
  box(win, 0, 0);

  mvwprintw(win, 2, 2, "Nice try, body :)");
  mvwprintw(win, 4, 2, "s: for restart game");
  mvwprintw(win, 6, 2, "e: for exit");

  wrefresh(win);
}

void new_init_menu_window() {
  WINDOW *win = newwin(SIZE_MENU_WINDOW_ROW, SIZE_MENU_WINDOW_COL,
                       POS_MENU_WINDOW_ROW, POS_MENU_WINDOW_COL);
  box(win, 0, 0);

  mvwprintw(win, 2, 2, "s: start game");
  mvwprintw(win, 4, 2, "e: exit game");
  mvwprintw(win, 6, 2, "i: info game");

  wrefresh(win);
}

void refresh_cli(int **field, CurFig_info *curr_fig, int **next, int score,
                 int msx_score, int level, int choose) {
  if (choose == 0) {
    new_refresh_field_window(field, curr_fig);
  } else if (choose == 1) {
    new_refresh_info_window(next, score, msx_score, level);
  }
}

void new_refresh_info_window(int **next, int score, int msx_score, int level) {
  WINDOW *info_window = newwin(SIZE_INFO_WINDOW_ROW, SIZE_INFO_WINDOW_COL,
                               POS_INFO_WINDOW_ROW, POS_INFO_WINDOW_COL);

  box(info_window, 0, 0);

  mvwprintw(info_window, POS_NEXT_FIG_STRING_ROW, POS_NEXT_FIG_STRING_COL,
            "  Next figure: ");
  mvwprintw(info_window, POS_NEXT_FIG_STRING_ROW + 5, 2, "Score / M_Score: ");
  mvwprintw(info_window, POS_NEXT_FIG_STRING_ROW + 6, 2, "   %d / %d", score,
            msx_score);
  mvwprintw(info_window, POS_NEXT_FIG_STRING_ROW + 8, 2, "     Level: ");
  mvwprintw(info_window, POS_NEXT_FIG_STRING_ROW + 9, 2, "      %d", level);

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (next[i][j]) {
        mvwprintw(info_window, POS_NEXT_FIG_STRING_ROW + 2 + i,
                  (j + NEXT_FIG_POS_COL) * FIG_COL_SCALE, "[");
        mvwprintw(info_window, POS_NEXT_FIG_STRING_ROW + 2 + i,
                  (j + NEXT_FIG_POS_COL) * FIG_COL_SCALE + 1, "]");
      }
    }
  }

  wrefresh(info_window);
}

void new_refresh_field_window(int **field, const CurFig_info *curr_fig) {
  WINDOW *filed_window =
      newwin(SIZE_FIELD_WINDOW_ROW, SIZE_FIELD_WINDOW_COL * FIG_COL_SCALE,
             POS_FIELD_WINDOW_ROW, POS_FIELD_WINDOW_COL);
  box(filed_window, 0, 0);
  for (int i = 0; i < 20; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (field[i][j]) {
        mvwprintw(filed_window, i, 1 + FIG_COL_SCALE * j, "[");
        mvwprintw(filed_window, i, 1 + FIG_COL_SCALE * j + 1, "]");
      }
    }
  }
  Fig_ptr fig = FigsConfig();
  int col = curr_fig->col_pos;
  int row = curr_fig->row_pos;
  for (int j = 0; j < CURRENT_FIG_COL; j++) {
    int d_col = fig[curr_fig->shape][curr_fig->rot][0][j];
    int d_row = fig[curr_fig->shape][curr_fig->rot][1][j];
    int row_row = curr_fig->row_pos + d_row;
    if (row_row >= 0) {
      mvwprintw(filed_window, row + d_row,
                1 + FIG_COL_SCALE * col + FIG_COL_SCALE * d_col, "[");
      mvwprintw(filed_window, row + d_row,
                1 + FIG_COL_SCALE * col + FIG_COL_SCALE * d_col + 1, "]");
    }
  }
  wrefresh(filed_window);
}

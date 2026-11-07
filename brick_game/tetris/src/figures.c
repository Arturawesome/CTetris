#include "../include/figures.h"

FigShape_info Get_next_fig() { return (FigShape_info)(rand() % COUNT_SHAPE); }

Fig_ptr FigsConfig() {
  static Fig fig = {{// Z shape
                     {{0, 1, 1, 2}, {-1, -1, 0, 0}},
                     {{0, 0, 1, 1}, {0, -1, -1, -2}},

                     {{0, 1, 1, 2}, {-1, -1, 0, 0}},
                     {{0, 0, 1, 1}, {0, -1, -1, -2}}

                    },
                    {// S shape
                     {{0, 1, 1, 2}, {0, 0, -1, -1}},
                     {{0, 0, 1, 1}, {-1, -2, -1, 0}},
                     {{0, 1, 1, 2}, {0, 0, -1, -1}},
                     {{0, 0, 1, 1}, {-1, -2, -1, 0}}

                    },
                    {// O shape
                     {{0, 0, 1, 1}, {0, -1, -1, 0}},
                     {{0, 0, 1, 1}, {0, -1, -1, 0}},
                     {{0, 0, 1, 1}, {0, -1, -1, 0}},
                     {{0, 0, 1, 1}, {0, -1, -1, 0}}},
                    {// I shape
                     {{0, 1, 2, 3}, {0, 0, 0, 0}},
                     {{0, 0, 0, 0}, {0, -1, -2, -3}},

                     {{0, 1, 2, 3}, {0, 0, 0, 0}},
                     {{0, 0, 0, 0}, {0, -1, -2, -3}}},
                    {// L shape
                     {{0, 0, 1, 2}, {-1, 0, 0, 0}},
                     {{0, 0, 0, 1}, {0, -1, -2, -2}},

                     {{0, 1, 2, 2}, {-1, -1, -1, 0}},
                     {{0, 1, 1, 1}, {0, 0, -1, -2}}},

                    {// J shape
                     {{0, 1, 2, 2}, {0, 0, 0, -1}},
                     {{0, 0, 0, 1}, {0, -1, -2, 0}},
                     {{0, 0, 1, 2}, {0, -1, -1, -1}},
                     {{0, 1, 1, 1}, {-2, -2, -1, 0}}},
                    {// T shape
                     {{0, 1, 1, 2}, {0, 0, -1, 0}},
                     {{0, 0, 0, 1}, {0, -1, -2, -1}},
                     {{0, 1, 1, 2}, {-1, -1, 0, -1}},
                     {{0, 1, 1, 1}, {-1, 0, -1, -2}}}};

  return fig;
}

void Cpoy_fig(CurFig_info *curr_fig, FigShape_info shape) {
  // Fig_ptr fig = FigsConfig();
  curr_fig->shape = shape;
  curr_fig->rot = ROTATOR_0;
}

void move_figure_down(CurFig_info *curr_fig, int **field) {
  curr_fig->row_pos += 1;
  int flag = check_bourder_and_filed(curr_fig, field);
  if (flag == 0) {
    curr_fig->row_pos -= 1;
  }
}

int check_landing(const CurFig_info *curr_fig, int **field) {
  Fig_ptr fig = FigsConfig();
  int flag = 1;
  for (int jj = 0; jj < CURRENT_FIG_COL && flag; jj++) {
    int d_col = fig[curr_fig->shape][curr_fig->rot][0][jj];
    int d_row = fig[curr_fig->shape][curr_fig->rot][1][jj];
    int i = curr_fig->row_pos + d_row;
    if (i >= 19) {
      flag = 0;
    }
    if (flag) {
      if (i < 0) {
        continue;
      }
      int j = curr_fig->col_pos + d_col;
      if (field[i + 1][j]) {
        flag = 0;
      }
    }
  }
  return flag;
}

int fill_field_by_landing_figure(const CurFig_info *curr_fig, int **field) {
  int is_end_game = 0;
  Fig_ptr fig = FigsConfig();
  for (int jj = 0; jj < CURRENT_FIG_COL && is_end_game == 0; jj++) {
    int d_col = fig[curr_fig->shape][curr_fig->rot][0][jj];
    int d_row = fig[curr_fig->shape][curr_fig->rot][1][jj];
    int i = curr_fig->row_pos + d_row;
    int j = curr_fig->col_pos + d_col;
    if (i <= 0) {
      is_end_game = 1;
      continue;
    }
    field[i][j] = 1;
  }
  return is_end_game;
}
void new_figure_rotator(CurFig_info *curr_fig, int **field) {
  Rotator_info old_rot = curr_fig->rot;
  curr_fig->rot = (curr_fig->rot + 1) % COUN_ROTATOR;
  int flag = check_bourder_and_filed(curr_fig, field);
  if (flag == 0) {
    flag = rotator_decision(curr_fig, field);
  }
  if (flag == 0) {
    curr_fig->rot = old_rot;
  }
}
int rotator_decision(CurFig_info *curr_fig, int **field) {
  Fig_ptr fig = FigsConfig();
  int flag = 1;
  int min_j = 100;
  int max_j = -100;
  int width = 0;
  for (int jj = 0; jj < CURRENT_FIG_COL; jj++) {
    int d_col = fig[curr_fig->shape][curr_fig->rot][0][jj];
    int j = curr_fig->col_pos + d_col;
    min_j = min_j > j ? j : min_j;
    max_j = max_j < j ? j : max_j;
  }
  width = max_j - min_j;
  int i = curr_fig->row_pos;
  int start_j = curr_fig->col_pos;
  int d_j = 0;
  for (int j = start_j; j < 10 && field[i][j] == 0; j++) {
    width--;
  }
  for (int j = start_j; j >= 0 && field[i][j] == 0 && width; j--) {
    width--;
    d_j++;
  }
  d_j++;
  if (width == 0) {
    curr_fig->col_pos -= d_j;
  } else {
    flag = 0;
  }
  return flag;
}

void update_position(CurFig_info *curr_fig, int **field, int d_col, int d_row) {
  if (curr_fig == NULL || field == NULL) {
    return;
  }
  curr_fig->col_pos += d_col;
  curr_fig->row_pos += d_row;
  int flag = check_bourder_and_filed(curr_fig, field);

  if (flag == 0) {
    curr_fig->col_pos -= d_col;
    curr_fig->row_pos -= d_row;
  }
}

int check_bourder_and_filed(const CurFig_info *curr_fig, int **field) {
  Fig_ptr fig = FigsConfig();
  int flag = 1;
  for (int jj = 0; jj < CURRENT_FIG_COL && flag; jj++) {
    int d_col = fig[curr_fig->shape][curr_fig->rot][0][jj];
    int d_row = fig[curr_fig->shape][curr_fig->rot][1][jj];
    int i = curr_fig->row_pos + d_row;
    int j = curr_fig->col_pos + d_col;
    if (i >= 20) {
      flag = 0;
    }
    if (j < 0 || j >= 10) {
      flag = 0;
    }
    if (flag) {
      if (i < 0) {
        continue;
      }
      if (field[i][j]) {
        flag = 0;
      }
    }
  }
  return flag;
}

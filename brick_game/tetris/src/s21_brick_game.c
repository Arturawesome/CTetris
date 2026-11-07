#include "../include/s21_brick_game.h"

// check
void clear_fields(GameInfo_t *game_info) {
  if (game_info == NULL) {
    return;
  }
  if (game_info->field != NULL) {
    for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
      free(game_info->field[i]);
    }
    free(game_info->field);
    game_info->field = NULL;
  }
  if (game_info->next != NULL) {
    for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
      free(game_info->next[i]);
    }
    free(game_info->next);
    game_info->next = NULL;
  }
  game_info->score = 0;
  game_info->high_score = 0;
  game_info->level = 0;
  game_info->speed = 0;
  game_info->pause = 0;
  game_info->delay = 0.0;
}

void check_and_clean_line(GameInfo_t *game_info, CurFig_info *curr_fig,
                          int *is_end_round) {
  int flag = check_landing(curr_fig, game_info->field);
  if (flag == 0) {
    *is_end_round = fill_field_by_landing_figure(curr_fig, game_info->field);
    put_next_fig_in_curr(&game_info->next_fig, curr_fig);
    generate_next_figure(&game_info->next_fig, game_info->next);
  }
  int cleaned_line = clear_line(game_info->field);
  refresh_game_state(game_info, cleaned_line);
}

void get_start_command(UserAction_t *user_action) {
  int c = getch();
  set_user_action(user_action, c);
  while (*user_action != Start && *user_action != Terminate) {
    c = getch();
    set_user_action(user_action, c);
    napms(10);
  }
}

// check
void set_user_action(UserAction_t *user_action, int c) {
  switch (c) {
    case KEY_UP:
      *user_action = Up;
      break;
    case KEY_DOWN:
      *user_action = Down;
      break;
    case KEY_LEFT:
      *user_action = Left;
      break;
    case KEY_RIGHT:
      *user_action = Right;
      break;
    case 'e':
      *user_action = Terminate;
      break;
    case 'E':
      *user_action = Terminate;
      break;
    case 's':
      *user_action = Start;
      break;
    case 'S':
      *user_action = Start;
      break;
    case 'p':
      *user_action = Pause;
      break;
    case 'P':
      *user_action = Pause;
      break;
    default:
      *user_action = Calm;
  }
}

// check
void put_next_fig_in_curr(const CurFig_info *next_fig, CurFig_info *curr_fig) {
  if (curr_fig != NULL && next_fig != NULL) {
    Cpoy_fig(curr_fig, next_fig->shape);
    curr_fig->col_pos = START_CURR_FIG_COL;
    curr_fig->row_pos = START_CURR_FIG_ROW;
  }
}
// check
int move_figure(const UserAction_t *user_action, CurFig_info *curr_fig,
                GameInfo_t *game_info) {
  if (user_action == NULL || curr_fig == NULL || game_info == NULL ||
      game_info->field == NULL) {
    return 0;
  }
  int flag = 1;
  if (*user_action == Left) {
    update_position(curr_fig, game_info->field, -1, 0);
  } else if (*user_action == Right) {
    update_position(curr_fig, game_info->field, 1, 0);
  } else if (*user_action == Down) {
    update_position(curr_fig, game_info->field, 0, 1);
  } else {
    flag = 0;
  }
  return flag;
}

// check
void refresh_game_state(GameInfo_t *game_info, int cleaned_line) {
  if (cleaned_line == 0) return;

  if (cleaned_line == 1) {
    game_info->score += 100;
  } else if (cleaned_line == 2) {
    game_info->score += 300;
  } else if (cleaned_line == 3) {
    game_info->score += 700;
  } else if (cleaned_line == 4) {
    game_info->score += 1500;
  }

  int new_level = START_LEVEL + (game_info->score / 600);
  if (new_level > game_info->level) {
    game_info->level = new_level;
    game_info->speed = START_SPEED + 100 * (game_info->level - START_LEVEL);
    game_info->delay = 1.0 / game_info->speed;
  }
  if (game_info->score > game_info->high_score) {
    game_info->high_score = game_info->score;
  }
}

// check
int clear_line(int **field) {
  int counter_cleaned_line = 0;
  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    int flag = 1;
    for (int j = 0; j < SIZE_FIELD_COL && flag; j++) {
      if (field[i][j] == 0) {
        flag = 0;
      }
    }
    if (flag) {
      for (int ii = i; ii > 0; ii--) {
        for (int jj = 0; jj < SIZE_FIELD_COL; jj++) {
          field[ii][jj] = field[ii - 1][jj];
        }
      }

      for (int jj = 0; jj < SIZE_FIELD_COL; jj++) {
        field[0][jj] = 0;
      }
      counter_cleaned_line++;
    }
  }

  return counter_cleaned_line;
}

// check
void generate_next_figure(CurFig_info *next_fig, int **next) {
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      next[i][j] = 0;
    }
  }

  Fig_ptr fig = FigsConfig();
  Cpoy_fig(next_fig, Get_next_fig());
  int min_row = 100;

  for (int j = 0; j < CURRENT_FIG_COL; j++) {
    int row = fig[next_fig->shape][next_fig->rot][1][j];
    min_row = min_row > row ? row : min_row;
  }

  for (int j = 0; j < CURRENT_FIG_COL; j++) {
    int col = fig[next_fig->shape][next_fig->rot][0][j];
    int row = fig[next_fig->shape][next_fig->rot][1][j] - min_row;
    next[row][col] = 1;
  }
  return;
}

// check
void refresh_game_info_for_new_round(GameInfo_t *game_info) {
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      game_info->next[i][j] = 0;
    }
  }
  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      game_info->field[i][j] = 0;
    }
  }

  game_info->score = 0;
  game_info->speed = START_SPEED;
  game_info->delay = 1.0 / game_info->speed;
  game_info->level = 0;
}

// check
void init_game_info_struct(GameInfo_t *game_info, CurFig_info *curr_fig) {
  game_info->field = (int **)malloc((SIZE_FIELD_ROW) * sizeof(int *));
  game_info->next = (int **)malloc(FIG_SIZE_NEXT_ROW * sizeof(int *));
  for (int i = 0; i < (SIZE_FIELD_ROW); ++i) {
    game_info->field[i] = malloc(SIZE_FIELD_COL * sizeof(int));
  }
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    game_info->next[i] = malloc(FIG_SIZE_NEXT_COL * sizeof(int));
  }
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      game_info->next[i][j] = 0;
    }
  }
  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      game_info->field[i][j] = 0;
    }
  }
  game_info->score = 0;
  game_info->high_score = 0;
  game_info->speed = START_SPEED;
  game_info->delay = 1.0 / game_info->speed;
  game_info->level = 0;
  game_info->pause = 0;
  generate_next_figure(&game_info->next_fig, game_info->next);
  put_next_fig_in_curr(&game_info->next_fig, curr_fig);
}

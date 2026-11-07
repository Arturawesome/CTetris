#ifndef S21_BRICK_GAME_H
#define S21_BRICK_GAME_H

#define SIZE_FIELD_ROW 20
#define SIZE_FIELD_COL 10
#define START_SPEED 100
#define START_LEVEL 1
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../../gui/cli/include/cli.h"
#include "figures.h"

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  double delay;
  CurFig_info next_fig;
} GameInfo_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Calm,
  Action
} UserAction_t;
void init_game_info_struct(GameInfo_t *game_info, CurFig_info *curr_fig);

void set_user_action(UserAction_t *user_action, int c);

int clear_line(int **field);
void refresh_game_state(GameInfo_t *game_info, int cleaned_line);

void generate_next_figure(CurFig_info *next_fig, int **next);
void put_next_fig_in_curr(const CurFig_info *next_fig, CurFig_info *curr_fig);
void refresh_game_info_for_new_round(GameInfo_t *game_info);
void get_start_command(UserAction_t *user_action);
void get_command(UserAction_t *user_action);
void check_and_clean_line(GameInfo_t *game_info, CurFig_info *curr_fig,
                          int *is_end_round);

int move_figure(const UserAction_t *user_action, CurFig_info *curr_fig,
                GameInfo_t *game_info);
void clear_fields(GameInfo_t *game_info);
#endif  // S21_BRICK_GAME_H

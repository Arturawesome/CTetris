#ifndef _BRICK_GAME_H_
#define _BRICK_GAME_H_
#include <stdio.h>
#include <stdlib.h>

#include "brick_game/tetris/include/s21_brick_game.h"
#include "gui/cli/include/cli.h"

void new_spawn(GameInfo_t *game_info, UserAction_t *user_action,
               CurFig_info *curr_fig);
void restart_game(GameInfo_t *game_info, const UserAction_t *user_action,
                  CurFig_info *curr_fig);
#endif
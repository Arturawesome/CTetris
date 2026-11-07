#ifndef _TETRIS_TESTS_H_
#define _TETRIS_TESTS_H_

#include <check.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../brick_game.h"
Suite *init_game_info_suite(void);
Suite *clear_fields_suite(void);
Suite *clear_line_suite(void);
Suite *generate_next_figure_suite(void);
Suite *refresh_game_info_suite(void);
Suite *refresh_game_state_suite(void);
Suite *put_next_fig_suite(void);
Suite *get_command_suite(void);
Suite *move_figure_suite(void);
Suite *new_figure_rotator_suite(void);
Suite *fill_field_by_landing_figure_suite(void);

#endif

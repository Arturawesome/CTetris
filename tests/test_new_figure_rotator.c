#include "tetris_test.h"

START_TEST(test_new_figure_rotator_only_rot_changes_before_checks) {
  GameInfo_t game_info = {0};

  CurFig_info curr_fig = {0};
  init_game_info_struct(&game_info, &curr_fig);

  curr_fig.shape = O_SHAPE;
  curr_fig.rot = ROTATOR_90;
  curr_fig.row_pos = 3;
  curr_fig.col_pos = 4;

  new_figure_rotator(&curr_fig, game_info.field);
  ck_assert_int_eq(curr_fig.rot, ROTATOR_180);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_new_figure_rotator_edge_cases_rot_values) {
  GameInfo_t game_info = {0};

  CurFig_info curr_fig = {0};
  init_game_info_struct(&game_info, &curr_fig);

  curr_fig.shape = Z_SHAPE;
  curr_fig.rot = ROTATOR_0;
  curr_fig.row_pos = 3;
  curr_fig.col_pos = 4;

  new_figure_rotator(&curr_fig, game_info.field);
  ck_assert_int_eq(curr_fig.rot, ROTATOR_90);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_new_figure_rotator_invalid_rot_value) {
  GameInfo_t game_info = {0};

  CurFig_info curr_fig = {0};
  init_game_info_struct(&game_info, &curr_fig);

  curr_fig.shape = L_SHAPE;
  curr_fig.rot = ROTATOR_180;
  curr_fig.row_pos = 19;
  curr_fig.col_pos = 0;

  new_figure_rotator(&curr_fig, game_info.field);
  ck_assert_int_eq(curr_fig.rot, ROTATOR_270);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_new_figure_rotator_negative_rot_value) {
  GameInfo_t game_info = {0};

  CurFig_info curr_fig = {0};
  init_game_info_struct(&game_info, &curr_fig);

  curr_fig.shape = I_SHAPE;
  curr_fig.rot = ROTATOR_180;
  curr_fig.row_pos = 3;
  curr_fig.col_pos = 9;

  game_info.field[1][2] = 1;
  game_info.field[2][2] = 1;
  game_info.field[2][2] = 1;
  game_info.field[3][2] = 1;
  game_info.field[4][2] = 1;
  game_info.field[5][2] = 1;

  game_info.field[1][4] = 1;
  game_info.field[2][4] = 1;
  game_info.field[3][4] = 1;
  game_info.field[4][4] = 1;
  game_info.field[5][4] = 1;

  new_figure_rotator(&curr_fig, game_info.field);
  ck_assert_int_eq(curr_fig.rot, ROTATOR_270);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_new_figure_rotator_conditional_logic) {
  GameInfo_t game_info = {0};

  CurFig_info curr_fig = {0};
  init_game_info_struct(&game_info, &curr_fig);

  curr_fig.shape = T_SHAPE;
  curr_fig.rot = ROTATOR_0;
  curr_fig.row_pos = 10;
  curr_fig.col_pos = 9;

  new_figure_rotator(&curr_fig, game_info.field);
  new_figure_rotator(&curr_fig, game_info.field);
  ck_assert_int_eq(curr_fig.rot, ROTATOR_180);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_new_figure_rotator_multiple_calls_same_state) {
  GameInfo_t game_info = {0};

  CurFig_info curr_fig = {0};
  init_game_info_struct(&game_info, &curr_fig);

  curr_fig.shape = S_SHAPE;
  curr_fig.rot = ROTATOR_0;
  curr_fig.row_pos = 10;
  curr_fig.col_pos = 3;

  new_figure_rotator(&curr_fig, game_info.field);
  new_figure_rotator(&curr_fig, game_info.field);
  new_figure_rotator(&curr_fig, game_info.field);
  ck_assert_int_eq(curr_fig.rot, ROTATOR_270);
  clear_fields(&game_info);
}
END_TEST

Suite *new_figure_rotator_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("NewFigureRotator");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core,
                 test_new_figure_rotator_only_rot_changes_before_checks);
  tcase_add_test(tc_core, test_new_figure_rotator_edge_cases_rot_values);
  tcase_add_test(tc_core, test_new_figure_rotator_multiple_calls_same_state);
  tcase_add_test(tc_core, test_new_figure_rotator_conditional_logic);
  tcase_add_test(tc_core, test_new_figure_rotator_invalid_rot_value);
  tcase_add_test(tc_core, test_new_figure_rotator_negative_rot_value);
  suite_add_tcase(s, tc_core);

  return s;
}

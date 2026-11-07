#include "tetris_test.h"

int count_ones_in_field(int **field) {
  int count = 0;
  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      if (field[i][j] == 1) {
        count++;
      }
    }
  }
  return count;
}

START_TEST(test_fill_field_normal_landing) {
  GameInfo_t game_info = {0};
  CurFig_info curr_fig = {0};
  init_game_info_struct(&game_info, &curr_fig);

  curr_fig.shape = I_SHAPE;
  curr_fig.rot = ROTATOR_0;
  curr_fig.row_pos = 5;
  curr_fig.col_pos = 3;

  int result = fill_field_by_landing_figure(&curr_fig, game_info.field);
  ck_assert_int_eq(result, 0);

  ck_assert_int_eq(count_ones_in_field(game_info.field), 4);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_fill_field_end_game_detected) {
  GameInfo_t game_info = {0};

  CurFig_info curr_fig = {0};
  init_game_info_struct(&game_info, &curr_fig);

  curr_fig.shape = I_SHAPE;
  curr_fig.rot = ROTATOR_0;
  curr_fig.row_pos = 0;
  curr_fig.col_pos = 0;
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      game_info.field[i][j] = 1;
    }
  }

  int result = fill_field_by_landing_figure(&curr_fig, game_info.field);
  ck_assert_int_eq(result, 1);

  ck_assert_int_eq(game_info.field[5][3], 1);
  ck_assert_int_eq(game_info.field[6][3], 1);
  ck_assert_int_eq(game_info.field[7][3], 1);
  ck_assert_int_eq(game_info.field[8][3], 1);

  ck_assert_int_eq(count_ones_in_field(game_info.field), 200);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_down_figure) {
  GameInfo_t game_info = {0};
  CurFig_info curr_fig = {0};
  init_game_info_struct(&game_info, &curr_fig);

  curr_fig.shape = Z_SHAPE;
  curr_fig.rot = ROTATOR_0;
  curr_fig.row_pos = 19;
  curr_fig.col_pos = 3;

  int result = fill_field_by_landing_figure(&curr_fig, game_info.field);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(count_ones_in_field(game_info.field), 4);
  clear_fields(&game_info);
}
END_TEST

START_TEST(check_landing1) {
  GameInfo_t game_info = {0};
  CurFig_info curr_fig = {0};
  init_game_info_struct(&game_info, &curr_fig);

  curr_fig.shape = I_SHAPE;
  curr_fig.rot = ROTATOR_0;
  curr_fig.row_pos = 5;
  curr_fig.col_pos = 3;

  int result = check_landing(&curr_fig, game_info.field);
  ck_assert_int_eq(result, 1);
  clear_fields(&game_info);
}
END_TEST

Suite *fill_field_by_landing_figure_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("FillFieldByLandingFigure");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_fill_field_normal_landing);
  tcase_add_test(tc_core, test_fill_field_end_game_detected);
  tcase_add_test(tc_core, test_down_figure);
  tcase_add_test(tc_core, check_landing1);

  suite_add_tcase(s, tc_core);
  return s;
}

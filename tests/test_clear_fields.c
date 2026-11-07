#include "tetris_test.h"

START_TEST(test_clear_fields_basic) {
  GameInfo_t game_info;
  CurFig_info curr_fig;

  init_game_info_struct(&game_info, &curr_fig);

  ck_assert_ptr_nonnull(game_info.field);
  ck_assert_ptr_nonnull(game_info.next);

  game_info.score = 100;
  game_info.high_score = 500;
  game_info.level = 3;
  game_info.speed = 150;
  game_info.pause = 1;
  game_info.delay = 0.5;

  clear_fields(&game_info);

  ck_assert_ptr_null(game_info.field);
  ck_assert_ptr_null(game_info.next);

  ck_assert_int_eq(game_info.score, 0);
  ck_assert_int_eq(game_info.high_score, 0);
  ck_assert_int_eq(game_info.level, 0);
  ck_assert_int_eq(game_info.speed, 0);
  ck_assert_int_eq(game_info.pause, 0);
  ck_assert_double_eq_tol(game_info.delay, 0.0, 0.0001);
}
END_TEST

START_TEST(test_clear_fields_multiple_calls) {
  GameInfo_t game_info;
  CurFig_info curr_fig;

  init_game_info_struct(&game_info, &curr_fig);

  clear_fields(&game_info);

  ck_assert_ptr_null(game_info.field);
  ck_assert_ptr_null(game_info.next);

  clear_fields(&game_info);

  ck_assert_ptr_null(game_info.field);
  ck_assert_ptr_null(game_info.next);
  ck_assert_int_eq(game_info.score, 0);

  clear_fields(&game_info);
  ck_assert_int_eq(game_info.score, 0);
}
END_TEST

START_TEST(test_clear_fields_partial_null) {
  GameInfo_t game_info;
  CurFig_info curr_fig;

  init_game_info_struct(&game_info, &curr_fig);

  if (game_info.field != NULL) {
    for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
      free(game_info.field[i]);
    }
    free(game_info.field);
    game_info.field = NULL;
  }

  game_info.score = 200;
  game_info.high_score = 300;

  clear_fields(&game_info);

  ck_assert_ptr_null(game_info.field);
  ck_assert_ptr_null(game_info.next);
  ck_assert_int_eq(game_info.score, 0);
  ck_assert_int_eq(game_info.high_score, 0);
}
END_TEST

START_TEST(test_clear_fields_null_game_info) {
  GameInfo_t game_info;

  game_info.field = NULL;
  game_info.next = NULL;
  game_info.score = 123;
  game_info.high_score = 456;
  game_info.level = 2;
  game_info.speed = 100;
  game_info.pause = 1;
  game_info.delay = 0.1;

  clear_fields(&game_info);

  ck_assert_int_eq(game_info.score, 0);
  ck_assert_int_eq(game_info.high_score, 0);
  ck_assert_int_eq(game_info.level, 0);
  ck_assert_int_eq(game_info.speed, 0);
  ck_assert_int_eq(game_info.pause, 0);
  ck_assert_double_eq_tol(game_info.delay, 0.0, 0.0001);
}
END_TEST

START_TEST(test_clear_fields_memory_leak_check) {
  GameInfo_t game_info;
  CurFig_info curr_fig;

  init_game_info_struct(&game_info, &curr_fig);

  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      game_info.field[i][j] = (i + j) % 2;
    }
  }

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      game_info.next[i][j] = 1;
    }
  }

  game_info.score = 999;
  game_info.high_score = 1000;
  game_info.level = 10;
  game_info.speed = 200;
  game_info.pause = 1;
  game_info.delay = 0.005;

  clear_fields(&game_info);

  ck_assert_ptr_null(game_info.field);
  ck_assert_ptr_null(game_info.next);
  ck_assert_int_eq(game_info.score, 0);
  ck_assert_int_eq(game_info.high_score, 0);
  ck_assert_int_eq(game_info.level, 0);
  ck_assert_int_eq(game_info.speed, 0);
  ck_assert_int_eq(game_info.pause, 0);
  ck_assert_double_eq_tol(game_info.delay, 0.0, 0.0001);
}
END_TEST

START_TEST(test_clear_fields_after_reinit) {
  GameInfo_t game_info;
  CurFig_info curr_fig;

  init_game_info_struct(&game_info, &curr_fig);

  clear_fields(&game_info);

  init_game_info_struct(&game_info, &curr_fig);

  ck_assert_ptr_nonnull(game_info.field);
  ck_assert_ptr_nonnull(game_info.next);
  ck_assert_int_eq(game_info.score, 0);

  game_info.score = 777;

  clear_fields(&game_info);

  ck_assert_ptr_null(game_info.field);
  ck_assert_ptr_null(game_info.next);
  ck_assert_int_eq(game_info.score, 0);
}
END_TEST

Suite *clear_fields_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("ClearFields");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_clear_fields_basic);
  tcase_add_test(tc_core, test_clear_fields_multiple_calls);
  tcase_add_test(tc_core, test_clear_fields_partial_null);
  tcase_add_test(tc_core, test_clear_fields_null_game_info);
  tcase_add_test(tc_core, test_clear_fields_memory_leak_check);
  tcase_add_test(tc_core, test_clear_fields_after_reinit);

  suite_add_tcase(s, tc_core);

  return s;
}

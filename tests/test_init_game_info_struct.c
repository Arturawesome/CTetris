#include "tetris_test.h"

START_TEST(test_init_game_info_struct_basic) {
  GameInfo_t game_info;
  CurFig_info curr_fig;

  init_game_info_struct(&game_info, &curr_fig);

  ck_assert_ptr_nonnull(game_info.field);
  ck_assert_ptr_nonnull(game_info.next);

  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    ck_assert_ptr_nonnull(game_info.field[i]);
  }

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    ck_assert_ptr_nonnull(game_info.next[i]);
  }

  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      ck_assert_int_eq(game_info.field[i][j], 0);
    }
  }

  int sum_next = 0;
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      if (game_info.next[i][j] == 1) {
        sum_next++;
      }
    }
  }
  ck_assert_int_eq(sum_next, 4);

  ck_assert_int_eq(game_info.score, 0);
  ck_assert_int_eq(game_info.high_score, 0);
  ck_assert_int_eq(game_info.speed, START_SPEED);
  ck_assert_int_eq(game_info.level, 0);
  ck_assert(game_info.delay > 0.0);

  ck_assert_int_ge(curr_fig.shape, 0);
  ck_assert_int_le(curr_fig.shape, COUNT_SHAPE - 1);

  clear_fields(&game_info);
}
END_TEST

START_TEST(test_init_game_info_struct_double_init) {
  GameInfo_t game_info;
  CurFig_info curr_fig;

  init_game_info_struct(&game_info, &curr_fig);

  int **field_ptr = game_info.field;
  int **next_ptr = game_info.next;

  init_game_info_struct(&game_info, &curr_fig);

  ck_assert_ptr_ne(field_ptr, game_info.field);
  ck_assert_ptr_ne(next_ptr, game_info.next);

  clear_fields(&game_info);
}
END_TEST

START_TEST(test_init_game_info_struct_memory_failure) {
  GameInfo_t game_info;
  CurFig_info curr_fig;

  init_game_info_struct(&game_info, &curr_fig);

  ck_assert_ptr_nonnull(game_info.field);
  ck_assert_ptr_nonnull(game_info.next);

  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    ck_assert_ptr_nonnull(game_info.field[i]);
  }

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    ck_assert_ptr_nonnull(game_info.next[i]);
  }

  clear_fields(&game_info);
}
END_TEST

START_TEST(test_init_game_info_struct_default_values) {
  GameInfo_t game_info;
  CurFig_info curr_fig;

  init_game_info_struct(&game_info, &curr_fig);
  ck_assert_int_eq(game_info.speed, START_SPEED);
  ck_assert_int_eq(game_info.pause, 0);

  clear_fields(&game_info);
}
END_TEST

START_TEST(test_init_game_info_struct_next_figure_generated) {
  GameInfo_t game_info;
  CurFig_info curr_fig;

  init_game_info_struct(&game_info, &curr_fig);

  ck_assert_int_ge(game_info.next_fig.shape, 0);
  ck_assert_int_le(game_info.next_fig.shape, COUNT_SHAPE - 1);
  ck_assert_int_ge(game_info.next_fig.rot, 0);
  ck_assert_int_le(game_info.next_fig.rot, COUN_ROTATOR - 1);

  int has_non_zero = 0;
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      if (game_info.next[i][j] != 0) {
        has_non_zero = 1;
        break;
      }
    }
    if (has_non_zero) break;
  }
  ck_assert_int_eq(has_non_zero, 1);

  clear_fields(&game_info);
}
END_TEST

Suite *init_game_info_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("InitGameInfoStruct");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_init_game_info_struct_basic);
  tcase_add_test(tc_core, test_init_game_info_struct_double_init);
  tcase_add_test(tc_core, test_init_game_info_struct_memory_failure);
  tcase_add_test(tc_core, test_init_game_info_struct_default_values);
  tcase_add_test(tc_core, test_init_game_info_struct_next_figure_generated);

  suite_add_tcase(s, tc_core);

  return s;
}

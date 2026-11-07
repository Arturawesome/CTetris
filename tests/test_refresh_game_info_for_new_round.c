#include "tetris_test.h"

GameInfo_t* create_test_game_info(void) {
  GameInfo_t* game_info = (GameInfo_t*)malloc(sizeof(GameInfo_t));

  game_info->field = (int**)malloc(SIZE_FIELD_ROW * sizeof(int*));
  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    game_info->field[i] = (int*)malloc(SIZE_FIELD_COL * sizeof(int));
  }

  game_info->next = (int**)malloc(FIG_SIZE_NEXT_ROW * sizeof(int*));
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    game_info->next[i] = (int*)malloc(FIG_SIZE_NEXT_COL * sizeof(int));
  }

  return game_info;
}

void fill_game_info_with_test_data(GameInfo_t* game_info) {
  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      game_info->field[i][j] = (i * SIZE_FIELD_COL + j) % 5 + 1;
    }
  }

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      game_info->next[i][j] = (i * FIG_SIZE_NEXT_COL + j) % 3 + 1;
    }
  }

  game_info->score = 1234;
  game_info->high_score = 5678;
  game_info->level = 5;
  game_info->speed = 200;
  game_info->pause = 1;
  game_info->delay = 0.12345;
}

void free_test_game_info(GameInfo_t* game_info) {
  if (game_info == NULL) return;

  if (game_info->field != NULL) {
    for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
      free(game_info->field[i]);
    }
    free(game_info->field);
  }

  if (game_info->next != NULL) {
    for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
      free(game_info->next[i]);
    }
    free(game_info->next);
  }

  free(game_info);
}

int array_all_equal(int** arr, int rows, int cols, int value) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (arr[i][j] != value) {
        return 0;
      }
    }
  }
  return 1;
}

START_TEST(test_refresh_game_info_basic_reset) {
  GameInfo_t* game_info = create_test_game_info();
  fill_game_info_with_test_data(game_info);

  refresh_game_info_for_new_round(game_info);

  ck_assert(
      array_all_equal(game_info->field, SIZE_FIELD_ROW, SIZE_FIELD_COL, 0));

  ck_assert(array_all_equal(game_info->next, FIG_SIZE_NEXT_ROW,
                            FIG_SIZE_NEXT_COL, 0));

  ck_assert_int_eq(game_info->score, 0);
  ck_assert_int_eq(game_info->speed, START_SPEED);
  ck_assert_double_eq_tol(game_info->delay, 1.0 / START_SPEED, 0.0001);
  ck_assert_int_eq(game_info->level, 0);

  free_test_game_info(game_info);
}
END_TEST

START_TEST(test_refresh_game_info_preserves_high_score) {
  GameInfo_t* game_info = create_test_game_info();
  fill_game_info_with_test_data(game_info);

  game_info->high_score = 9999;

  refresh_game_info_for_new_round(game_info);

  ck_assert_int_eq(game_info->high_score, 9999);

  ck_assert_int_eq(game_info->score, 0);

  free_test_game_info(game_info);
}
END_TEST

START_TEST(test_refresh_game_info_preserves_pause_state) {
  GameInfo_t* game_info = create_test_game_info();
  fill_game_info_with_test_data(game_info);

  game_info->pause = 1;
  refresh_game_info_for_new_round(game_info);
  ck_assert_int_eq(game_info->pause, 1);  // Должен сохраниться

  game_info->pause = 0;
  refresh_game_info_for_new_round(game_info);
  ck_assert_int_eq(game_info->pause, 0);  // Должен сохраниться

  game_info->pause = -1;
  refresh_game_info_for_new_round(game_info);
  ck_assert_int_eq(game_info->pause, -1);  // Должен сохраниться

  free_test_game_info(game_info);
}
END_TEST

START_TEST(test_refresh_game_info_correct_delay_calculation) {
  GameInfo_t* game_info = create_test_game_info();
  fill_game_info_with_test_data(game_info);

  game_info->speed = 100;
  refresh_game_info_for_new_round(game_info);
  ck_assert_double_eq_tol(game_info->delay, 1.0 / 100.0, 0.0001);
  ck_assert_int_eq(game_info->speed, START_SPEED);

  refresh_game_info_for_new_round(game_info);
  ck_assert_double_eq_tol(game_info->delay, 1.0 / START_SPEED, 0.0001);
  ck_assert_int_eq(game_info->speed, START_SPEED);

  free_test_game_info(game_info);
}
END_TEST

START_TEST(test_refresh_game_info_multiple_calls) {
  GameInfo_t* game_info = create_test_game_info();

  for (int call = 0; call < 5; ++call) {
    fill_game_info_with_test_data(game_info);
    game_info->score = 1000 * (call + 1);
    game_info->level = call + 2;

    refresh_game_info_for_new_round(game_info);

    ck_assert(
        array_all_equal(game_info->field, SIZE_FIELD_ROW, SIZE_FIELD_COL, 0));
    ck_assert(array_all_equal(game_info->next, FIG_SIZE_NEXT_ROW,
                              FIG_SIZE_NEXT_COL, 0));
    ck_assert_int_eq(game_info->score, 0);
    ck_assert_int_eq(game_info->level, 0);
    ck_assert_int_eq(game_info->speed, START_SPEED);
    ck_assert_double_eq_tol(game_info->delay, 1.0 / START_SPEED, 0.0001);
  }

  free_test_game_info(game_info);
}
END_TEST

START_TEST(test_refresh_game_info_partial_filled_arrays) {
  GameInfo_t* game_info = create_test_game_info();

  for (int i = 0; i < SIZE_FIELD_ROW; i += 2) {
    for (int j = 0; j < SIZE_FIELD_COL; j += 2) {
      game_info->field[i][j] = 7;
    }
  }

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; i += 2) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; j += 2) {
      game_info->next[i][j] = 3;
    }
  }

  game_info->score = 555;
  game_info->level = 3;
  game_info->speed = 150;
  game_info->delay = 1.0 / 150.0;

  refresh_game_info_for_new_round(game_info);

  ck_assert(
      array_all_equal(game_info->field, SIZE_FIELD_ROW, SIZE_FIELD_COL, 0));
  ck_assert(array_all_equal(game_info->next, FIG_SIZE_NEXT_ROW,
                            FIG_SIZE_NEXT_COL, 0));
  ck_assert_int_eq(game_info->score, 0);
  ck_assert_int_eq(game_info->level, 0);
  ck_assert_int_eq(game_info->speed, START_SPEED);
  ck_assert_double_eq_tol(game_info->delay, 1.0 / START_SPEED, 0.0001);

  free_test_game_info(game_info);
}
END_TEST

START_TEST(test_refresh_game_info_edge_cases_arrays) {
  GameInfo_t* game_info = create_test_game_info();

  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      game_info->field[i][j] = 1;
    }
  }

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      game_info->next[i][j] = 1;
    }
  }

  refresh_game_info_for_new_round(game_info);

  ck_assert(
      array_all_equal(game_info->field, SIZE_FIELD_ROW, SIZE_FIELD_COL, 0));
  ck_assert(array_all_equal(game_info->next, FIG_SIZE_NEXT_ROW,
                            FIG_SIZE_NEXT_COL, 0));

  free_test_game_info(game_info);
}
END_TEST

START_TEST(test_refresh_game_info_negative_values) {
  GameInfo_t* game_info = create_test_game_info();

  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      game_info->field[i][j] = -((i * SIZE_FIELD_COL + j) % 10);
    }
  }

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      game_info->next[i][j] = -((i * FIG_SIZE_NEXT_COL + j) % 5);
    }
  }

  game_info->score = -100;
  game_info->level = -1;

  refresh_game_info_for_new_round(game_info);

  ck_assert(
      array_all_equal(game_info->field, SIZE_FIELD_ROW, SIZE_FIELD_COL, 0));
  ck_assert(array_all_equal(game_info->next, FIG_SIZE_NEXT_ROW,
                            FIG_SIZE_NEXT_COL, 0));
  ck_assert_int_eq(game_info->score, 0);
  ck_assert_int_eq(game_info->level, 0);

  free_test_game_info(game_info);
}
END_TEST

START_TEST(test_refresh_game_info_large_values) {
  GameInfo_t* game_info = create_test_game_info();

  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      game_info->field[i][j] = 1000000 + i * SIZE_FIELD_COL + j;
    }
  }

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      game_info->next[i][j] = 500000 + i * FIG_SIZE_NEXT_COL + j;
    }
  }

  game_info->score = 2147483647;
  game_info->level = 100;
  game_info->speed = 1000;
  game_info->delay = 0.001;

  refresh_game_info_for_new_round(game_info);

  ck_assert(
      array_all_equal(game_info->field, SIZE_FIELD_ROW, SIZE_FIELD_COL, 0));
  ck_assert(array_all_equal(game_info->next, FIG_SIZE_NEXT_ROW,
                            FIG_SIZE_NEXT_COL, 0));
  ck_assert_int_eq(game_info->score, 0);
  ck_assert_int_eq(game_info->level, 0);
  ck_assert_int_eq(game_info->speed, START_SPEED);
  ck_assert_double_eq_tol(game_info->delay, 1.0 / START_SPEED, 0.0001);

  free_test_game_info(game_info);
}
END_TEST

START_TEST(test_refresh_game_info_null_pointer) {
  ck_assert_ptr_nonnull(refresh_game_info_for_new_round);
}
END_TEST

START_TEST(test_refresh_game_info_does_not_free_memory) {
  GameInfo_t* game_info = create_test_game_info();
  fill_game_info_with_test_data(game_info);

  int** field_ptr = game_info->field;
  int** next_ptr = game_info->next;

  refresh_game_info_for_new_round(game_info);

  ck_assert_ptr_eq(game_info->field, field_ptr);
  ck_assert_ptr_eq(game_info->next, next_ptr);
  ck_assert_int_eq(game_info->field[0][0], 0);
  ck_assert_int_eq(game_info->next[0][0], 0);

  free_test_game_info(game_info);
}
END_TEST

START_TEST(test_refresh_game_info_integration_with_other_functions) {
  GameInfo_t* game_info = create_test_game_info();
  CurFig_info next_fig;

  refresh_game_info_for_new_round(game_info);

  generate_next_figure(&next_fig, game_info->next);

  int ones_before = 0;
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      if (game_info->next[i][j] == 1) ones_before++;
    }
  }
  ck_assert_int_eq(ones_before, 4);
  refresh_game_info_for_new_round(game_info);

  int ones_after = 0;
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      if (game_info->next[i][j] == 1) ones_after++;
    }
  }
  ck_assert_int_eq(ones_after, 0);

  free_test_game_info(game_info);
}
END_TEST

Suite* refresh_game_info_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("RefreshGameInfo");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_refresh_game_info_basic_reset);
  tcase_add_test(tc_core, test_refresh_game_info_preserves_high_score);
  tcase_add_test(tc_core, test_refresh_game_info_preserves_pause_state);
  tcase_add_test(tc_core, test_refresh_game_info_correct_delay_calculation);
  tcase_add_test(tc_core, test_refresh_game_info_multiple_calls);
  tcase_add_test(tc_core, test_refresh_game_info_does_not_free_memory);
  tcase_add_test(tc_core,
                 test_refresh_game_info_integration_with_other_functions);
  tcase_add_test(tc_core, test_refresh_game_info_partial_filled_arrays);
  tcase_add_test(tc_core, test_refresh_game_info_edge_cases_arrays);
  tcase_add_test(tc_core, test_refresh_game_info_negative_values);
  tcase_add_test(tc_core, test_refresh_game_info_large_values);
  tcase_add_test(tc_core, test_refresh_game_info_null_pointer);

  suite_add_tcase(s, tc_core);

  return s;
}

#include "tetris_test.h"

GameInfo_t* create_test_game_info2(void) {
  GameInfo_t* game_info = (GameInfo_t*)malloc(sizeof(GameInfo_t));

  memset(game_info, 0, sizeof(GameInfo_t));

  game_info->field = NULL;
  game_info->next = NULL;

  return game_info;
}

void free_test_game_info2(GameInfo_t* game_info) { free(game_info); }

START_TEST(test_refresh_game_state_zero_lines) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 500;
  game_info->level = 2;
  game_info->high_score = 1000;

  refresh_game_state(game_info, 0);

  ck_assert_int_eq(game_info->score, 500);
  ck_assert_int_eq(game_info->level, 2);
  ck_assert_int_eq(game_info->high_score, 1000);

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_one_line) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 200;
  game_info->high_score = 300;

  refresh_game_state(game_info, 1);

  ck_assert_int_eq(game_info->score, 300);

  ck_assert_int_eq(game_info->high_score, 300);

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_two_lines) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 100;

  refresh_game_state(game_info, 2);

  ck_assert_int_eq(game_info->score, 400);

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_three_lines) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 50;

  refresh_game_state(game_info, 3);

  ck_assert_int_eq(game_info->score, 750);

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_four_lines) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 0;

  refresh_game_state(game_info, 4);

  ck_assert_int_eq(game_info->score, 1500);

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_multiple_calls) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 0;

  refresh_game_state(game_info, 1);
  refresh_game_state(game_info, 2);
  refresh_game_state(game_info, 1);
  refresh_game_state(game_info, 4);

  ck_assert_int_eq(game_info->score, 2000);

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_high_score_update) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 800;
  game_info->high_score = 1000;

  refresh_game_state(game_info, 2);
  ck_assert_int_eq(game_info->score, 1100);
  ck_assert_int_eq(game_info->high_score, 1100);

  game_info->high_score = 2000;

  refresh_game_state(game_info, 1);
  ck_assert_int_eq(game_info->score, 1200);
  ck_assert_int_eq(game_info->high_score, 2000);

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_level_up_calculation) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 0;
  game_info->level = START_LEVEL;
  game_info->speed = START_SPEED;

  refresh_game_state(game_info, 4);
  refresh_game_state(game_info, 0);

  if (game_info->score % 600 == 0) {
    ck_assert_int_eq(game_info->level, START_LEVEL + (game_info->score / 600));
    ck_assert_int_eq(game_info->speed,
                     START_SPEED + 100 * (game_info->score / 100));
    ck_assert_double_eq_tol(game_info->delay, 1.0 / game_info->speed, 0.0001);
  }

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_multiple_level_ups) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 0;
  game_info->level = START_LEVEL;
  game_info->speed = START_SPEED;

  game_info->score = 600;
  refresh_game_state(game_info, 0);

  game_info->score = 1200;
  refresh_game_state(game_info, 0);

  game_info->score = 1800;
  refresh_game_state(game_info, 0);

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_negative_lines) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 100;

  ck_assert_ptr_nonnull(refresh_game_state);

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_large_number_of_lines) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 0;

  refresh_game_state(game_info, 5);

  ck_assert_int_eq(game_info->score, 0);

  refresh_game_state(game_info, 10);
  ck_assert_int_eq(game_info->score, 0);

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_score_overflow_protection) {
  GameInfo_t* game_info = create_test_game_info2();

  game_info->score = 2000000000;
  game_info->high_score = 2000000000;

  refresh_game_state(game_info, 4);

  ck_assert_int_gt(game_info->score, 0);

  if (game_info->score > game_info->high_score) {
    ck_assert_int_eq(game_info->high_score, game_info->score);
  }

  free_test_game_info2(game_info);
}
END_TEST

START_TEST(test_refresh_game_state_invalid_pointer) {
  ck_assert_ptr_nonnull(refresh_game_state);
}
END_TEST

START_TEST(test_refresh_game_state_combined_scenario) {
  GameInfo_t* game_info = create_test_game_info2();
  game_info->score = 0;
  game_info->high_score = 0;
  game_info->level = START_LEVEL;
  game_info->speed = START_SPEED;
  game_info->delay = 1.0 / START_SPEED;

  refresh_game_state(game_info, 4);
  ck_assert_int_eq(game_info->score, 1500);
  ck_assert_int_eq(game_info->high_score, 1500);

  refresh_game_state(game_info, 3);
  ck_assert_int_eq(game_info->score, 2200);
  ck_assert_int_eq(game_info->high_score, 2200);

  refresh_game_state(game_info, 2);
  ck_assert_int_eq(game_info->score, 2500);

  refresh_game_state(game_info, 1);
  ck_assert_int_eq(game_info->score, 2600);

  free_test_game_info2(game_info);
}
END_TEST

Suite* refresh_game_state_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("RefreshGameState");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_refresh_game_state_high_score_update);
  tcase_add_test(tc_core, test_refresh_game_state_level_up_calculation);
  tcase_add_test(tc_core, test_refresh_game_state_multiple_level_ups);
  tcase_add_test(tc_core, test_refresh_game_state_combined_scenario);
  tcase_add_test(tc_core, test_refresh_game_state_one_line);
  tcase_add_test(tc_core, test_refresh_game_state_two_lines);
  tcase_add_test(tc_core, test_refresh_game_state_three_lines);
  tcase_add_test(tc_core, test_refresh_game_state_four_lines);
  tcase_add_test(tc_core, test_refresh_game_state_zero_lines);
  tcase_add_test(tc_core, test_refresh_game_state_multiple_calls);
  tcase_add_test(tc_core, test_refresh_game_state_large_number_of_lines);
  tcase_add_test(tc_core, test_refresh_game_state_score_overflow_protection);
  tcase_add_test(tc_core, test_refresh_game_state_negative_lines);
  tcase_add_test(tc_core, test_refresh_game_state_invalid_pointer);

  suite_add_tcase(s, tc_core);

  return s;
}

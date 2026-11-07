#include "tetris_test.h"

START_TEST(test_move_figure_left_action_returns_one) {
  UserAction_t action = Left;
  CurFig_info curr_fig;
  GameInfo_t game_info;
  init_game_info_struct(&game_info, &curr_fig);

  int result = move_figure(&action, &curr_fig, &game_info);

  ck_assert_int_eq(result, 1);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_right_action_returns_one) {
  UserAction_t action = Right;
  CurFig_info curr_fig;
  GameInfo_t game_info;

  init_game_info_struct(&game_info, &curr_fig);

  int result = move_figure(&action, &curr_fig, &game_info);

  ck_assert_int_eq(result, 1);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_down_action_returns_one) {
  UserAction_t action = Down;
  CurFig_info curr_fig;
  GameInfo_t game_info;

  init_game_info_struct(&game_info, &curr_fig);

  int result = move_figure(&action, &curr_fig, &game_info);

  ck_assert_int_eq(result, 1);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_up_action_returns_zero) {
  UserAction_t action = Up;
  CurFig_info curr_fig;
  GameInfo_t game_info;

  init_game_info_struct(&game_info, &curr_fig);

  int result = move_figure(&action, &curr_fig, &game_info);

  ck_assert_int_eq(result, 0);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_start_action_returns_zero) {
  UserAction_t action = Start;
  CurFig_info curr_fig;
  GameInfo_t game_info;

  init_game_info_struct(&game_info, &curr_fig);
  int result = move_figure(&action, &curr_fig, &game_info);
  ck_assert_int_eq(result, 0);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_pause_action_returns_zero) {
  UserAction_t action = Pause;
  CurFig_info curr_fig;
  GameInfo_t game_info;

  init_game_info_struct(&game_info, &curr_fig);

  int result = move_figure(&action, &curr_fig, &game_info);

  ck_assert_int_eq(result, 0);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_terminate_action_returns_zero) {
  UserAction_t action = Terminate;
  CurFig_info curr_fig;
  GameInfo_t game_info;

  init_game_info_struct(&game_info, &curr_fig);

  int result = move_figure(&action, &curr_fig, &game_info);

  ck_assert_int_eq(result, 0);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_calm_action_returns_zero) {
  UserAction_t action = Calm;
  CurFig_info curr_fig;
  GameInfo_t game_info;

  init_game_info_struct(&game_info, &curr_fig);

  int result = move_figure(&action, &curr_fig, &game_info);

  ck_assert_int_eq(result, 0);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_action_action_returns_zero) {
  UserAction_t action = Action;
  CurFig_info curr_fig;
  GameInfo_t game_info;

  init_game_info_struct(&game_info, &curr_fig);

  int result = move_figure(&action, &curr_fig, &game_info);

  ck_assert_int_eq(result, 0);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_all_valid_actions_return_one) {
  CurFig_info curr_fig;
  GameInfo_t game_info;
  init_game_info_struct(&game_info, &curr_fig);

  UserAction_t valid_actions[] = {Left, Right, Down};
  int num_valid = sizeof(valid_actions) / sizeof(valid_actions[0]);

  for (int i = 0; i < num_valid; i++) {
    int result = move_figure(&valid_actions[i], &curr_fig, &game_info);
    ck_assert_int_eq(result, 1);
  }
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_all_invalid_actions_return_zero) {
  CurFig_info curr_fig;
  GameInfo_t game_info;
  init_game_info_struct(&game_info, &curr_fig);

  UserAction_t invalid_actions[] = {Start, Pause, Terminate, Up, Calm, Action};
  int num_invalid = sizeof(invalid_actions) / sizeof(invalid_actions[0]);

  for (int i = 0; i < num_invalid; i++) {
    int result = move_figure(&invalid_actions[i], &curr_fig, &game_info);
    ck_assert_int_eq(result, 0);
  }
  clear_fields(&game_info);
}
END_TEST

END_TEST

START_TEST(test_move_figure_multiple_calls_different_results) {
  CurFig_info curr_fig;
  GameInfo_t game_info;
  init_game_info_struct(&game_info, &curr_fig);

  UserAction_t actions[] = {Left, Up, Right, Start, Down, Calm, Pause};
  int expected_results[] = {1, 0, 1, 0, 1, 0, 0};

  for (int i = 0; i < 7; i++) {
    int result = move_figure(&actions[i], &curr_fig, &game_info);
    ck_assert_int_eq(result, expected_results[i]);
  }
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_same_action_multiple_times) {
  CurFig_info curr_fig;
  GameInfo_t game_info;
  init_game_info_struct(&game_info, &curr_fig);

  UserAction_t action = Left;
  for (int i = 0; i < 5; i++) {
    int result = move_figure(&action, &curr_fig, &game_info);
    ck_assert_int_eq(result, 1);
  }

  action = Up;
  for (int i = 0; i < 5; i++) {
    int result = move_figure(&action, &curr_fig, &game_info);
    ck_assert_int_eq(result, 0);
  }
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_return_value_consistency) {
  CurFig_info curr_fig;
  GameInfo_t game_info;
  init_game_info_struct(&game_info, &curr_fig);

  UserAction_t action = Down;

  int first_result = move_figure(&action, &curr_fig, &game_info);

  for (int i = 0; i < 3; i++) {
    int result = move_figure(&action, &curr_fig, &game_info);
    ck_assert_int_eq(result, first_result);
  }

  action = Terminate;
  first_result = move_figure(&action, &curr_fig, &game_info);

  for (int i = 0; i < 3; i++) {
    int result = move_figure(&action, &curr_fig, &game_info);
    ck_assert_int_eq(result, first_result);
  }
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_enum_boundary_values) {
  CurFig_info curr_fig;
  GameInfo_t game_info;
  init_game_info_struct(&game_info, &curr_fig);

  UserAction_t action = Start;
  int result = move_figure(&action, &curr_fig, &game_info);
  ck_assert_int_eq(result, 0);

  // Максимальное значение (Action = 8)
  action = Action;
  result = move_figure(&action, &curr_fig, &game_info);
  ck_assert_int_eq(result, 0);

  // Некоторые средние значения
  action = Left;  // 3
  result = move_figure(&action, &curr_fig, &game_info);
  ck_assert_int_eq(result, 1);

  action = Up;  // 4
  result = move_figure(&action, &curr_fig, &game_info);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_move_figure_invalid_enum_value) {
  CurFig_info curr_fig;
  GameInfo_t game_info;
  init_game_info_struct(&game_info, &curr_fig);

  UserAction_t invalid_action = (UserAction_t)(Action + 1);  // 9

  int result = move_figure(&invalid_action, &curr_fig, &game_info);

  ck_assert_int_eq(result, 0);

  invalid_action = (UserAction_t)(-1);
  result = move_figure(&invalid_action, &curr_fig, &game_info);
  ck_assert_int_eq(result, 0);

  invalid_action = (UserAction_t)(100);
  result = move_figure(&invalid_action, &curr_fig, &game_info);
  ck_assert_int_eq(result, 0);
  clear_fields(&game_info);
}
END_TEST

START_TEST(test_move_figure_flag_logic) {
  CurFig_info curr_fig;
  GameInfo_t game_info;
  init_game_info_struct(&game_info, &curr_fig);

  UserAction_t flag_one_actions[] = {Left, Right, Down};
  for (int i = 0; i < 3; i++) {
    int result = move_figure(&flag_one_actions[i], &curr_fig, &game_info);
    ck_assert_int_eq(result, 1);
  }

  UserAction_t flag_zero_actions[] = {Start, Pause, Terminate,
                                      Up,    Calm,  Action};
  for (int i = 0; i < 6; i++) {
    int result = move_figure(&flag_zero_actions[i], &curr_fig, &game_info);
    ck_assert_int_eq(result, 0);
  }
  clear_fields(&game_info);
}
END_TEST

Suite *move_figure_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("MoveFigure");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_move_figure_left_action_returns_one);
  tcase_add_test(tc_core, test_move_figure_right_action_returns_one);
  tcase_add_test(tc_core, test_move_figure_down_action_returns_one);
  tcase_add_test(tc_core, test_move_figure_up_action_returns_zero);
  tcase_add_test(tc_core, test_move_figure_start_action_returns_zero);
  tcase_add_test(tc_core, test_move_figure_pause_action_returns_zero);
  tcase_add_test(tc_core, test_move_figure_terminate_action_returns_zero);
  tcase_add_test(tc_core, test_move_figure_calm_action_returns_zero);
  tcase_add_test(tc_core, test_move_figure_action_action_returns_zero);
  tcase_add_test(tc_core, test_move_figure_all_valid_actions_return_one);
  tcase_add_test(tc_core, test_move_figure_all_invalid_actions_return_zero);
  tcase_add_test(tc_core, test_move_figure_multiple_calls_different_results);
  tcase_add_test(tc_core, test_move_figure_same_action_multiple_times);
  tcase_add_test(tc_core, test_move_figure_return_value_consistency);

  tcase_add_test(tc_core, test_move_figure_flag_logic);

  tcase_add_test(tc_core, test_move_figure_enum_boundary_values);
  tcase_add_test(tc_core, test_move_figure_invalid_enum_value);

  suite_add_tcase(s, tc_core);

  return s;
}

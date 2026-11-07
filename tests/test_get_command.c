#include "tetris_test.h"

START_TEST(test_set_user_action_arrow_keys) {
  UserAction_t action;

  set_user_action(&action, KEY_UP);
  ck_assert_int_eq(action, Up);

  set_user_action(&action, KEY_DOWN);
  ck_assert_int_eq(action, Down);

  set_user_action(&action, KEY_LEFT);
  ck_assert_int_eq(action, Left);

  set_user_action(&action, KEY_RIGHT);
  ck_assert_int_eq(action, Right);
}
END_TEST

START_TEST(test_set_user_action_terminate_keys) {
  UserAction_t action;

  set_user_action(&action, 'e');
  ck_assert_int_eq(action, Terminate);

  set_user_action(&action, 'E');
  ck_assert_int_eq(action, Terminate);
}
END_TEST

START_TEST(test_set_user_action_start_keys) {
  UserAction_t action;

  set_user_action(&action, 's');
  ck_assert_int_eq(action, Start);

  set_user_action(&action, 'S');
  ck_assert_int_eq(action, Start);
}
END_TEST

START_TEST(test_set_user_action_pause_keys) {
  UserAction_t action;

  set_user_action(&action, 'p');
  ck_assert_int_eq(action, Pause);

  set_user_action(&action, 'P');
  ck_assert_int_eq(action, Pause);
}
END_TEST

START_TEST(test_set_user_action_calm_for_unknown_keys) {
  UserAction_t action = Up;

  set_user_action(&action, 'a');
  ck_assert_int_eq(action, Calm);

  set_user_action(&action, 'z');
  ck_assert_int_eq(action, Calm);

  set_user_action(&action, '1');
  ck_assert_int_eq(action, Calm);

  set_user_action(&action, ' ');
  ck_assert_int_eq(action, Calm);

  set_user_action(&action, '\n');
  ck_assert_int_eq(action, Calm);

  set_user_action(&action, 0);
  ck_assert_int_eq(action, Calm);

  set_user_action(&action, 255);
  ck_assert_int_eq(action, Calm);
}
END_TEST

START_TEST(test_set_user_action_null_pointer) {
  ck_assert_ptr_nonnull(set_user_action);
}
END_TEST

START_TEST(test_set_user_action_case_sensitivity) {
  UserAction_t action;

  set_user_action(&action, 'e');
  ck_assert_int_eq(action, Terminate);

  set_user_action(&action, 'E');
  ck_assert_int_eq(action, Terminate);

  set_user_action(&action, 's');
  ck_assert_int_eq(action, Start);

  set_user_action(&action, 'S');
  ck_assert_int_eq(action, Start);

  set_user_action(&action, 'p');
  ck_assert_int_eq(action, Pause);

  set_user_action(&action, 'P');
  ck_assert_int_eq(action, Pause);

  set_user_action(&action, 'a');
  ck_assert_int_eq(action, Calm);

  set_user_action(&action, 'A');
  ck_assert_int_eq(action, Calm);
}
END_TEST

Suite *get_command_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("GetCommand");

  tc_core = tcase_create("SetUserAction");
  tcase_add_test(tc_core, test_set_user_action_arrow_keys);
  tcase_add_test(tc_core, test_set_user_action_terminate_keys);
  tcase_add_test(tc_core, test_set_user_action_start_keys);
  tcase_add_test(tc_core, test_set_user_action_pause_keys);
  tcase_add_test(tc_core, test_set_user_action_calm_for_unknown_keys);
  tcase_add_test(tc_core, test_set_user_action_case_sensitivity);
  tcase_add_test(tc_core, test_set_user_action_null_pointer);

  suite_add_tcase(s, tc_core);

  return s;
}

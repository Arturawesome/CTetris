#include "tetris_test.h"

void run_test_suite(Suite *test_suite) {
  SRunner *suite_runner = srunner_create(test_suite);
  srunner_run_all(suite_runner, CK_NORMAL);
  srunner_free(suite_runner);
}

void run_tests(void) {
  Suite *test_suites[] = {clear_fields_suite(),
                          clear_line_suite(),
                          generate_next_figure_suite(),
                          init_game_info_suite(),
                          refresh_game_info_suite(),
                          refresh_game_state_suite(),
                          put_next_fig_suite(),
                          get_command_suite(),
                          move_figure_suite(),
                          new_figure_rotator_suite(),
                          fill_field_by_landing_figure_suite(),

                          NULL};

  for (Suite **test_suite = test_suites; *test_suite != NULL; ++test_suite) {
    run_test_suite(*test_suite);
  }
}

int main(void) {
  run_tests();
  return 0;
}

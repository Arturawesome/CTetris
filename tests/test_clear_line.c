#include "tetris_test.h"

int** create_test_field(void) {
  int** field = (int**)malloc(SIZE_FIELD_ROW * sizeof(int*));
  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    field[i] = (int*)malloc(SIZE_FIELD_COL * sizeof(int));
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      field[i][j] = 0;
    }
  }
  return field;
}

void free_test_field(int** field) {
  if (field != NULL) {
    for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
      free(field[i]);
    }
    free(field);
  }
}

START_TEST(test_clear_line_no_lines_to_clear) {
  int** field = create_test_field();

  for (int i = 10; i < 15; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL - 1; ++j) {
      field[i][j] = 1;
    }
  }

  int cleared = clear_line(field);

  ck_assert_int_eq(cleared, 0);

  for (int i = 10; i < 15; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL - 1; ++j) {
      ck_assert_int_eq(field[i][j], 1);
    }
    ck_assert_int_eq(field[i][SIZE_FIELD_COL - 1], 0);
  }

  free_test_field(field);
}
END_TEST

START_TEST(test_clear_line_one_full_line) {
  int** field = create_test_field();

  for (int j = 0; j < SIZE_FIELD_COL; ++j) {
    field[15][j] = 1;
  }

  field[16][3] = 1;
  field[16][4] = 1;
  field[16][5] = 1;

  int cleared = clear_line(field);

  ck_assert_int_eq(cleared, 1);

  for (int j = 0; j < SIZE_FIELD_COL; ++j) {
    ck_assert_int_eq(field[15][j], 0);
  }

  ck_assert_int_eq(field[16][3], 1);
  ck_assert_int_eq(field[16][4], 1);
  ck_assert_int_eq(field[16][5], 1);

  ck_assert_int_eq(field[14][3], 0);
  ck_assert_int_eq(field[14][4], 0);
  ck_assert_int_eq(field[14][5], 0);

  free_test_field(field);
}
END_TEST

START_TEST(test_clear_line_lines_with_gaps) {
  int** field = create_test_field();

  for (int j = 0; j < SIZE_FIELD_COL; ++j) {
    field[5][j] = 2;
    field[8][j] = 3;
    field[12][j] = 4;
  }
  int cleared = clear_line(field);
  ck_assert_int_eq(cleared, 3);
  for (int j = 0; j < SIZE_FIELD_COL; ++j) {
    ck_assert_int_eq(field[5][j], 0);
    ck_assert_int_eq(field[8][j], 0);
    ck_assert_int_eq(field[12][j], 0);
  }
  ck_assert_int_eq(field[5][1], 0);
  ck_assert_int_eq(field[6][1], 0);
  ck_assert_int_eq(field[8][4], 0);
  ck_assert_int_eq(field[10][4], 0);

  free_test_field(field);
}
END_TEST

START_TEST(test_clear_line_top_line) {
  int** field = create_test_field();

  for (int j = 0; j < SIZE_FIELD_COL; ++j) {
    field[0][j] = 1;
  }

  field[1][0] = 2;
  field[1][1] = 2;

  int cleared = clear_line(field);

  ck_assert_int_eq(cleared, 1);

  for (int j = 0; j < SIZE_FIELD_COL; ++j) {
    ck_assert_int_eq(field[0][j], 0);
  }

  ck_assert_int_eq(field[1][0], 2);
  ck_assert_int_eq(field[1][1], 2);

  free_test_field(field);
}
END_TEST

START_TEST(test_clear_line_bottom_line) {
  int** field = create_test_field();

  for (int j = 0; j < SIZE_FIELD_COL; ++j) {
    field[SIZE_FIELD_ROW - 1][j] = 1;
  }

  field[SIZE_FIELD_ROW - 2][SIZE_FIELD_COL - 1] = 1;

  int cleared = clear_line(field);

  ck_assert_int_eq(cleared, 1);

  for (int j = 0; j < SIZE_FIELD_COL - 1; ++j) {
    ck_assert_int_eq(field[SIZE_FIELD_ROW - 1][j], 0);
  }

  ck_assert_int_eq(field[SIZE_FIELD_ROW - 1][SIZE_FIELD_COL - 1], 1);
  ck_assert_int_eq(field[SIZE_FIELD_ROW - 2][SIZE_FIELD_COL - 1], 0);

  free_test_field(field);
}
END_TEST

START_TEST(test_clear_line_all_lines_full) {
  int** field = create_test_field();

  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      field[i][j] = (i + j) % 5 + 1;
    }
  }

  int cleared = clear_line(field);

  ck_assert_int_eq(cleared, SIZE_FIELD_ROW);

  for (int i = 0; i < SIZE_FIELD_ROW; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      ck_assert_int_eq(field[i][j], 0);
    }
  }

  free_test_field(field);
}
END_TEST

START_TEST(test_clear_line_almost_full_line) {
  int** field = create_test_field();

  for (int j = 0; j < SIZE_FIELD_COL - 1; ++j) {
    field[10][j] = 1;
  }
  field[10][SIZE_FIELD_COL - 1] = 0;

  for (int j = 1; j < SIZE_FIELD_COL; ++j) {
    field[9][j] = 2;
  }
  field[9][0] = 0;

  int cleared = clear_line(field);

  ck_assert_int_eq(cleared, 0);

  for (int j = 0; j < SIZE_FIELD_COL - 1; ++j) {
    ck_assert_int_eq(field[10][j], 1);
  }
  ck_assert_int_eq(field[10][SIZE_FIELD_COL - 1], 0);

  for (int j = 1; j < SIZE_FIELD_COL; ++j) {
    ck_assert_int_eq(field[9][j], 2);
  }
  ck_assert_int_eq(field[9][0], 0);

  free_test_field(field);
}
END_TEST

START_TEST(test_clear_line_with_different_values) {
  int** field = create_test_field();

  for (int i = 5; i <= 7; ++i) {
    for (int j = 0; j < SIZE_FIELD_COL; ++j) {
      field[i][j] = (i * 10) + j + 1;
    }
  }

  field[4][2] = -1;
  field[4][3] = -2;

  int cleared = clear_line(field);

  ck_assert_int_eq(cleared, 3);

  ck_assert_int_eq(field[7][2], -1);
  ck_assert_int_eq(field[7][3], -2);
  ck_assert_int_eq(field[4][2], 0);
  ck_assert_int_eq(field[4][3], 0);

  free_test_field(field);
}
END_TEST

START_TEST(test_clear_line_null_field) { ck_assert_ptr_nonnull(clear_line); }
END_TEST

Suite* clear_line_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("ClearLine");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_clear_line_no_lines_to_clear);
  tcase_add_test(tc_core, test_clear_line_one_full_line);
  tcase_add_test(tc_core, test_clear_line_almost_full_line);
  tcase_add_test(tc_core, test_clear_line_lines_with_gaps);
  tcase_add_test(tc_core, test_clear_line_with_different_values);
  tcase_add_test(tc_core, test_clear_line_all_lines_full);

  tcase_add_test(tc_core, test_clear_line_top_line);
  tcase_add_test(tc_core, test_clear_line_bottom_line);
  tcase_add_test(tc_core, test_clear_line_null_field);

  suite_add_tcase(s, tc_core);

  return s;
}

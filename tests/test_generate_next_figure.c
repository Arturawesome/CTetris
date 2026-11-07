#include "tetris_test.h"

int** create_next_array(void) {
  int** next = (int**)malloc(FIG_SIZE_NEXT_ROW * sizeof(int*));
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    next[i] = (int*)malloc(FIG_SIZE_NEXT_COL * sizeof(int));

    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      next[i][j] = 999;
    }
  }
  return next;
}

void free_next_array(int** next) {
  if (next != NULL) {
    for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
      free(next[i]);
    }
    free(next);
  }
}

int count_ones_in_next(int** next) {
  int count = 0;
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      if (next[i][j] == 1) {
        count++;
      }
    }
  }
  return count;
}

int next_contains_only_zeros_and_ones(int** next) {
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      if (next[i][j] != 0 && next[i][j] != 1) {
        return 0;
      }
    }
  }
  return 1;
}

START_TEST(test_generate_next_figure_basic) {
  CurFig_info next_fig;
  int** next = create_next_array();

  generate_next_figure(&next_fig, next);

  ck_assert_int_ge(next_fig.shape, 0);
  ck_assert_int_le(next_fig.shape, COUNT_SHAPE - 1);
  ck_assert_int_ge(next_fig.rot, 0);
  ck_assert_int_le(next_fig.rot, COUN_ROTATOR - 1);

  ck_assert(next_contains_only_zeros_and_ones(next));

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      ck_assert_int_ne(next[i][j], 999);
    }
  }

  int ones_count = count_ones_in_next(next);
  ck_assert_int_eq(ones_count, 4);

  free_next_array(next);
}
END_TEST

START_TEST(test_generate_next_figure_multiple_calls) {
  CurFig_info next_fig1, next_fig2, next_fig3;
  int** next = create_next_array();

  generate_next_figure(&next_fig1, next);

  int** saved_next = create_next_array();
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    memcpy(saved_next[i], next[i], FIG_SIZE_NEXT_COL * sizeof(int));
  }

  generate_next_figure(&next_fig2, next);

  int identical = 1;
  for (int i = 0; i < FIG_SIZE_NEXT_ROW && identical; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL && identical; ++j) {
      if (saved_next[i][j] != next[i][j]) {
        identical = 0;
      }
    }
  }

  generate_next_figure(&next_fig3, next);

  ck_assert_int_ge(next_fig1.shape, 0);
  ck_assert_int_le(next_fig1.shape, COUNT_SHAPE - 1);
  ck_assert_int_ge(next_fig2.shape, 0);
  ck_assert_int_le(next_fig2.shape, COUNT_SHAPE - 1);
  ck_assert_int_ge(next_fig3.shape, 0);
  ck_assert_int_le(next_fig3.shape, COUNT_SHAPE - 1);

  free_next_array(next);
  free_next_array(saved_next);
}
END_TEST

START_TEST(test_generate_next_figure_centered_in_next) {
  CurFig_info next_fig;
  int** next = create_next_array();

  generate_next_figure(&next_fig, next);

  int min_row = FIG_SIZE_NEXT_ROW;
  int max_row = -1;
  int min_col = FIG_SIZE_NEXT_COL;
  int max_col = -1;

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      if (next[i][j] == 1) {
        if (i < min_row) min_row = i;
        if (i > max_row) max_row = i;
        if (j < min_col) min_col = j;
        if (j > max_col) max_col = j;
      }
    }
  }

  ck_assert_int_ge(min_row, 0);
  ck_assert_int_le(max_row, FIG_SIZE_NEXT_ROW - 1);
  ck_assert_int_ge(min_col, 0);
  ck_assert_int_le(max_col, FIG_SIZE_NEXT_COL - 1);

  ck_assert_int_eq(count_ones_in_next(next), 4);

  free_next_array(next);
}
END_TEST

START_TEST(test_generate_next_figure_all_shapes_possible) {
  int shape_counts[COUNT_SHAPE] = {0};
  const int NUM_TRIALS = 1000;
  int** next = create_next_array();

  for (int trial = 0; trial < NUM_TRIALS; ++trial) {
    CurFig_info next_fig;
    generate_next_figure(&next_fig, next);

    ck_assert_int_ge(next_fig.shape, 0);
    ck_assert_int_le(next_fig.shape, COUNT_SHAPE - 1);

    shape_counts[next_fig.shape]++;
  }

  for (int shape = 0; shape < COUNT_SHAPE; ++shape) {
    ck_assert_int_gt(shape_counts[shape], 0);
  }

  free_next_array(next);
}
END_TEST

START_TEST(test_generate_next_figure_rotation_consistency) {
  CurFig_info next_fig;
  int** next = create_next_array();

  generate_next_figure(&next_fig, next);

  ck_assert_int_ge(next_fig.rot, 0);
  ck_assert_int_le(next_fig.rot, COUN_ROTATOR - 1);

  Fig_ptr fig = FigsConfig();
  int min_row = 100;

  for (int j = 0; j < CURRENT_FIG_COL; j++) {
    int row = fig[next_fig.shape][next_fig.rot][1][j];
    min_row = min_row > row ? row : min_row;
  }

  for (int j = 0; j < CURRENT_FIG_COL; j++) {
    int col = fig[next_fig.shape][next_fig.rot][0][j];
    int row = fig[next_fig.shape][next_fig.rot][1][j] - min_row;

    ck_assert_int_ge(row, 0);
    ck_assert_int_lt(row, FIG_SIZE_NEXT_ROW);
    ck_assert_int_ge(col, 0);
    ck_assert_int_lt(col, FIG_SIZE_NEXT_COL);

    ck_assert_int_eq(next[row][col], 1);
  }

  free_next_array(next);
}
END_TEST

START_TEST(test_generate_next_figure_clear_before_fill) {
  CurFig_info next_fig;
  int** next = create_next_array();

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      next[i][j] = 2;
    }
  }

  generate_next_figure(&next_fig, next);

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      ck_assert(next[i][j] == 0 || next[i][j] == 1);
    }
  }

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      ck_assert_int_ne(next[i][j], 2);
    }
  }

  free_next_array(next);
}
END_TEST

START_TEST(test_generate_next_figure_position_values) {
  CurFig_info next_fig;
  int** next = create_next_array();

  generate_next_figure(&next_fig, next);

  ck_assert_int_ge(next_fig.shape, Z_SHAPE);
  ck_assert_int_le(next_fig.shape, T_SHAPE);
  ck_assert_int_ge(next_fig.rot, ROTATOR_0);
  ck_assert_int_le(next_fig.rot, ROTATOR_270);

  free_next_array(next);
}
END_TEST

START_TEST(test_generate_next_figure_reproducibility_with_seed) {
  CurFig_info next_fig1, next_fig2;
  int** next1 = create_next_array();
  int** next2 = create_next_array();

  srand(42);
  generate_next_figure(&next_fig1, next1);

  srand(42);
  generate_next_figure(&next_fig2, next2);

  ck_assert_int_eq(next_fig1.shape, next_fig2.shape);
  ck_assert_int_eq(next_fig1.rot, next_fig2.rot);

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    for (int j = 0; j < FIG_SIZE_NEXT_COL; ++j) {
      ck_assert_int_eq(next1[i][j], next2[i][j]);
    }
  }

  free_next_array(next1);
  free_next_array(next2);
}
END_TEST

START_TEST(test_generate_next_figure_no_memory_leak) {
  CurFig_info next_fig;

  for (int i = 0; i < 100; ++i) {
    int** next = create_next_array();
    generate_next_figure(&next_fig, next);

    ck_assert_int_ge(next_fig.shape, 0);
    ck_assert_int_le(next_fig.shape, COUNT_SHAPE - 1);
    ck_assert_int_eq(count_ones_in_next(next), 4);

    free_next_array(next);
  }
}
END_TEST

Suite* generate_next_figure_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("GenerateNextFigure");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_generate_next_figure_basic);
  tcase_add_test(tc_core, test_generate_next_figure_multiple_calls);
  tcase_add_test(tc_core, test_generate_next_figure_centered_in_next);
  tcase_add_test(tc_core, test_generate_next_figure_clear_before_fill);
  tcase_add_test(tc_core, test_generate_next_figure_position_values);
  tcase_add_test(tc_core, test_generate_next_figure_all_shapes_possible);
  tcase_add_test(tc_core, test_generate_next_figure_rotation_consistency);
  tcase_add_test(tc_core, test_generate_next_figure_reproducibility_with_seed);
  tcase_add_test(tc_core, test_generate_next_figure_no_memory_leak);

  suite_add_tcase(s, tc_core);

  return s;
}

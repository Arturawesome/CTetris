#include "tetris_test.h"

START_TEST(test_put_next_fig_basic_copy) {
  CurFig_info next_fig;
  CurFig_info curr_fig;

  next_fig.shape = I_SHAPE;
  next_fig.rot = ROTATOR_90;
  next_fig.row_pos = 5;
  next_fig.col_pos = 3;

  curr_fig.shape = O_SHAPE;
  curr_fig.rot = ROTATOR_180;
  curr_fig.row_pos = 10;
  curr_fig.col_pos = 7;

  put_next_fig_in_curr(&next_fig, &curr_fig);

  ck_assert_int_eq(curr_fig.shape, I_SHAPE);

  ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
  ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);

  ck_assert_int_eq(next_fig.shape, I_SHAPE);
  ck_assert_int_eq(next_fig.rot, ROTATOR_90);
  ck_assert_int_eq(next_fig.row_pos, 5);
  ck_assert_int_eq(next_fig.col_pos, 3);
}
END_TEST

START_TEST(test_put_next_fig_all_shapes) {
  FigShape_info shapes[] = {Z_SHAPE, S_SHAPE, O_SHAPE, I_SHAPE,
                            L_SHAPE, J_SHAPE, T_SHAPE};
  int num_shapes = sizeof(shapes) / sizeof(shapes[0]);

  for (int i = 0; i < num_shapes; i++) {
    CurFig_info next_fig;
    CurFig_info curr_fig;

    next_fig.shape = shapes[i];
    next_fig.rot = (Rotator_info)(i % COUN_ROTATOR);
    next_fig.row_pos = i * 2;
    next_fig.col_pos = i * 3;

    curr_fig.shape = T_SHAPE;
    curr_fig.rot = ROTATOR_270;
    curr_fig.row_pos = 99;
    curr_fig.col_pos = 99;

    put_next_fig_in_curr(&next_fig, &curr_fig);

    ck_assert_int_eq(curr_fig.shape, shapes[i]);

    ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
    ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);

    ck_assert_int_eq(next_fig.shape, shapes[i]);
    ck_assert_int_eq(next_fig.row_pos, i * 2);
    ck_assert_int_eq(next_fig.col_pos, i * 3);
  }
}
END_TEST

START_TEST(test_put_next_fig_all_rotations) {
  Rotator_info rotations[] = {ROTATOR_0, ROTATOR_90, ROTATOR_180, ROTATOR_270};

  for (int i = 0; i < COUN_ROTATOR; i++) {
    CurFig_info next_fig;
    CurFig_info curr_fig;

    next_fig.shape = T_SHAPE;
    next_fig.rot = rotations[i];
    next_fig.row_pos = 15;
    next_fig.col_pos = 8;

    put_next_fig_in_curr(&next_fig, &curr_fig);

    ck_assert_int_eq(curr_fig.shape, T_SHAPE);

    ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
    ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);

    ck_assert_int_eq(next_fig.rot, rotations[i]);
  }
}
END_TEST

START_TEST(test_put_next_fig_start_positions) {
  CurFig_info next_fig;
  CurFig_info curr_fig;

  ck_assert_int_ge(START_CURR_FIG_ROW, 0);
  ck_assert_int_ge(START_CURR_FIG_COL, 0);

  ck_assert_int_le(START_CURR_FIG_COL, SIZE_FIELD_COL - CURRENT_FIG_COL);

  next_fig.shape = L_SHAPE;
  put_next_fig_in_curr(&next_fig, &curr_fig);

  ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
  ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);
}
END_TEST

START_TEST(test_put_next_fig_same_variable) {
  CurFig_info fig;

  fig.shape = I_SHAPE;
  fig.rot = ROTATOR_90;
  fig.row_pos = 5;
  fig.col_pos = 2;

  put_next_fig_in_curr(&fig, &fig);

  ck_assert_int_eq(fig.shape, I_SHAPE);
  ck_assert_int_eq(fig.row_pos, START_CURR_FIG_ROW);
  ck_assert_int_eq(fig.col_pos, START_CURR_FIG_COL);
}
END_TEST

START_TEST(test_put_next_fig_multiple_calls) {
  CurFig_info next_fig;
  CurFig_info curr_fig;

  next_fig.shape = Z_SHAPE;
  put_next_fig_in_curr(&next_fig, &curr_fig);
  ck_assert_int_eq(curr_fig.shape, Z_SHAPE);
  ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
  ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);

  next_fig.shape = O_SHAPE;
  put_next_fig_in_curr(&next_fig, &curr_fig);
  ck_assert_int_eq(curr_fig.shape, O_SHAPE);
  ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
  ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);

  next_fig.shape = T_SHAPE;
  put_next_fig_in_curr(&next_fig, &curr_fig);
  ck_assert_int_eq(curr_fig.shape, T_SHAPE);
  ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
  ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);
}
END_TEST

START_TEST(test_put_next_fig_invalid_rotation_value) {
  CurFig_info next_fig;
  CurFig_info curr_fig;

  next_fig.shape = J_SHAPE;
  next_fig.rot = (Rotator_info)(COUN_ROTATOR + 2);
  next_fig.row_pos = 3;
  next_fig.col_pos = 4;

  put_next_fig_in_curr(&next_fig, &curr_fig);

  ck_assert_int_eq(curr_fig.shape, J_SHAPE);

  ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
  ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);

  ck_assert_int_eq(next_fig.rot, COUN_ROTATOR + 2);
}
END_TEST

START_TEST(test_put_next_fig_negative_positions) {
  CurFig_info next_fig;
  CurFig_info curr_fig;

  next_fig.shape = S_SHAPE;
  next_fig.rot = ROTATOR_180;
  next_fig.row_pos = -5;
  next_fig.col_pos = -3;

  put_next_fig_in_curr(&next_fig, &curr_fig);

  ck_assert_int_eq(curr_fig.shape, S_SHAPE);

  ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
  ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);

  ck_assert_int_eq(next_fig.row_pos, -5);
  ck_assert_int_eq(next_fig.col_pos, -3);
}
END_TEST

START_TEST(test_put_next_fig_large_positions) {
  CurFig_info next_fig;
  CurFig_info curr_fig;

  next_fig.shape = L_SHAPE;
  next_fig.rot = ROTATOR_270;
  next_fig.row_pos = 10000;
  next_fig.col_pos = 5000;

  put_next_fig_in_curr(&next_fig, &curr_fig);

  ck_assert_int_eq(curr_fig.shape, L_SHAPE);

  ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
  ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);

  ck_assert_int_eq(next_fig.row_pos, 10000);
  ck_assert_int_eq(next_fig.col_pos, 5000);
}
END_TEST

START_TEST(test_put_next_fig_integration_with_generate) {
  int **next = (int **)malloc(FIG_SIZE_NEXT_ROW * sizeof(int *));
  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    next[i] = (int *)malloc(FIG_SIZE_NEXT_COL * sizeof(int));
  }

  CurFig_info next_fig;
  CurFig_info curr_fig;

  generate_next_figure(&next_fig, next);

  put_next_fig_in_curr(&next_fig, &curr_fig);

  ck_assert_int_eq(curr_fig.shape, next_fig.shape);
  ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
  ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);

  for (int i = 0; i < FIG_SIZE_NEXT_ROW; ++i) {
    free(next[i]);
  }
  free(next);
}
END_TEST

START_TEST(test_put_next_fig_copy_fig_function_call) {
  CurFig_info next_fig;
  CurFig_info curr_fig;

  next_fig.shape = I_SHAPE;
  next_fig.rot = ROTATOR_90;

  put_next_fig_in_curr(&next_fig, &curr_fig);

  ck_assert_int_eq(curr_fig.shape, I_SHAPE);
  ck_assert_int_eq(curr_fig.row_pos, START_CURR_FIG_ROW);
  ck_assert_int_eq(curr_fig.col_pos, START_CURR_FIG_COL);
}
END_TEST

Suite *put_next_fig_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("PutNextFigInCurr");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_put_next_fig_basic_copy);
  tcase_add_test(tc_core, test_put_next_fig_all_shapes);
  tcase_add_test(tc_core, test_put_next_fig_all_rotations);
  tcase_add_test(tc_core, test_put_next_fig_start_positions);
  tcase_add_test(tc_core, test_put_next_fig_multiple_calls);
  tcase_add_test(tc_core, test_put_next_fig_copy_fig_function_call);
  tcase_add_test(tc_core, test_put_next_fig_same_variable);
  tcase_add_test(tc_core, test_put_next_fig_invalid_rotation_value);
  tcase_add_test(tc_core, test_put_next_fig_negative_positions);
  tcase_add_test(tc_core, test_put_next_fig_large_positions);
  tcase_add_test(tc_core, test_put_next_fig_integration_with_generate);

  suite_add_tcase(s, tc_core);

  return s;
}

#ifndef FIGURES_TETRIS_H
#define FIGURES_TETRIS_H

#include <stdio.h>
#include <stdlib.h>

#define CURRENT_FIG_COL 4
#define CURRENT_FIG_ROW 2
#define START_CURR_FIG_COL 3
#define START_CURR_FIG_ROW 0

typedef enum {
  Z_SHAPE,
  S_SHAPE,
  O_SHAPE,
  I_SHAPE,
  L_SHAPE,
  J_SHAPE,
  T_SHAPE,
  COUNT_SHAPE
} FigShape_info;

typedef enum {
  ROTATOR_0,
  ROTATOR_90,
  ROTATOR_180,
  ROTATOR_270,
  COUN_ROTATOR
} Rotator_info;

typedef int Fig[COUNT_SHAPE][COUN_ROTATOR][CURRENT_FIG_ROW][CURRENT_FIG_COL];
typedef int (*Fig_ptr)[COUN_ROTATOR][CURRENT_FIG_ROW][CURRENT_FIG_COL];
typedef int (*FigCoord_ptr)[CURRENT_FIG_COL];

typedef struct {
  int row_pos;
  int col_pos;
  FigShape_info shape;
  Rotator_info rot;
} CurFig_info;

Fig_ptr FigsConfig();
FigShape_info Get_next_fig();
void Cpoy_fig(CurFig_info *curr_fig, FigShape_info shape);
void update_position(CurFig_info *curr_fig, int **field, int d_col, int d_row);
void new_figure_rotator(CurFig_info *curr_fig, int **field);
int check_bourder_and_filed(const CurFig_info *curr_fig, int **field);
void move_figure_down(CurFig_info *curr_fig, int **field);
int check_landing(const CurFig_info *curr_fig, int **field);
int fill_field_by_landing_figure(const CurFig_info *curr_fig, int **field);
int rotator_decision(CurFig_info *curr_fig, int **field);

#endif

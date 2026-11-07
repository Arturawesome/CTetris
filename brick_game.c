#include "brick_game.h"

int main() {
  init_ncurses();
  new_init_menu_window();
  GameInfo_t game_info = {0};
  UserAction_t user_action = {0};
  CurFig_info curr_fig = {0};
  init_game_info_struct(&game_info, &curr_fig);
  new_refresh_info_window(game_info.next, game_info.score, game_info.high_score,
                          game_info.level);
  new_refresh_field_window(game_info.field, &curr_fig);

  get_start_command(&user_action);

  if (user_action == Start) {
    new_spawn(&game_info, &user_action, &curr_fig);
  }
  endwin();
  clear_fields(&game_info);
  return 0;
}

void restart_game(GameInfo_t *game_info, const UserAction_t *user_action,
                  CurFig_info *curr_fig) {
  if (*user_action == Start) {
    refresh_game_info_for_new_round(game_info);
    put_next_fig_in_curr(&game_info->next_fig, curr_fig);
    generate_next_figure(&game_info->next_fig, game_info->next);
    new_refresh_info_window(game_info->next, game_info->score,
                            game_info->high_score, game_info->level);
    new_refresh_field_window(game_info->field, curr_fig);
    new_init_menu_window();
  }
}
void new_spawn(GameInfo_t *game_info, UserAction_t *user_action,
               CurFig_info *curr_fig) {
  clock_t last_fall = clock();
  int is_end_round = 0;
  generate_next_figure(&game_info->next_fig, game_info->next);

  while (*user_action != Terminate) {
    if (is_end_round) {
      new_refresh_window();
      get_start_command(user_action);
      restart_game(game_info, user_action, curr_fig);
      is_end_round = 0;
    }
    clock_t now = clock();
    if (*user_action == Pause) {
      get_start_command(user_action);
    } else if (*user_action == Up) {
      new_figure_rotator(curr_fig, game_info->field);
    } else {
      move_figure(user_action, curr_fig, game_info);
    }
    double elapsed = (double)(now - last_fall) / CLOCKS_PER_SEC;
    if (elapsed >= game_info->delay) {
      check_and_clean_line(game_info, curr_fig, &is_end_round);
      new_refresh_info_window(game_info->next, game_info->score,
                              game_info->high_score, game_info->level);
      move_figure_down(curr_fig, game_info->field);
      last_fall = now;
    }
    new_refresh_field_window(game_info->field, curr_fig);
    if (*user_action != Terminate) {
      int c = getch();
      set_user_action(user_action, c);
    }
    napms(10);
  }
}

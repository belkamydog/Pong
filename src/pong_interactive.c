#include <stdio.h>
#include <ncurses.h>

#define ROW 25
#define COL 80
#define ROCKET_SIZE 3
#define WIN_SCORE 21
#define GAME_SPEED 500

void print_game_field(int x_ball, int y_ball, int p1_pos, int p2_pos,
                      int score_player_1, int score_player_2);
void print_ball_and_rockets(int x_ball, int y_ball, int p1_pos, int p2_pos,
                            int x_current, int y_current);
char is_rocket(int y_rocket_p1, int y_rocket_p2, int x_current, int y_current);
int x_ball_mooving(int x_ball, int x_vector);
int y_ball_mooving(int y_ball, int y_vector);
int change_y_vector_mooving(int y_ball, int y_vector);
int hit_the_ball(int x_ball, int y_ball, int y_rocket_1, int y_rocket_2);
int move_rocket_up(int y_rocket);
int move_rocket_down(int y_rocket);
void print_horisontal_border();
void print_score(int p1, int p2);
void printAfterEndGame(int score_p1, int score_p2);
void print_hello_and_instruction();

int main() {
  int player_1_score = 0, player_2_score = 0;
  int y_position_player_1 = ROW / 2, y_position_player_2 = ROW / 2;
  int x_ball = COL / 2, y_ball = ROW / 2, vector_x = 1, vector_y = -1;
  char user_input = ' ';
  initscr();
  print_hello_and_instruction();
  if ((user_input = getch()) == 's' || (user_input = getch()) == 'S') {
    while ((user_input != 'q' || user_input == 'Q') &&
           (player_1_score < WIN_SCORE && player_2_score < WIN_SCORE)) {
      keypad(stdscr,true);
      timeout(GAME_SPEED);
      print_game_field(x_ball, y_ball, y_position_player_1, y_position_player_2,
                       player_1_score, player_2_score);
      user_input = getch();
      if (user_input == 'a')
        y_position_player_1 = move_rocket_up(y_position_player_1);
      else if (user_input == 'z')
        y_position_player_1 = move_rocket_down(y_position_player_1);
      else if (user_input == 'k')
        y_position_player_2 = move_rocket_up(y_position_player_2);
      else if (user_input == 'm')
        y_position_player_2 = move_rocket_down(y_position_player_2);
      vector_y = change_y_vector_mooving(y_ball, vector_y);
      x_ball = x_ball_mooving(x_ball, vector_x);
      y_ball = y_ball_mooving(y_ball, vector_y);
      if (hit_the_ball(x_ball, y_ball, y_position_player_1,
                       y_position_player_2) != 0) {
        vector_y = hit_the_ball(x_ball, y_ball, y_position_player_1,
                                y_position_player_2);
        vector_x *= -1;
      }
      if (x_ball <= 0) {
        x_ball = COL / 2;
        y_ball = ROW / 2;
        player_2_score++;
        vector_x = 1;
      } else if (x_ball >= COL) {
        x_ball = COL / 2;
        y_ball = ROW / 2;
        player_1_score++;
        vector_x = -1;
      }
      erase();
    }
    printAfterEndGame(player_1_score, player_2_score);
  }
  endwin();
  return 1;
}

// printing functions

void print_game_field(int x_ball, int y_ball, int p1_pos, int p2_pos,
                      int score_player_1, int score_player_2) {
  print_horisontal_border();
  for (int y = 0; y < ROW; y++) {
    for (int x = 0; x < COL; x++) {
      print_ball_and_rockets(x_ball, y_ball, p1_pos, p2_pos, x, y);
    }
    printw("\n");
  }
  print_horisontal_border();
  print_score(score_player_1, score_player_2);
}

void print_ball_and_rockets(int x_ball, int y_ball, int p1_pos, int p2_pos,
                            int x_current, int y_current) {
  if (x_current == x_ball && y_current == y_ball)
    printw("@");
  else if (is_rocket(p1_pos, p2_pos, x_current, y_current) == 1) {
    printw("|");
  } else
    printw(" ");
}

char is_rocket(int y_rocket_p1, int y_rocket_p2, int x_current, int y_current) {
  char result = 0;
  if (y_rocket_p1 == y_current && x_current == 0)
    result = 1;
  else if (y_rocket_p2 == y_current && x_current == COL - 1)
    result = 1;
  else if (y_rocket_p1 - 1 == y_current && x_current == 0)
    result = 1;
  else if (y_rocket_p1 + 1 == y_current && x_current == 0)
    result = 1;
  else if (y_rocket_p2 - 1 == y_current && x_current == COL - 1)
    result = 1;
  else if (y_rocket_p2 + 1 == y_current && x_current == COL - 1)
    result = 1;
  return result;
}

void print_horisontal_border() {
  for (int i = 0; i < COL; i++) printw("-");
  printw("\n");
}

void print_score(int p1, int p2) { printw("%d < GAME SCORE > %d\n", p1, p2); }

void printAfterEndGame(int score_p1, int score_p2) {
  if (score_p1 > score_p2) {
    printw("player 1 WINNER\n with score %d : %d\n", score_p1, score_p2);
  } else
    printw("player 2 WINNER\n with score %d : %d\n", score_p2, score_p1);
}

void print_hello_and_instruction() {
  printw("<< Ping-Pong game >>\n");
  printw("Hello Dear Gamer!!!\n");
  printw("Control: \n");
  printw("Player 1: A - up Z - down  Player 2: K - up M - down\n");
  printw("Q for Exit\n");
  printw("S for Start game\n");
}

// game process functions

int move_rocket_up(int y_rocket) {
  if (y_rocket > 1) y_rocket--;
  return y_rocket;
}

int move_rocket_down(int y_rocket) {
  if (y_rocket < ROW - 2) y_rocket++;
  return y_rocket;
}

int y_ball_mooving(int y_ball, int y_vector) {
  if (y_vector == 1)
    y_ball--;
  else
    y_ball++;
  return y_ball;
}

int change_y_vector_mooving(int y_ball, int y_vector) {
  if (y_ball == ROW - 1 && y_vector == -1)
    y_vector = 1;
  else if (y_ball == 0 && y_vector == 1)
    y_vector = -1;
  return y_vector;
}

int x_ball_mooving(int x_ball, int x_vector) {
  if (x_vector == 1)
    x_ball++;
  else
    x_ball--;
  return x_ball;
}

int hit_the_ball(int x_ball, int y_ball, int y_rocket_1, int y_rocket_2) {
  int result = 0;
  if (x_ball == 1 || x_ball == COL - 2) {
    if (y_ball == y_rocket_1)
      result = -1;
    else if (y_ball == (y_rocket_1 - 1))
      result = 1;
    else if (y_ball == (y_rocket_1 + 1))
      result = -1;
    else if (y_ball == y_rocket_2)
      result = 1;
    else if (y_ball == (y_rocket_2 - 1))
      result = -1;
    else if (y_ball == (y_rocket_2 + 1))
      result = 1;
  }
  return result;
}

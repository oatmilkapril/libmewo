#include <ncurses.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>

#include "mewo.h"
#include "frames.h"

WINDOW *create_win(int startx, int starty, int lines, int columns);
void run_frame_validation();
void draw_mewo(WINDOW *win, mewo *m);
void draw_stats(WINDOW *win, mewo *m);

int main() {	
    initscr();
    raw(); // Disable user input buffering
    noecho(); // Prevents user input from echoing to output
    setlocale(LC_ALL, "");

    // Enable color
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);

    run_frame_validation();

    endwin(); // End curses mode

    return EXIT_SUCCESS;
}

void run_frame_validation() {
    // Create main window
    int board_win_startx = 0;
    int board_win_starty = 0;
    int board_win_width = MEWO_DISPLAY_COLS + 2;
    int board_win_height = MEWO_DISPLAY_ROWS + 2;
    WINDOW *board_win = create_win(board_win_startx, board_win_starty, board_win_height, board_win_width);

    // Create stats window
    int stats_win_startx = board_win_startx + board_win_width + 1;
    int stats_win_starty = 0;
    int stats_win_width = 50;
    int stats_win_height = board_win_height;
    WINDOW *stats_win = create_win(stats_win_startx, stats_win_starty, stats_win_height, stats_win_width);

    mewo_config m_conf = {
        .name = "Mewo",
        .wakeup_time_ms = 0,
        .sleep_time_ms = 0
    };
    mewo m = {};
    mewo_init(&m, &m_conf);

    for (int body_frame = 0; body_frame < MEWO_BODY_FRAME_LEN; body_frame++) {
        for (int head_frame = 0; head_frame < MEWO_HEAD_FRAME_LEN; head_frame++) {

            if (body_frame == MEWO_BODY_FRAME_SLEEP) {
                if (head_frame == MEWO_HEAD_FRAME_SIDE_LEFT || head_frame == MEWO_HEAD_FRAME_SIDE_RIGHT) {
                    continue;
                }
            }

            mewo_set_body_frame(&m, body_frame);
            mewo_set_head_frame(&m, head_frame);
            mewo_refresh(&m);
            draw_mewo(board_win, &m);
            draw_stats(stats_win, &m);
            wgetch(board_win);
        }
    }
}

void draw_mewo(WINDOW *win, mewo *m) {
    int startx = 1;
    int starty = 1;

    wattron(win, COLOR_PAIR(1));
    for (int row_index = 0; row_index < MEWO_DISPLAY_ROWS; row_index++) {
        for (int col_index = 0; col_index < MEWO_DISPLAY_COLS; col_index++) {
            int x = col_index;
            int y = MEWO_DISPLAY_ROWS - row_index;
            bool pixel = mewo_get_pixel(m, x, y);

            if (pixel) {
                mvwaddch(win, starty + row_index, startx + col_index, ACS_CKBOARD);
            } else {
                mvwaddch(win, starty + row_index, startx + col_index, '_');
            }
        }
    }
    wattroff(win, COLOR_PAIR(1));

    wrefresh(win);
}

void draw_stats(WINDOW *win, mewo *m) {
    int win_height, win_width;
    getmaxyx(win, win_height, win_width);
    
    char buffer[128];

    snprintf(buffer, sizeof(buffer), "mewo debugging info            ");
    mvwprintw(win, 0, (win_width - strlen(buffer)) / 2, buffer);

    snprintf(buffer, sizeof(buffer), "state: %s            ", MEWO_STATE_STRING[m->state]);
    mvwprintw(win, 5, 3, buffer);
    
    snprintf(buffer, sizeof(buffer), "head frame: %s       ", MEWO_HEAD_FRAME_STRING[m->head_frame]);
    mvwprintw(win, 6, 3, buffer);
    
    snprintf(buffer, sizeof(buffer), "body frame: %s       ", MEWO_BODY_FRAME_STRING[m->body_frame]);
    mvwprintw(win, 7, 3, buffer);
    
    snprintf(buffer, sizeof(buffer), "x position: %d       ", m->x_pos);
    mvwprintw(win, 8, 3, buffer);
    
    snprintf(buffer, sizeof(buffer), "x speed: %d          ", m->x_speed);
    mvwprintw(win, 9, 3, buffer);

    int mewo_body_left_bound = m->x_pos + m->body_frame_info->x_offset;
    int mewo_body_right_bound = mewo_body_left_bound + m->body_frame_info->num_cols * 8;
    int mewo_head_left_bound = mewo_body_left_bound + MEWO_FRAMES_X_OFFSETS[m->body_frame][m->head_frame];
    int mewo_head_right_bound = mewo_head_left_bound + m->head_frame_info->num_cols * 8;

    snprintf(buffer, sizeof(buffer), "body: %d, %d         ", mewo_body_left_bound, mewo_body_right_bound);
    mvwprintw(win, 10, 3, buffer);
    
    snprintf(buffer, sizeof(buffer), "head: %d, %d         ", mewo_head_left_bound, mewo_head_right_bound);
    mvwprintw(win, 11, 3, buffer);

    wrefresh(win);
}

WINDOW *create_win(int startx, int starty, int lines, int columns) {
    WINDOW *win = newwin(lines, columns, starty, startx);
    box(win, 0, 0);
    keypad(win, TRUE);
    wrefresh(win);
    return win;
}

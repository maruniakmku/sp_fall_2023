#ifdef _WIN64
#define PDC_DLL_BUILD 1
#endif

#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include <curses.h>

#define ES_SPEED 5.0  /* 3 lines per sec. */

#define ES_DROP_FACTOR 6.0  /* increase speed 6 times
                               when DOWN is pressed */

#define ES_NLINES 20
#define ES_LINE_WIDTH 10
#define ES_FIGURE_WIDTH 4
#define ES_FIGURE_HEIGHT 4
#define ES_NFIGURES 16           /* total number of all rotated forms */
#define ES_NROTATIONS 4          /* number of rotations for each figure */

#define ES_HIGHSCORES_PATH "highscores.bin"
#define ES_SCORES_TO_KEEP 5

typedef unsigned char es_rock_t;

typedef const es_rock_t es_figure_t[ES_FIGURE_HEIGHT][ES_FIGURE_WIDTH];

enum es_line_state {
    ES_LINE_EMPTY = 0,
    ES_LINE_NOT_EMPTY,
    ES_LINE_FULL
};

struct es_line {
    enum es_line_state state;
    es_rock_t rocks[ES_LINE_WIDTH];
    int nrocks;
    int y;
};

enum es_moving_figure_state {
    ES_MOVING_FIGURE_FALLING = 0,
    ES_MOVING_FIGURE_DROPPING,
    ES_MOVING_FIGURE_STUCK
};

struct es_moving_figure {
    es_figure_t* figure;
    int x, y;
    clock_t timer;
    double fall_speed;
    enum es_moving_figure_state state;
};

struct es_field {
    struct es_line lines[ES_NLINES];
    int line_index[ES_NLINES];
};

enum es_game_state {
    ES_IN_PROGRESS = 0,
    ES_GAME_OVER,
    ES_GAME_EXIT,
};

struct es_game {
    enum es_game_state state;
    struct es_moving_figure moving_figure;
    struct es_field field;
    unsigned int score;
    int game_over_line_index;
    unsigned int highscores[ES_SCORES_TO_KEEP];
};

enum es_input {
    ES_INPUT_NONE = 0,
    ES_INPUT_EXIT,
    ES_INPUT_MOVE_LEFT,
    ES_INPUT_MOVE_RIGHT,
    ES_INPUT_ROTATE,
    ES_INPUT_DROP,
};

es_figure_t es_all_figures[ES_NFIGURES] = {
    {
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 0, 0, 0},
    },
    {
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 0, 0, 0},
    },
    {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 1, 1, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
    },
    {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
};

es_figure_t* es_figure_random(clock_t current_time)
{
    return &es_all_figures[current_time % ES_NFIGURES];
}

es_figure_t* es_figure_rotate(es_figure_t* figure)
{
    size_t i = figure - &es_all_figures[0];
    size_t rotated_i = ES_NROTATIONS * (i / ES_NROTATIONS) + (i + 1) % ES_NROTATIONS;
    return &es_all_figures[rotated_i];
}

int time_passed(clock_t* start, double ts, clock_t current_time)
{
    if ((current_time - *start) / (double)CLOCKS_PER_SEC >= ts) {
        *start = current_time;
        return 1;
    }
    return 0;
}

void es_line_init(struct es_line* line, int y)
{
    int x;
    line->state = ES_LINE_EMPTY;
    for (x = 0; x < ES_LINE_WIDTH; ++x)
        line->rocks[x] = 0;
    line->y = y;
    line->nrocks = 0;
}

void es_field_init(struct es_field* field)
{
    for (int i = 0; i < ES_NLINES; ++i) {
        es_line_init(&field->lines[i], i);
        field->line_index[i] = i;
    }
}

int es_field_remove_full_lines(struct es_field* field)
{
    int i, j;
    struct es_line* iline;
    struct es_line* jline;
    int removed = 0;
    for (i = 0, iline = field->lines; i < ES_NLINES; ++i, ++iline) {
        if (iline->state != ES_LINE_FULL)
            continue;
        for (j = 0, jline = field->lines; j < ES_NLINES; ++j, ++jline)
            if (jline->y < iline->y) {
                jline->y += 1;
                field->line_index[jline->y] = j;
            }
        es_line_init(iline, 0);
        field->line_index[0] = i;
        removed += 1;
    }
    return removed;
}

struct es_line* es_field_line(struct es_field* field, int y)
{
    assert(y >= 0);
    assert(y < ES_NLINES);
    return &field->lines[field->line_index[y]];
}

int es_field_is_rock(struct es_field* field, int y, int x)
{
    assert(x >= 0);
    assert(x < ES_LINE_WIDTH);
    return es_field_line(field, y)->rocks[x];
}

int es_field_collides(struct es_field* field,
    es_figure_t figure,
    int y,
    int x)
{
    for (int i = 0; i < ES_FIGURE_HEIGHT; ++i) {
        for (int j = 0; j < ES_FIGURE_WIDTH; ++j) {
            if (!figure[i][j])
                continue;
            int ry = y + i;
            int rx = x + j;
            if (ry < 0 || ry >= ES_NLINES)
                return 1;
            if (rx < 0 || rx >= ES_LINE_WIDTH)
                return 1;
            if (es_field_is_rock(field, ry, rx))
                return 1;
        }
    }
    return 0;
}

void es_field_set_rock(struct es_field* field, int y, int x)
{
    assert(x >= 0);
    assert(x < ES_LINE_WIDTH);
    struct es_line* line = es_field_line(field, y);
    assert(!line->rocks[x]);
    line->rocks[x] = 1;
    line->nrocks += 1;
    if (line->nrocks == ES_LINE_WIDTH)
        line->state = ES_LINE_FULL;
    else
        line->state = ES_LINE_NOT_EMPTY;
}

void es_field_merge(struct es_field* field,
    es_figure_t figure,
    int y,
    int x)
{
    int i, j, ry, rx;
    for (i = 0; i < ES_FIGURE_HEIGHT; ++i) {
        for (j = 0; j < ES_FIGURE_WIDTH; ++j) {
            if (!figure[i][j])
                continue;
            ry = y + i;
            rx = x + j;
            es_field_set_rock(field, ry, rx);
        }
    }
}

int es_field_line_is_empty(struct es_field* field, int y)
{
    return es_field_line(field, y)->state == ES_LINE_EMPTY;
}

void es_moving_figure_init(
    struct es_moving_figure* figure,
    clock_t current_time)
{
    figure->figure = es_figure_random(current_time);
    figure->y = 0;
    figure->x = ES_LINE_WIDTH / 2;
    figure->timer = current_time;
    figure->fall_speed = ES_SPEED;
    figure->state = ES_MOVING_FIGURE_FALLING;
}

void es_moving_figure_update(struct es_moving_figure* moving_figure,
    struct es_field* field,
    enum es_input input,
    clock_t current_time)
{
    if (moving_figure->state == ES_MOVING_FIGURE_STUCK)
        return;

    es_figure_t* rotated;

    switch (input) {
    case ES_INPUT_MOVE_LEFT:
        if (!es_field_collides(field,
            *moving_figure->figure,
            moving_figure->y,
            moving_figure->x - 1))
            moving_figure->x -= 1;
        break;
    case ES_INPUT_MOVE_RIGHT:
        if (!es_field_collides(field,
            *moving_figure->figure,
            moving_figure->y,
            moving_figure->x + 1))
            moving_figure->x += 1;
        break;
    case ES_INPUT_ROTATE:
        rotated = es_figure_rotate(moving_figure->figure);
        if (!es_field_collides(field,
            *rotated,
            moving_figure->y,
            moving_figure->x))
            moving_figure->figure = rotated;
        break;
    case ES_INPUT_DROP:
        if (moving_figure->state == ES_MOVING_FIGURE_FALLING) {
            moving_figure->state = ES_MOVING_FIGURE_DROPPING;
            moving_figure->fall_speed *= ES_DROP_FACTOR;
        }
        break;
    default:
        break;
    }

    if (!time_passed(&moving_figure->timer,
        1.0 / moving_figure->fall_speed,
        current_time))
        return;

    if (!es_field_collides(field,
        *moving_figure->figure,
        moving_figure->y + 1,
        moving_figure->x))
        moving_figure->y += 1;
    else {
        if (moving_figure->state == ES_MOVING_FIGURE_DROPPING) {
            moving_figure->fall_speed /= ES_DROP_FACTOR;
        }
        moving_figure->state = ES_MOVING_FIGURE_STUCK;
    }
}

void es_game_init(struct es_game* game, clock_t current_time)
{
    es_field_init(&game->field);

    es_moving_figure_init(&game->moving_figure, current_time);

    /* init other fields */
    game->score = 0;
    game->state = ES_IN_PROGRESS;
    game->game_over_line_index = ES_FIGURE_HEIGHT - 1;

    for (int i = 0; i < ES_SCORES_TO_KEEP; ++i)
        game->highscores[i] = 0;
}

int es_score_increment(int nlines)
{
    assert(nlines > 0);
    return (1 << (nlines - 1)) * ES_LINE_WIDTH;
}

void es_highscores_update(struct es_game *game)
{
    int insert_pos = -1;
    for (int i = 0; i < ES_SCORES_TO_KEEP; ++i) {
        if (game->score > game->highscores[i]) {
            insert_pos = i;
            break;
        }
    }
    if (insert_pos == -1)
        return;
    for (int j = ES_SCORES_TO_KEEP - 1; j > insert_pos; --j)
        game->highscores[j] = game->highscores[j - 1];
    game->highscores[insert_pos] = game->score;
}

void es_game_update(struct es_game* game,
    enum es_input input,
    clock_t current_time)
{
    if (input == ES_INPUT_EXIT)
        game->state = ES_GAME_EXIT;

    if (game->state != ES_IN_PROGRESS)
        return;

    es_moving_figure_update(&game->moving_figure,
        &game->field,
        input,
        current_time);

    if (game->moving_figure.state != ES_MOVING_FIGURE_STUCK)
        return;

    es_field_merge(&game->field,
        *game->moving_figure.figure,
        game->moving_figure.y,
        game->moving_figure.x);

    int removed_lines = es_field_remove_full_lines(&game->field);
    if (removed_lines)
        game->score += es_score_increment(removed_lines);

    es_moving_figure_init(&game->moving_figure, current_time);

    if (!es_field_line_is_empty(&game->field,
        game->game_over_line_index)) {
        game->state = ES_GAME_OVER;
        es_highscores_update(game);
    }
}

void io_cleanup(void)
{
    endwin();
}

void io_init()
{
    initscr();
    cbreak();
    noecho();
    nonl();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    /* Cleanup ncurses on exit */
    atexit(io_cleanup);
}

enum es_input io_readkey()
{
    switch (getch()) {
    case 27:
        return ES_INPUT_EXIT;
    case KEY_LEFT:
        return ES_INPUT_MOVE_LEFT;
    case KEY_RIGHT:
        return ES_INPUT_MOVE_RIGHT;
    case KEY_UP:
        return ES_INPUT_ROTATE;
    case KEY_DOWN:
        return ES_INPUT_DROP;
    default:
        return ES_INPUT_NONE;
    }
}

void io_draw(struct es_game* game)
{
    int i, j, y, x;

    /* Clear screen */
    erase();

    /* Walls */
    for (i = 0; i < ES_NLINES; ++i) {
        mvaddch(i, 0, '>');
        mvaddch(i, ES_LINE_WIDTH + 1, '<');
    }

    for (j = 0; j < ES_LINE_WIDTH + 2; ++j)
        mvaddch(ES_NLINES, j, '^');

    /* Red line */
    for (j = 0; j < ES_LINE_WIDTH + 2; ++j)
        mvaddch(game->game_over_line_index, j, '-');

    /* Lines */
    for (i = 0; i < ES_NLINES; ++i) {
        y = game->field.lines[i].y;
        for (x = 0; x < ES_LINE_WIDTH; ++x) {
            if (game->field.lines[i].rocks[x]) {
                mvaddch(y, x + 1, '#');
            }
        }
    }

    /* Moving figure */
    es_figure_t* f = game->moving_figure.figure;
    x = game->moving_figure.x;
    y = game->moving_figure.y;
    for (i = 0; i < ES_FIGURE_HEIGHT; ++i) {
        for (j = 0; j < ES_FIGURE_WIDTH; ++j) {
            if ((*f)[i][j]) {
                mvaddch(y + i, x + j + 1, '#');
            }
        }
    }

    /* Score */
    mvprintw(0, ES_LINE_WIDTH + 10,
        "Score: %d", game->score);

    /* State */
    if (game->state == ES_GAME_OVER)
        mvprintw(2, ES_LINE_WIDTH + 10, "Game over");

    /* Highscores */
    mvprintw(11, ES_LINE_WIDTH + 10, "Highscores:");
    for (int i = 0; i < ES_SCORES_TO_KEEP; ++i) {
        mvprintw(12 + i, ES_LINE_WIDTH + 10, "%d: %d", i + 1, game->highscores[i]);
    }

    /* Instructions */
    mvprintw(4, ES_LINE_WIDTH + 10, "How to play:");
    mvprintw(5, ES_LINE_WIDTH + 10, "Left  - Move figure left");
    mvprintw(6, ES_LINE_WIDTH + 10, "Right - Move figure right");
    mvprintw(7, ES_LINE_WIDTH + 10, "Up    - Rotate figure");
    mvprintw(8, ES_LINE_WIDTH + 10, "Down  - Drop figure");
    mvprintw(9, ES_LINE_WIDTH + 10, "Esc   - Exit");

    /* Swap buffers */
    refresh();
}

void io_load_highscores(
    char *file_path, unsigned int* highscores, size_t count)
{
    FILE* f = fopen(file_path, "rb");
    if (f == NULL)
        return;
    size_t result = fread(
        highscores, sizeof(highscores[0]), count, f);
    assert(result == count);
    fclose(f);
}

void io_save_highscores(
    char* file_path, unsigned int* highscores, size_t count)
{
    FILE* f = fopen(file_path, "wb");
    assert(f != NULL);
    size_t result = fwrite(
        highscores, sizeof(highscores[0]), count, f);
    assert(result == count);
    fclose(f);
}

int main()
{
    io_init();
    struct es_game game;
    es_game_init(&game, clock());
    io_load_highscores(
        ES_HIGHSCORES_PATH,
        &(game.highscores),
        ES_SCORES_TO_KEEP
    );
    while (game.state != ES_GAME_EXIT) {
        es_game_update(&game, io_readkey(), clock());
        io_draw(&game);
    }
    io_save_highscores(
        ES_HIGHSCORES_PATH,
        &(game.highscores),
        ES_SCORES_TO_KEEP
    );
    return 0;
}

#ifndef MEWO_H
#define MEWO_H

#include <stdint.h>
#include <stdbool.h>

#define MEWO_DISPLAY_COLS (128)
#define MEWO_DISPLAY_ROWS (64)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_SELF(SELF) SELF

#define FOREACH_MEWO_STATE(FN) \
        FN(MEWO_STATE_SIT) \
        FN(MEWO_STATE_WALK) \
        FN (MEWO_STATE_SLEEP)

typedef enum {
    FOREACH_MEWO_STATE(GENERATE_ENUM)
} mewo_state;

static const char *MEWO_STATE_STRING[] = {
    FOREACH_MEWO_STATE(GENERATE_STRING)
};

typedef struct {
    uint8_t *fdata;
    int num_rows;
    int num_cols;
    int y_offset;
    int x_offset;
} mewo_body_frame_info;

typedef struct {
    uint8_t *fdata;
    int num_rows;
    int num_cols;
} mewo_head_frame_info;


#define FOREACH_MEWO_BODY_FRAME(FN) \
        FN(MEWO_BODY_FRAME_WALK_A_LEFT) \
        FN(MEWO_BODY_FRAME_WALK_A_RIGHT) \
        FN(MEWO_BODY_FRAME_WALK_B_LEFT)  \
        FN(MEWO_BODY_FRAME_WALK_B_RIGHT) \
        FN(MEWO_BODY_FRAME_SIT) \
        FN(MEWO_BODY_FRAME_SIT_TAIL) \
        FN(MEWO_BODY_FRAME_SLEEP) \
        FN(MEWO_BODY_FRAME_LEN)
typedef enum {
    FOREACH_MEWO_BODY_FRAME(GENERATE_ENUM)
} mewo_body_frame;
static const char *MEWO_BODY_FRAME_STRING[] = {
    FOREACH_MEWO_BODY_FRAME(GENERATE_STRING)
};


#define FOREACH_MEWO_HEAD_FRAME(FN) \
        FN(MEWO_HEAD_FRAME_FORWARD) \
        FN(MEWO_HEAD_FRAME_FORWARD_EYES_CLOSED) \
        FN(MEWO_HEAD_FRAME_SIDE_LEFT) \
        FN(MEWO_HEAD_FRAME_SIDE_RIGHT) \
        FN(MEWO_HEAD_FRAME_LEN)
typedef enum {
    FOREACH_MEWO_HEAD_FRAME(GENERATE_ENUM)
} mewo_head_frame;
static const char *MEWO_HEAD_FRAME_STRING[] = {
    FOREACH_MEWO_HEAD_FRAME(GENERATE_STRING)
};


typedef struct {
    char *name;
    uint64_t wakeup_time_ms;
    uint64_t sleep_time_ms;
} mewo_config;

typedef struct {
    mewo_config *config;
    mewo_state state;
    int x_pos;
    int16_t x_speed;
    mewo_body_frame body_frame;
    mewo_body_frame_info *body_frame_info;
    mewo_head_frame head_frame;
    mewo_head_frame_info *head_frame_info;
    bool stale;
    bool vbuffer[MEWO_DISPLAY_ROWS][MEWO_DISPLAY_COLS];
} mewo;

void mewo_init(mewo *m, mewo_config *config);

/**
 * time_sec: Number of seconds into current day. [0, 86400)
*/
void mewo_tick(mewo *m); 

void mewo_refresh(mewo *m);

bool mewo_get_pixel(mewo *m, int x, int y);

void mewo_set_body_frame(mewo *m, mewo_body_frame frame);

void mewo_set_head_frame(mewo *m, mewo_head_frame frame);

#endif

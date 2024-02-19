#ifndef MEWO_FRAMES_H
#define MEWO_FRAMES_H

#include <stdint.h>
#include "mewo.h"
#include "transforms.h"

extern int MEWO_FRAMES_X_OFFSETS[MEWO_BODY_FRAME_LEN][MEWO_HEAD_FRAME_LEN];
extern int MEWO_FRAMES_Y_OFFSETS[MEWO_BODY_FRAME_LEN][MEWO_HEAD_FRAME_LEN];


// Body frames 

extern mewo_body_frame_info MEWO_BODY_SIT;
extern mewo_body_frame_info MEWO_BODY_SIT_TAIL;
extern mewo_body_frame_info MEWO_BODY_WALK_A_LEFT;
extern mewo_body_frame_info MEWO_BODY_WALK_A_RIGHT;
extern mewo_body_frame_info MEWO_BODY_WALK_B_LEFT;
extern mewo_body_frame_info MEWO_BODY_WALK_B_RIGHT;
extern mewo_body_frame_info MEWO_BODY_SLEEP;


// Head frames

extern mewo_head_frame_info MEWO_HEAD_FORWARD;
extern mewo_head_frame_info MEWO_HEAD_FORWARD_EYES_CLOSED;
extern mewo_head_frame_info MEWO_HEAD_SIDE_LEFT;
extern mewo_head_frame_info MEWO_HEAD_SIDE_RIGHT;

void mewo_frames_init(); 


#endif

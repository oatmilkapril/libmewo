#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include <stdint.h>

void mirror_x(uint8_t *orig_frame, uint8_t *new_frame, int num_rows, int num_cols);

#endif
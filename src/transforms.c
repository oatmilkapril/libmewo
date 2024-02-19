#include "transforms.h"

static uint8_t _reverse_byte(uint8_t value);

void mirror_x(uint8_t *orig_frame, uint8_t *new_frame, int num_rows, int num_cols) {
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {
            uint8_t value = orig_frame[row * num_cols + col];
            int new_col = num_cols - 1 - col;
            uint8_t new_value = _reverse_byte(value);
            new_frame[row * num_cols + new_col] = new_value;
        }
    }
}

static uint8_t reverse_byte_lookup[16] = {
    0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe, 0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf
};

static uint8_t _reverse_byte(uint8_t value) {
    return reverse_byte_lookup[value & 0xf] << 4 | reverse_byte_lookup[value >> 4];
}

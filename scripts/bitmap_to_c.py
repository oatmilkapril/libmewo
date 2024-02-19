import argparse
import logging
from PIL import Image 
import copy


def print_ascii(mono_pixels: list[list[bool]]):
    for row in mono_pixels:
        for pixel in row:
            if pixel:
                print('o', end='')
            else:
                print('_', end='')
        print('')


def print_c(mono_pixels: list[list[bool]]):
    print('{')
    for row_index, row in enumerate(mono_pixels):
        data = 0
        col_index = 0
        print('    {', end='')

        while col_index < len(row):
            if row[col_index]:
                data = data | (1 << (7 - col_index % 8))
            if col_index % 8 == 7:
                # Current byte is complete. Print it
                print(f'0x{data:02x}', end='')
                if col_index < len(row) - 1:
                    # Print delimiter if not last column in row
                    print(', ', end='')
                data = 0
            col_index += 1

        if col_index % 8 != 0:
            print(f'0x{data:02x}', end='')

        print('}', end='')
        if row_index < len(mono_pixels) - 1:
            # Print delimiter if not last row
            print(',', end='')
        print()

    print('}')


def shift_image(mono_pixels: list[list[bool]]) -> list[list[bool]]:
    right_bound_col_index = None
    left_bound_col_index = None
    top_bound_row_index = None
    bottom_bound_row_index = None

    shifted_mono_pixels = copy.deepcopy(mono_pixels)

    num_rows = len(shifted_mono_pixels)
    num_cols = len(shifted_mono_pixels[0])

    for row_index, row in enumerate(shifted_mono_pixels):
        for col_index, col in enumerate(row):
            if col:
                if top_bound_row_index is None or row_index < top_bound_row_index:
                    # Found highest pixel in buffer
                    top_bound_row_index = row_index
                if bottom_bound_row_index is None or row_index > bottom_bound_row_index:
                    # Found lowest pixel in buffer
                    bottom_bound_row_index = row_index
                if left_bound_col_index is None or col_index < left_bound_col_index:
                    # Found left-most pixel in buffer
                    left_bound_col_index = col_index
                if right_bound_col_index is None or col_index > right_bound_col_index:
                    # Found right-most pixel in buffer
                    right_bound_col_index = col_index

    # Shift buffer left to first set pixel
    for row in shifted_mono_pixels:
        del row[0:left_bound_col_index]
        for _ in range(left_bound_col_index):
            row.append(False)

    # Shift buffer down to first set pixel
    if bottom_bound_row_index < num_rows - 1:
        del shifted_mono_pixels[bottom_bound_row_index+1: num_rows]
        for _ in range(num_rows - 1 - bottom_bound_row_index):
            empty_row = [None for i in range(num_cols)]
            shifted_mono_pixels.insert(0, empty_row)

    return shifted_mono_pixels

def trim_image_to(mono_pixels: list[list[bool]], width: int, height: int) -> list[list[bool]]:
    assert height < len(mono_pixels)
    assert width < len(mono_pixels[0])

    trimmed_mono_pixels = copy.deepcopy(mono_pixels)
    num_rows = len(trimmed_mono_pixels)
    num_cols = len(trimmed_mono_pixels[0])

    # Trim rows
    del trimmed_mono_pixels[0:num_rows-height]

    # Trim columns from each row
    for row in trimmed_mono_pixels:
        del row[width: num_cols]

    return trimmed_mono_pixels


def trim_image(mono_pixels: list[list[bool]]) -> list[list[bool]]:
    right_bound_col_index = None
    top_bound_row_index = None

    trimmed_mono_pixels = copy.deepcopy(mono_pixels)
    num_cols = len(trimmed_mono_pixels[0])

    for row_index, row in enumerate(trimmed_mono_pixels):
        for col_index, col in enumerate(row):
            if col:
                if top_bound_row_index is None or row_index < top_bound_row_index:
                    # Found highest pixel in buffer
                    top_bound_row_index = row_index
                if right_bound_col_index is None or col_index > right_bound_col_index:
                    # Found right-most pixel in buffer
                    right_bound_col_index = col_index

    # Trim rows
    del trimmed_mono_pixels[0:top_bound_row_index]

    # Trim columns from each row
    for row in trimmed_mono_pixels:
        if right_bound_col_index + 1 < num_cols:
            del row[right_bound_col_index + 1: num_cols]

    return trimmed_mono_pixels


def process(image_path: str):
    img = Image.open(image_path)
    width = img.size[0]
    height = img.size[1]
    pixels = list(img.getdata())

    mono_pixels = [[None for i in range(width)] for j in range(height)]
    for (pixel_index, pixel_value) in enumerate(pixels):
        col_index = pixel_index % width
        row_index = pixel_index // width

        mono_pixels[row_index][col_index] = not (pixel_value == (255, 255, 255))

    mono_pixels = shift_image(mono_pixels)
    mono_pixels = trim_image(mono_pixels)
    print_c(mono_pixels)
    # print_ascii(mono_pixels)


def main():
    parser = argparse.ArgumentParser(
        prog='bitmap_to_c',
        description='Script to convert bitmap images to C arrays'
    )
    parser.add_argument('--img',
        required=True,
        help='Bitmap image to convert'
    )
    args = parser.parse_args()
    logging.basicConfig(level='INFO')

    process(args.img)


if __name__ == "__main__":
    main()
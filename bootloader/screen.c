
void print_char(char character, int col, int row, char attribute_byte) {
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;

    if (!attribute_byte)
        attribute_byte = WHITE_ON_BLACK;

    int offset;
    if (col >= 0 && row >= 0)
        offset = get_screen_offset(col, row);
    else
        offset = get_cursor();

    if (character == '\n') {
        int rows = offset / (2 * MAX_COLS);
        offset = get_screen_offset(79, rows);
    }
    else {
        vidmem[offset] = character;
        vidmem[offset + 1] = attribute_byte;
    }

    offset += 2;
    offset = handle_scrolling(offset);
    set_cursor(offset);
}

int get_screen_offset(int col, int row) {
    return (row * MAX_COLS + col) * 2;
}

void set_cursor(int offset) {
    offset /= 2;

    // 14 = high byte of cursor location
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    // 15 = low byte of cursor location
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset));
}

int get_cursor() {
    // 14 = high byte of cursor location
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    // 15 = low byte of cursor location
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);

    return offset * 2;
}

void print_at(char *message, int col, int row) {
    /* Set to given col, row if col and row make sense */
    if (col >= 0 && row >= 0)
        set_cursor(get_screen_offset(col, row));

    int i = 0;
    while (message[i] != 0)
        print_char(message[i++], col, row, WHITE_ON_BLACK);
}

void print(char *message) {
    print_at(message, -1, -1);
}

void clear_screen() {
    int row = 0;
    int col = 0;

    for (row = 0; row < MAX_ROWS; row++)
        for (col = 0; col < MAX_COLS; col++)
            print_char(' ', col, row, WHITE_ON_BLACK);

    set_cursor(get_screen_offset(0, 0));
}

void memory_copy(char *source, char *dest, int no_bytes) {
    int i;
    for (i = 0; i < no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}

int handle_scrolling(int cursor_offset) {
    if (cursor_offset < MAX_ROWS * MAX_COLS * 2)
        return cursor_offset;

    int i;
    for (i = 0; i < MAX_ROWS; i++) {
        memory_copy(get_screen_offset(0, i) + VIDEO_ADDRESS,
                get_screen_offset(0, i - 1) + VIDEO_ADDRESS,
                MAX_COLS * 2);
    }

    char *last_line = get_screen_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS;
    for (i = 0; i < MAX_COLS * 2; i++) {
        last_line[i] = 0;
    }

    cursor_offset -= 2 * MAX_COLS;

    return cursor_offset;
}

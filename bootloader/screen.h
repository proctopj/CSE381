#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void memory_copy(char *source, char *dest, int no_bytes);
int handle_scrolling(int cursor_offset);
int get_screen_offset(int col, int row);
void set_cursor(int offset);
int get_cursor();
void print_char(char character, int col, int row, char attribute_byte);
void print_at(char *message, int col, int row);
void print(char *message);
void clear_screen();

#include "kernel/low_level.c"

#include "screen.c"

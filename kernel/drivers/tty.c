#include <kernel/tty.h>
#include <kernel/io.h>
#include <limine.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

struct limine_terminal *terminal;

void tty_init() {
    if (terminal_request.response == NULL|| terminal_request.response->terminal_count < 1) {
        panic("No Terminal Found");
    }
    sprintf("Found %d terminal(s).\r\n", terminal_request.response->terminal_count);
    terminal = terminal_request.response->terminals[0];
    sprintf("TTY Initialized\r\n\r\n");
}

void tty_write(const char *str, uint64_t size) {
    terminal_request.response->write(terminal, str, size);
}

void tty_putchar(const char c) {
    terminal_request.response->write(terminal, &c, 1);
}
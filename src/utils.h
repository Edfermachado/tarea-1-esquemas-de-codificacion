#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdint.h>

// Funciones de utilidad común
void *safe_malloc(size_t size);
char *string_duplicate(const char *src);
int is_valid_bitstream(const char *str);
void print_binary(uint8_t byte);

#endif
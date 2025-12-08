#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stddef.h>
#include <stdio.h>

// Función puntero para codificación/decodificación
typedef char* (*encode_fn)(const char*);
typedef char* (*decode_fn)(const char*);

// Genera un mensaje aleatorio de n bits ('0'/'1')
char* generate_random_bits(size_t n);

// Cuenta el número de bits diferentes entre dos cadenas
size_t count_bit_errors(const char* original, const char* received);

// Devuelve el tamaño del mensaje codificado (overhead)
size_t get_encoded_length(const char* bitstream, encode_fn encode);

// Ejecuta N simulaciones con ruido y reporta estadísticas
void run_simulations(FILE* md, const char* bitstream, double ber, int N, 
                     const char* name, encode_fn encode, decode_fn decode);

// Simula ráfagas de errores de longitud 'burst_len'
void simulate_burst_errors(char* bitstream, double ber, size_t burst_len);

#endif

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stddef.h>
#include <stdio.h>

// 1. Definición de tipos para punteros a funciones (Hacer que coincidan con encoding.h)
typedef char* (*encode_ptr)(const char*);
typedef char* (*decode_ptr)(const char*);

// 2. Generación y Utilidades
char* generate_random_bits(size_t n);
size_t count_bit_errors(const char* original, const char* received);
size_t get_encoded_length(const char* bitstream, encode_ptr encode);

// 3. Reporte de Análisis (Parte B)
void prepare_analysis_report(const char *filename, const char *cedula, double personal_ber);

// 4. Simulaciones Estadísticas
// NOTA: Usamos los typedef encode_ptr/decode_ptr para que la firma sea limpia y consistente
void run_simulations(const char *filename, const char *bitstream, double ber, int N,
                     const char *name, encode_ptr encode, decode_ptr decode);

void run_ber_sensitivity_analysis(const char *filename, const char *bitstream);

// 5. Inyección de Errores
void simulate_burst_errors(char* bitstream, double ber, size_t burst_len);

#endif
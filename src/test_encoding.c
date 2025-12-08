#include "encoding.h"
#include "analysis.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Función auxiliar para comparar strings de bits
void test_equal(const char *test_name, const char *expected, const char *actual)
{
    if (strcmp(expected, actual) != 0)
    {
        fprintf(stderr, "❌ %s falló.\nEsperado: %s\nObtenido: %s\n", test_name,
                expected, actual);
        exit(1);
    }
    else
    {
        printf("✅ %s pasó.\n", test_name);
    }
}

int main(void)
{

    FILE *f_init = fopen("results/signals.txt", "w");
    if (f_init)
    {
        fprintf(f_init, "=== REPORTE DE SEÑALES GENERADO AUTOMÁTICAMENTE ===\n");
        fclose(f_init);
    }

    printf("=== Iniciando pruebas automáticas de codificación ===\n");

    const size_t MSG_LEN = 1000;
    const int N = 50;
    const double ber = 0.01; // Cedula => 0.1%

    // Casos de prueba base — ajusta o amplía según tu curso
    const char *bitstream = "110010";

    // NRZ
    char *enc_nrz = encode_nrz(bitstream);
    char *dec_nrz = decode_nrz(enc_nrz);
    test_equal("NRZ encode/decode", bitstream, dec_nrz);
    add_noise(enc_nrz, ber);
    plot_signal(enc_nrz, "results/signals.txt");
    free(enc_nrz);
    free(dec_nrz);

    // NRZI
    char *enc_nrzi = encode_nrzi(bitstream);
    char *dec_nrzi = decode_nrzi(enc_nrzi);
    test_equal("NRZI encode/decode", bitstream, dec_nrzi);
    add_noise(enc_nrzi, ber);
    plot_signal(enc_nrzi, "results/signals.txt");
    free(enc_nrzi);
    free(dec_nrzi);

    // Manchester
    char *enc_man = encode_manchester(bitstream);
    char *dec_man = decode_manchester(enc_man);
    test_equal("Manchester encode/decode", bitstream, dec_man);
    add_noise(enc_man, ber);
    plot_signal(enc_man, "results/signals.txt");
    free(enc_man);
    free(dec_man);

    // 4B/5B
    const char *bitstream_4b = "101011110000"; // múltiplo de 4 bits
    char *enc_4b5b = encode_4b5b(bitstream_4b);
    char *dec_4b5b = decode_4b5b(enc_4b5b);
    test_equal("4B/5B encode/decode", bitstream_4b, dec_4b5b);
    add_noise(enc_4b5b, ber);
    plot_signal(enc_4b5b, "results/signals.txt");
    free(enc_4b5b);
    free(dec_4b5b);

    printf("🎉 Todas las pruebas automáticas pasaron correctamente.\n");

    // Parte 2: Simulaciones estadísticas con mensaje aleatorio
    // -------------------------------
    printf("--- Simulaciones estadísticas ---\n");

    // Para 4B/5B:
    size_t len4b = (MSG_LEN / 4) * 4; // Redondear a múltiplo de 4
    char *bitstream_4b_simulation = generate_random_bits(len4b);

    char *bitstream_simulation = generate_random_bits(MSG_LEN);
    if (!bitstream_simulation)
    {
        fprintf(stderr, "No se pudo generar el mensaje de prueba.\n");
        return 1;
    }

    // Crear archivo Markdown para resultados
    FILE *md = fopen("results/analysis.md", "w");
    if (!md)
    {
        fprintf(stderr, "No se pudo crear results/analysis.md\n");
        free(bitstream_simulation);
        return 1;
    }

    prepare_analysis_report("results/analysis.md", "30532641", ber);

    // Simulaciones con ruido
    run_simulations("results/analysis.md", bitstream_simulation, ber, N, "NRZ", encode_nrz, decode_nrz);
    run_simulations("results/analysis.md", bitstream_simulation, ber, N, "NRZI", encode_nrzi, decode_nrzi);
    run_simulations("results/analysis.md", bitstream_simulation, ber, N, "Manchester", encode_manchester, decode_manchester);
    run_simulations("results/analysis.md", bitstream_4b_simulation, ber, N, "4B/5B", encode_4b5b, decode_4b5b);

    fclose(md);

    run_ber_sensitivity_analysis("results/analysis.md", bitstream_simulation);

    free(bitstream_simulation);
    free(bitstream_4b_simulation);

    printf("📊 Análisis generado en results/analysis.md\n");

    return 0;
}
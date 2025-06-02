#ifndef STATISTICAL_H
#define STATISTICAL_H

#include <graphics.h>
#include <time.h>

typedef struct {
    double data[1000];
    int count;
    char source[256]; 
    int is_loaded;
} DataSet;

// Estructura para mantener resultados y métricas de rendimiento
typedef struct {
    double result;           // Resultado del cálculo
    double processing_time;  // Tiempo en milisegundos
    int operations_count;    // Número de operaciones realizadas
    char description[100];   // Descripción de lo calculado
} StatResult;

// Funciones para estadísticas básicas
StatResult calc_mean(DataSet* data);
StatResult calc_median(DataSet* data);
StatResult calc_mode(DataSet* data);
StatResult calc_trimmed_mean(DataSet* data, double percentage);
StatResult calc_trimmed_mean_by_count(DataSet* data, int count);

// Funciones para varianza y desviación estándar
// Versión para muestras: división por (n-1)
StatResult calc_variance_sample(DataSet* data);
StatResult calc_stddev_sample(DataSet* data);

// Versión para poblaciones: división por n
StatResult calc_variance_population(DataSet* data);
StatResult calc_stddev_population(DataSet* data);

// Función de utilidad para ordenar datos (si no existe ya)
double* get_sorted_data(DataSet* data);

// Función para mostrar resultados en la interfaz
void display_stat_result(StatResult result, int y_position);

// Funciones para mostrar los resultados en la interfaz
void show_trimmed_mean_options();
void show_basic_stats();
void show_dispersion_stats();

#endif // STATISTICAL_H
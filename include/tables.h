#ifndef TABLES_H
#define TABLES_H

#include <graphics.h>
#include "excel_c.h"

// Constantes para las tablas
#define MAX_TABLE_SIZE 500
#define MAX_GL 100
#define MAX_ALPHA_VALUES 9

// Estructura para almacenar puntos para gráficos
typedef struct {
    int x;
    int y;
} Point;

// Funciones principales para tablas
void calculate_alpha_z(double z_value);           // Calcula el valor alpha (área bajo la curva)
void find_z_from_alpha(double alpha_value);       // Encuentra Z dado un valor de alpha
void search_t_table(int df, double alpha);        // Busca en tabla T para grados de libertad y alpha
void find_df_alpha_from_t(double t_value);        // Encuentra grados libertad y alpha dado T

// Funciones auxiliares
double normal_pdf(double x);                      // Función de densidad de probabilidad normal
double normal_cdf(double x);                      // Función de distribución acumulada normal
double normal_cdf_integrate(double z);            // Integración numérica para calcular CDF normal
double inverse_normal_cdf(double alpha);          // Función inversa de CDF normal para encontrar Z
double t_distribution_value(int df, double alpha); // Valor de la distribución T para df y alpha
void find_closest_t_values(double t_value, int* closest_df, double* closest_alpha); // Encontrar df y alpha cercanos

// Funciones de visualización
void draw_normal_curve(int x0, int y0, int width, int height, double z_value);
void draw_t_distribution(int x0, int y0, int width, int height, int df, double alpha);
void display_normal_curve_info(double z_value, double alpha);
void display_t_distribution_info(int df, double alpha, double t_value);

#endif // TABLES_H
#include "../../include/tables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// Constantes matemáticas
#define PI 3.14159265358979323846
#define SQRT_2PI 2.5066282746310002

// Valores comunes de alpha para tablas estadísticas (1 cola)
const double common_alpha_values[] = {0.001, 0.005, 0.010, 0.025, 0.050, 0.100, 0.150, 0.200, 0.500};
const int num_alpha_values = 9;

// Tabla T pre-calculada (grados libertad vs alpha)
// Formato: t_table[df][alpha_index] donde alpha_index corresponde a common_alpha_values
// Estos son valores típicos para T de una cola, df desde 1 hasta 30, y algunos valores seleccionados después
const double t_table[31][9] = {
    // df=1
    {318.31, 63.66, 31.82, 12.71, 6.31, 3.08, 1.96, 1.38, 0.00},
    // df=2
    {22.33, 9.92, 6.96, 4.30, 2.92, 1.89, 1.39, 1.06, 0.00},
    // df=3
    {10.21, 5.84, 4.54, 3.18, 2.35, 1.64, 1.25, 0.98, 0.00},
    // df=4
    {7.17, 4.60, 3.75, 2.78, 2.13, 1.53, 1.19, 0.94, 0.00},
    // df=5
    {5.89, 4.03, 3.36, 2.57, 2.02, 1.48, 1.16, 0.92, 0.00},
    // df=6
    {5.21, 3.71, 3.14, 2.45, 1.94, 1.44, 1.13, 0.91, 0.00},
    // df=7
    {4.79, 3.50, 3.00, 2.36, 1.90, 1.42, 1.12, 0.90, 0.00},
    // df=8
    {4.50, 3.36, 2.90, 2.31, 1.86, 1.40, 1.11, 0.89, 0.00},
    // df=9
    {4.30, 3.25, 2.82, 2.26, 1.83, 1.38, 1.10, 0.88, 0.00},
    // df=10
    {4.14, 3.17, 2.76, 2.23, 1.81, 1.37, 1.09, 0.88, 0.00},
    // df=11
    {4.02, 3.11, 2.72, 2.20, 1.80, 1.36, 1.09, 0.88, 0.00},
    // df=12
    {3.93, 3.05, 2.68, 2.18, 1.78, 1.36, 1.08, 0.87, 0.00},
    // df=13
    {3.85, 3.01, 2.65, 2.16, 1.77, 1.35, 1.08, 0.87, 0.00},
    // df=14
    {3.79, 2.98, 2.62, 2.14, 1.76, 1.34, 1.08, 0.87, 0.00},
    // df=15
    {3.73, 2.95, 2.60, 2.13, 1.75, 1.34, 1.07, 0.87, 0.00},
    // df=16
    {3.69, 2.92, 2.58, 2.12, 1.75, 1.34, 1.07, 0.87, 0.00},
    // df=17
    {3.65, 2.90, 2.57, 2.11, 1.74, 1.33, 1.07, 0.86, 0.00},
    // df=18
    {3.61, 2.88, 2.55, 2.10, 1.73, 1.33, 1.07, 0.86, 0.00},
    // df=19
    {3.58, 2.86, 2.54, 2.09, 1.73, 1.33, 1.07, 0.86, 0.00},
    // df=20
    {3.55, 2.85, 2.53, 2.09, 1.72, 1.32, 1.06, 0.86, 0.00},
    // df=21
    {3.53, 2.83, 2.52, 2.08, 1.72, 1.32, 1.06, 0.86, 0.00},
    // df=22
    {3.51, 2.82, 2.51, 2.07, 1.72, 1.32, 1.06, 0.86, 0.00},
    // df=23
    {3.49, 2.81, 2.50, 2.07, 1.71, 1.32, 1.06, 0.86, 0.00},
    // df=24
    {3.47, 2.80, 2.49, 2.06, 1.71, 1.32, 1.06, 0.86, 0.00},
    // df=25
    {3.45, 2.79, 2.48, 2.06, 1.71, 1.32, 1.06, 0.86, 0.00},
    // df=26
    {3.44, 2.78, 2.48, 2.06, 1.71, 1.32, 1.06, 0.86, 0.00},
    // df=27
    {3.42, 2.77, 2.47, 2.05, 1.70, 1.31, 1.06, 0.86, 0.00},
    // df=28
    {3.41, 2.76, 2.47, 2.05, 1.70, 1.31, 1.06, 0.86, 0.00},
    // df=29
    {3.40, 2.76, 2.46, 2.05, 1.70, 1.31, 1.06, 0.85, 0.00},
    // df=30
    {3.39, 2.75, 2.46, 2.04, 1.70, 1.31, 1.06, 0.85, 0.00},
    // Infinito (Z)
    {3.09, 2.58, 2.33, 1.96, 1.65, 1.28, 1.04, 0.84, 0.00}
};

// Función de densidad de probabilidad normal
double normal_pdf(double x) {
    return exp(-0.5 * x * x) / SQRT_2PI;
}

// Integración numérica para calcular CDF normal (método Trapezoidal)
double normal_cdf_integrate(double z) {
    if (z < -5.0) return 0.0;  // Aproximación para valores extremos
    if (z > 5.0) return 1.0;   // Aproximación para valores extremos
    
    // Integración numérica
    const int n = 500;  // Número de segmentos
    const double dx = z / n;
    double sum = 0.5 * normal_pdf(0.0);  // Primer término
    
    for (int i = 1; i < n; i++) {
        double x = i * dx;
        sum += normal_pdf(x);
    }
    
    sum += 0.5 * normal_pdf(z);  // Último término
    sum *= dx;   // Multiplicar por dx
    
    // El área hasta z desde -∞ es 0.5 + integral de 0 a z
    return 0.5 + sum;
}

// Función de distribución acumulada normal (aproximación)
double normal_cdf(double x) {
    // Usando una aproximación directa para evitar la integración numérica
    // Aproximación de Abramowitz & Stegun (26.2.17)
    double t, z, ans;
    
    z = fabs(x);
    
    if (z > 12.0) {
        return (x < 0.0) ? 0.0 : 1.0;
    }
    
    t = 1.0 / (1.0 + 0.2316419 * z);
    ans = t * (0.31938153 + t * (-0.356563782 + t * (1.781477937 + 
              t * (-1.821255978 + t * 1.330274429))));
    ans = 1.0 - normal_pdf(z) * ans;
    
    return (x < 0.0) ? (1.0 - ans) : ans;
}

// Función inversa de CDF normal para encontrar Z (método de aproximación)
double inverse_normal_cdf(double p) {
    // Aproximación de Acklam para la inversa de la normal estándar
    if (p <= 0.0) return -DBL_MAX;
    if (p >= 1.0) return DBL_MAX;
    
    if (p < 0.5) {
        return -inverse_normal_cdf(1.0 - p);
    }
    
    const double a[6] = {
        -3.969683028665376e+01, 2.209460984245205e+02,
        -2.759285104469687e+02, 1.383577518672690e+02,
        -3.066479806614716e+01, 2.506628277459239e+00
    };
    
    const double b[5] = {
        -5.447609879822406e+01, 1.615858368580409e+02,
        -1.556989798598866e+02, 6.680131188771972e+01,
        -1.328068155288572e+01
    };
    
    double q = p - 0.5;
    double r, result;
    
    if (fabs(q) <= 0.425) {
        r = 0.180625 - q * q;
        result = q * (((((a[5] * r + a[4]) * r + a[3]) * r + a[2]) * r + a[1]) * r + a[0]) /
                 (((((b[4] * r + b[3]) * r + b[2]) * r + b[1]) * r + b[0]) * r + 1.0);
        return result;
    }
    
    r = (q < 0.0) ? p : (1.0 - p);
    r = sqrt(-log(r));
    
    if (r <= 5.0) {
        r = r - 1.6;
        result = (((((a[5] * r + a[4]) * r + a[3]) * r + a[2]) * r + a[1]) * r + a[0]) /
                 (((((b[4] * r + b[3]) * r + b[2]) * r + b[1]) * r + b[0]) * r + 1.0);
    } else {
        r = r - 5.0;
        result = (((((a[5] * r + a[4]) * r + a[3]) * r + a[2]) * r + a[1]) * r + a[0]) /
                 (((((b[4] * r + b[3]) * r + b[2]) * r + b[1]) * r + b[0]) * r + 1.0);
    }
    
    return (q < 0.0) ? -result : result;
}

// Valor de la distribución T para df y alpha
double t_distribution_value(int df, double alpha) {
    // Buscar en la tabla el valor más cercano
    if (df < 1) df = 1;
    if (df > 30) df = 30; // Para grados de libertad mayores, usamos el valor más cercano
    
    // Encontrar el índice de alpha más cercano
    int alpha_idx = -1;
    double min_diff = 1.0;
    
    for (int i = 0; i < num_alpha_values; i++) {
        double diff = fabs(alpha - common_alpha_values[i]);
        if (diff < min_diff) {
            min_diff = diff;
            alpha_idx = i;
        }
    }
    
    if (alpha_idx == -1) {
        return 0.0; // No se encontró un valor adecuado
    }
    
    return t_table[df-1][alpha_idx];
}

// Encontrar grados de libertad y alpha para un valor T dado
void find_closest_t_values(double t_value, int* closest_df, double* closest_alpha) {
    double min_diff = DBL_MAX;
    *closest_df = 1;
    *closest_alpha = 0.05; // Default to common value
    
    // Iterar por todos los df y alpha en la tabla
    for (int df = 1; df <= 30; df++) {
        for (int a_idx = 0; a_idx < num_alpha_values; a_idx++) {
            double table_t = t_table[df-1][a_idx];
            double diff = fabs(table_t - t_value);
            
            if (diff < min_diff && table_t > 0) {
                min_diff = diff;
                *closest_df = df;
                *closest_alpha = common_alpha_values[a_idx];
            }
        }
    }
}

// Función principal para calcular alpha (tabla Z)
void calculate_alpha_z(double z_value) {
    clear_work_area();
    
    // Título
    setcolor(APP_COLOR_TITLE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    char title[50];
    sprintf(title, "Calculo de Alpha para Z = %.4f", z_value);
    outtextxy(WINDOW_WIDTH/2, 50, title);
    
    // Calcular el valor de alpha (área a la derecha de Z para valores positivos, a la izquierda para negativos)
    double alpha;
    if (z_value >= 0) {
        alpha = 1.0 - normal_cdf(z_value);
    } else {
        alpha = normal_cdf(z_value);
    }
    
    // Dibujar la curva normal con la región sombreada
    draw_normal_curve(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2, 400, 200, z_value);
    
    // Mostrar resultados
    display_normal_curve_info(z_value, alpha);
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

// Función principal para encontrar Z dado un valor de alpha
void find_z_from_alpha(double alpha_value) {
    clear_work_area();
    
    // Validar alpha (debe estar entre 0 y 0.5 para una cola)
    if (alpha_value <= 0 || alpha_value >= 1) {
        setcolor(RED);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 20, (char*)"Error: Alpha debe estar entre 0 y 1");
        
        while (!ismouseclick(WM_LBUTTONDOWN)) {
            delay(100);
        }
        clearmouseclick(WM_LBUTTONDOWN);
        return;
    }
    
    // Título
    setcolor(APP_COLOR_TITLE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    char title[50];
    sprintf(title, "Valor Z para Alpha = %.4f", alpha_value);
    outtextxy(WINDOW_WIDTH/2, 50, title);
    
    // Calcular el valor Z con la inversa de la normal
    double z_value;
    // Si alpha > 0.5, trabajamos con 1-alpha para obtener el Z negativo
    if (alpha_value > 0.5) {
        z_value = -inverse_normal_cdf(alpha_value);
    } else {
        z_value = inverse_normal_cdf(1.0 - alpha_value);
    }
    
    // Dibujar la curva normal con la región sombreada
    draw_normal_curve(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2, 400, 200, z_value);
    
    // Mostrar resultados
    display_normal_curve_info(z_value, alpha_value);
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

// Función principal para buscar en tabla T
void search_t_table(int df, double alpha) {
    clear_work_area();
    
    // Validar parámetros
    if (df < 1 || alpha <= 0 || alpha >= 1) {
        setcolor(RED);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 20, (char*)"Error: Parametros invalidos");
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 20, (char*)"GL >= 1 y 0 < Alpha < 1");
        
        while (!ismouseclick(WM_LBUTTONDOWN)) {
            delay(100);
        }
        clearmouseclick(WM_LBUTTONDOWN);
        return;
    }
    
    // Título
    setcolor(APP_COLOR_TITLE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    char title[100];
    sprintf(title, "Distribucion T: GL = %d, Alpha = %.4f", df, alpha);
    outtextxy(WINDOW_WIDTH/2, 50, title);
    
    // Obtener el valor T de la tabla
    double t_value = t_distribution_value(df, alpha);
    
    // Dibujar la distribución T
    draw_t_distribution(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2, 400, 200, df, alpha);
    
    // Mostrar información
    display_t_distribution_info(df, alpha, t_value);
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

// Función principal para encontrar GL y Alpha dado un valor T
void find_df_alpha_from_t(double t_value) {
    clear_work_area();
    
    // Validar T (debe ser positivo para este caso)
    if (t_value <= 0) {
        setcolor(RED);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 20, (char*)"Error: T debe ser positivo");
        
        while (!ismouseclick(WM_LBUTTONDOWN)) {
            delay(100);
        }
        clearmouseclick(WM_LBUTTONDOWN);
        return;
    }
    
    // Título
    setcolor(APP_COLOR_TITLE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    char title[50];
    sprintf(title, "Busqueda para T = %.4f", t_value);
    outtextxy(WINDOW_WIDTH/2, 50, title);
    
    // Buscar los valores más cercanos
    int closest_df;
    double closest_alpha;
    find_closest_t_values(t_value, &closest_df, &closest_alpha);
    
    // Dibujar la distribución T
    draw_t_distribution(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2, 400, 200, closest_df, closest_alpha);
    
    // Mostrar resultados encontrados
    setcolor(APP_COLOR_SUBTITLE);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    
    char result[100];
    sprintf(result, "Valores mas cercanos encontrados en la tabla T:");
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 120, result);
    
    setcolor(BLACK);
    sprintf(result, "Grados de Libertad: %d", closest_df);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 150, result);
    
    sprintf(result, "Alpha: %.4f", closest_alpha);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 175, result);
    
    double table_t = t_distribution_value(closest_df, closest_alpha);
    sprintf(result, "Valor T correspondiente: %.4f", table_t);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 200, result);
    
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        delay(100);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

// Función para dibujar la curva normal
void draw_normal_curve(int x0, int y0, int width, int height, double z_value) {
    // Parámetros para el gráfico
    double x_min = -4.0;
    double x_max = 4.0;
    double y_max = normal_pdf(0); // Valor máximo de la PDF normal
    
    // Puntos para la curva normal
    Point curve_points[MAX_TABLE_SIZE];
    int num_points = 0;
    
    // Crear los puntos de la curva
    for (int i = 0; i < width && num_points < MAX_TABLE_SIZE; i++) {
        double x = x_min + ((double)i / width) * (x_max - x_min);
        double y = normal_pdf(x);
        
        // Convertir a coordenadas de pantalla
        curve_points[num_points].x = x0 + i;
        curve_points[num_points].y = y0 - (int)(y * height / y_max);
        num_points++;
    }
    
    // Dibujar ejes
    setcolor(APP_COLOR_AXIS);
    line(x0, y0, x0 + width, y0);  // Eje X
    line(x0, y0, x0, y0 - height); // Eje Y
    
    // Dibujar marcas en el eje X
    for (double x = -3; x <= 3; x += 1) {
        int screen_x = x0 + (int)((x - x_min) / (x_max - x_min) * width);
        line(screen_x, y0, screen_x, y0 + 5);
        
        // Etiquetas con valores de x
        char label[10];
        sprintf(label, "%.0f", x);
        settextjustify(CENTER_TEXT, TOP_TEXT);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        outtextxy(screen_x, y0 + 7, label);
    }
    
    // Dibujar área bajo la curva
    int z_screen_x = x0 + (int)((z_value - x_min) / (x_max - x_min) * width);
    
    // Área a la derecha de Z (alpha para Z positivo) o a la izquierda (alpha para Z negativo)
    setfillstyle(SOLID_FILL, COLOR(200, 100, 100));
    
    // Puntos para el área sombreada
    int shade_points[MAX_TABLE_SIZE * 2];
    int num_shade_points = 0;
    
    if (z_value >= 0) {
        // Para Z positivo, sombrear el área a la derecha
        for (int i = 0; i < num_points && num_shade_points < MAX_TABLE_SIZE*2 - 2; i++) {
            if (curve_points[i].x >= z_screen_x) {
                shade_points[num_shade_points++] = curve_points[i].x;
                shade_points[num_shade_points++] = curve_points[i].y;
            }
        }
        if (num_shade_points > 0) {
            shade_points[num_shade_points++] = x0 + width;
            shade_points[num_shade_points++] = y0;
            shade_points[num_shade_points++] = z_screen_x;
            shade_points[num_shade_points++] = y0;
            fillpoly(num_shade_points/2, shade_points);
        }
    } else {
        // Para Z negativo, sombrear el área a la izquierda
        for (int i = 0; i < num_points && num_shade_points < MAX_TABLE_SIZE*2 - 2; i++) {
            if (curve_points[i].x <= z_screen_x) {
                shade_points[num_shade_points++] = curve_points[i].x;
                shade_points[num_shade_points++] = curve_points[i].y;
            }
        }
        if (num_shade_points > 0) {
            shade_points[num_shade_points++] = z_screen_x;
            shade_points[num_shade_points++] = y0;
            shade_points[num_shade_points++] = x0;
            shade_points[num_shade_points++] = y0;
            fillpoly(num_shade_points/2, shade_points);
        }
    }
    
    // Dibujar la curva normal
    setcolor(APP_COLOR_ACCENT);
    for (int i = 0; i < num_points - 1; i++) {
        line(curve_points[i].x, curve_points[i].y, curve_points[i+1].x, curve_points[i+1].y);
    }
    
    // Dibujar línea vertical en z
    setcolor(BLUE);
    setlinestyle(DASHED_LINE, 0, NORM_WIDTH);
    line(z_screen_x, y0, z_screen_x, y0 - (int)(normal_pdf(z_value) * height / y_max));
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    
    // Etiquetar Z
    char z_label[20];
    sprintf(z_label, "Z=%.2f", z_value);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    setcolor(BLUE);
    outtextxy(z_screen_x, y0 - (int)(normal_pdf(z_value) * height / y_max) - 20, z_label);
}

// Función para dibujar la distribución T
void draw_t_distribution(int x0, int y0, int width, int height, int df, double alpha) {
    // Para la visualización, usaremos una aproximación de la distribución T
    // basada en la normal estándar para simplificar
    
    // Parámetros para el gráfico
    double x_min = -4.0;
    double x_max = 4.0;
    double y_max = normal_pdf(0); // Usamos el máximo de la normal para escalar
    
    // Puntos para la curva
    Point curve_points[MAX_TABLE_SIZE];
    int num_points = 0;
    
    // Crear los puntos de la curva (usamos normal como aproximación visual)
    for (int i = 0; i < width && num_points < MAX_TABLE_SIZE; i++) {
        double x = x_min + ((double)i / width) * (x_max - x_min);
        // En realidad T es más pesada en las colas que la normal
        // pero usamos normal para simplificar la visualización
        double y = normal_pdf(x);
        
        // Convertir a coordenadas de pantalla
        curve_points[num_points].x = x0 + i;
        curve_points[num_points].y = y0 - (int)(y * height / y_max);
        num_points++;
    }
    
    // Dibujar ejes
    setcolor(APP_COLOR_AXIS);
    line(x0, y0, x0 + width, y0);  // Eje X
    line(x0, y0, x0, y0 - height); // Eje Y
    
    // Dibujar marcas en el eje X
    for (double x = -3; x <= 3; x += 1) {
        int screen_x = x0 + (int)((x - x_min) / (x_max - x_min) * width);
        line(screen_x, y0, screen_x, y0 + 5);
        
        // Etiquetas con valores de x
        char label[10];
        sprintf(label, "%.0f", x);
        settextjustify(CENTER_TEXT, TOP_TEXT);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        outtextxy(screen_x, y0 + 7, label);
    }
    
    // Obtener el valor T para los grados de libertad y alpha dados
    double t_value = t_distribution_value(df, alpha);
    int t_screen_x = x0 + (int)((t_value - x_min) / (x_max - x_min) * width);
    
    // Dibujar área a la derecha de t (alpha)
    setfillstyle(SOLID_FILL, COLOR(200, 100, 100));
    
    // Puntos para el área sombreada
    int shade_points[MAX_TABLE_SIZE * 2];
    int num_shade_points = 0;
    
    // Sombrear área a la derecha
    for (int i = 0; i < num_points && num_shade_points < MAX_TABLE_SIZE*2 - 2; i++) {
        if (curve_points[i].x >= t_screen_x) {
            shade_points[num_shade_points++] = curve_points[i].x;
            shade_points[num_shade_points++] = curve_points[i].y;
        }
    }
    
    if (num_shade_points > 0) {
        shade_points[num_shade_points++] = x0 + width;
        shade_points[num_shade_points++] = y0;
        shade_points[num_shade_points++] = t_screen_x;
        shade_points[num_shade_points++] = y0;
        fillpoly(num_shade_points/2, shade_points);
    }
    
    // Dibujar la curva
    setcolor(APP_COLOR_ACCENT);
    for (int i = 0; i < num_points - 1; i++) {
        line(curve_points[i].x, curve_points[i].y, curve_points[i+1].x, curve_points[i+1].y);
    }
    
    // Dibujar línea vertical en t
    setcolor(BLUE);
    setlinestyle(DASHED_LINE, 0, NORM_WIDTH);
    line(t_screen_x, y0, t_screen_x, y0 - (int)(normal_pdf(t_value) * height / y_max));
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    
    // Etiquetar t
    char t_label[20];
    sprintf(t_label, "t=%.2f", t_value);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    setcolor(BLUE);
    outtextxy(t_screen_x, y0 - (int)(normal_pdf(t_value) * height / y_max) - 20, t_label);
}

// Mostrar información de la curva normal
void display_normal_curve_info(double z_value, double alpha) {
    // Mostrar resultados
    setcolor(APP_COLOR_SUBTITLE);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 130, (char*)"Resultados del calculo:");
    
    setcolor(BLACK);
    char result[100];
    
    // Para Z positivo, el área sombreada es a la derecha
    // Para Z negativo, el área sombreada es a la izquierda
    if (z_value >= 0) {
        sprintf(result, "P(Z > %.4f) = %.6f", z_value, alpha);
    } else {
        sprintf(result, "P(Z < %.4f) = %.6f", z_value, alpha);
    }
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 160, result);
    
    // Mostrar valor complementario (1-alpha)
    sprintf(result, "Valor complementario = %.6f", 1.0 - alpha);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 190, result);
}

// Mostrar información de la distribución T
void display_t_distribution_info(int df, double alpha, double t_value) {
    // Mostrar resultados
    setcolor(APP_COLOR_SUBTITLE);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 130, (char*)"Resultados del calculo:");
    
    setcolor(BLACK);
    char result[100];
    
    sprintf(result, "GL = %d, Alpha = %.4f", df, alpha);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 160, result);
    
    sprintf(result, "Valor critico t = %.6f", t_value);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 190, result);
    
    // Mensaje adicional para la interpretación
    setcolor(DARKGRAY);
    sprintf(result, "P(T > %.4f) = %.4f para v = %d grados de libertad", t_value, alpha, df);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 220, result);
}
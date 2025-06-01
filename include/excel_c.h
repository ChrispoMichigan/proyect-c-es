#ifndef EXCEL_C_H
#define EXCEL_C_H

#include <graphics.h>

// Constantes generales
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define MAX_DATA_POINTS 1000
#define MAX_FILENAME_LEN 256

// Constantes de colores (con nombres modificados para evitar conflictos)
#define APP_COLOR_BACKGROUND WHITE
#define APP_COLOR_TEXT BLACK
#define APP_COLOR_GRID LIGHTGRAY
#define APP_COLOR_HIGHLIGHT LIGHTBLUE
#define APP_COLOR_AXIS RED
#define APP_COLOR_DATA_POINT GREEN
#define APP_COLOR_HISTOGRAM_BAR CYAN

// Estructura de datos
typedef struct {
    double data[MAX_DATA_POINTS];
    int count;
    char source[MAX_FILENAME_LEN];
    int is_loaded;
} DataSet;

// Tipos de gráficos
typedef enum {
    GRAPH_NONE,
    GRAPH_STEM_LEAF,
    GRAPH_SCATTER,
    GRAPH_HISTOGRAM
} GraphType;

// Variable global para datos actuales
extern DataSet current_data;
extern GraphType current_graph;

// Funciones principales
void app_init();
void app_cleanup();
void main_loop();

// Funciones del menú
void draw_menu();
int handle_menu_click(int x, int y);

// Funciones de entrada de datos
void input_data_manually();
void load_data_from_file();
int file_dialog(char* filename);

// Funciones de visualización
void display_data_grid(DataSet* data);
void plot_stem_leaf(DataSet* data, int stem_digit, int leaf_digit);
void plot_scatter(DataSet* data);
void plot_histogram(DataSet* data, int num_classes);

// Utilidades
void clear_work_area();
double* sort_data(DataSet* data);
double find_min(DataSet* data);
double find_max(DataSet* data);
double find_mean(DataSet* data);

#endif // EXCEL_C_H
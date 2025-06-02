#ifndef EXCEL_C_H
#define EXCEL_C_H

#include <graphics.h>

// Incluir statistical.h después de nuestras definiciones
// para evitar conflictos

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

// Colores adicionales para la interfaz
#define APP_COLOR_TITLE COLOR(0, 100, 180)
#define APP_COLOR_SUBTITLE COLOR(80, 80, 80)
#define APP_COLOR_TEXT_DARK COLOR(40, 40, 40)
#define APP_COLOR_ACCENT COLOR(0, 120, 215)
#define APP_COLOR_BUTTON COLOR(70, 130, 180)
#define APP_COLOR_BUTTON_HOVER COLOR(100, 160, 210)

// Incluir statistical.h aquí para usar su definición de DataSet
#include "statistical.h"

// Estructura para botón interactivo
typedef struct {
    int x1, y1, x2, y2;
    char text[50];
    int is_hover;
} UIButton;

// Tipos de gráficos
typedef enum {
    GRAPH_NONE,
    GRAPH_STEM_LEAF,
    GRAPH_SCATTER,
    GRAPH_HISTOGRAM,
    GRAPH_REGRESSION
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
void displayDataGrid();
void plot_stem_leaf(DataSet* data, int stem_digit, int leaf_digit);
void plot_scatter(DataSet* data);
void plot_histogram(DataSet* data, int num_classes);
void plot_regression(DataSet* data);

// Utilidades
void clear_work_area();
double* sort_data(DataSet* data);
double find_min(DataSet* data);
double find_max(DataSet* data);
double find_mean(DataSet* data);

// Funciones para créditos y UI mejorada
void show_developer_profile();
void draw_ui_button(UIButton* btn);
int is_point_in_button(UIButton* btn, int x, int y);
void open_url(const char* url);

// Funciones auxiliares para visualización de datos
void drawDataGrid(int startX, int startY, int rows, int cols, int cellWidth, int cellHeight);
void displayDataInGrid(int startX, int startY, int rows, int cols, 
                       int cellWidth, int cellHeight, int page);
void drawNavigationControls(int currentPage, int totalPages, int gridStartX, int gridStartY, int cellHeight);

#endif // EXCEL_C_H
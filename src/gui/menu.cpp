#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include "../../include/excel_c.h"

// Estructura para elementos de menú
typedef struct {
    char title[50];
    int x1, y1, x2, y2;
    int is_active;
} MenuItem;

// Colores personalizados para un mejor aspecto
const int MENU_BG_COLOR = COLOR(60, 80, 120);
const int MENU_ITEM_COLOR = COLOR(80, 100, 140);
const int MENU_ITEM_ACTIVE = COLOR(100, 130, 180);
const int MENU_TEXT_COLOR = COLOR(240, 240, 240);
const int SUBMENU_BG_COLOR = COLOR(240, 240, 245);
const int SUBMENU_TEXT_COLOR = COLOR(40, 40, 40);
const int SUBMENU_HIGHLIGHT = COLOR(210, 225, 245);

// Menú principal - Índices claros para cada menú
#define MENU_DATOS 0
#define MENU_GRAFICOS 1
#define MENU_AYUDA 2

// Menú principal
MenuItem main_menu[] = {
    {"Datos", 90, 5, 160, 30, 0},     // Índice 0
    {"Graficos", 170, 5, 240, 30, 0}, // Índice 1 
    {"Ayuda", 250, 5, 320, 30, 0}     // Índice 2
};

// Submenú para Datos
MenuItem data_submenu[] = {
    {"Insertar Manualmente", 90, 35, 230, 60, 0},
    {"Cargar desde .txt", 90, 65, 230, 90, 0},
    {"Limpiar Datos", 90, 95, 230, 120, 0}
};

// Submenú para Gráficos
MenuItem graph_submenu[] = {
    {"Tallos y Hojas", 170, 35, 310, 60, 0},
    {"Grafica de Puntos", 170, 65, 310, 90, 0},
    {"Histograma", 170, 95, 310, 120, 0}
};

// Constantes para las acciones/opciones
#define ACTION_NONE -1
#define ACTION_DATA_MANUAL 100
#define ACTION_DATA_LOAD 101
#define ACTION_DATA_CLEAR 102
#define ACTION_GRAPH_STEMLEAF 200
#define ACTION_GRAPH_SCATTER 201
#define ACTION_GRAPH_HISTOGRAM 202

// Variables de control de menú globales
int active_menu = -1;           // Cuál menú principal está activo (-1 = ninguno)
int current_submenu_open = 0;   // Flag para indicar si hay algún submenú abierto

// Dibuja la barra de menú principal sin submenús
void draw_main_menu_bar() {
    // Área de fondo del menú
    setfillstyle(SOLID_FILL, MENU_BG_COLOR);
    bar(0, 0, WINDOW_WIDTH, 35);
    setcolor(DARKGRAY);
    line(0, 35, WINDOW_WIDTH, 35);
    
    // Dibujar elementos de menú principal
    setcolor(MENU_TEXT_COLOR);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    
    for (int i = 0; i < sizeof(main_menu) / sizeof(MenuItem); i++) {
        // Configurar color según estado (activo/inactivo)
        if (main_menu[i].is_active) {
            setfillstyle(SOLID_FILL, MENU_ITEM_ACTIVE);
        } else {
            setfillstyle(SOLID_FILL, MENU_BG_COLOR);
        }
        
        // Dibujar fondo del elemento de menú
        bar(main_menu[i].x1, main_menu[i].y1, 
            main_menu[i].x2, main_menu[i].y2);
            
        // Borde suave
        setcolor(main_menu[i].is_active ? WHITE : MENU_ITEM_COLOR);
        rectangle(main_menu[i].x1, main_menu[i].y1, 
                 main_menu[i].x2, main_menu[i].y2);
        
        // Texto del menú
        setcolor(MENU_TEXT_COLOR);         
        outtextxy(main_menu[i].x1 + 10, 
                (main_menu[i].y1 + main_menu[i].y2) / 2, 
                main_menu[i].title);
    }
}

// Dibuja un submenú específico
void draw_submenu(MenuItem* submenu, int submenu_size) {
    // Sombra del submenú para efecto 3D
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(submenu[0].x1 + 4, submenu[0].y1 + 4, 
        submenu[submenu_size-1].x2 + 4, submenu[submenu_size-1].y2 + 4);
        
    // Fondo del submenú
    setfillstyle(SOLID_FILL, SUBMENU_BG_COLOR);
    bar(submenu[0].x1, submenu[0].y1, 
        submenu[submenu_size-1].x2, submenu[submenu_size-1].y2);
        
    setcolor(DARKGRAY);
    rectangle(submenu[0].x1, submenu[0].y1, 
             submenu[submenu_size-1].x2, submenu[submenu_size-1].y2);
    
    // Dibujar elementos del submenú
    for (int i = 0; i < submenu_size; i++) {
        if (submenu[i].is_active) {
            setfillstyle(SOLID_FILL, SUBMENU_HIGHLIGHT);
            bar(submenu[i].x1, submenu[i].y1, 
                submenu[i].x2, submenu[i].y2);
        }
        
        setcolor(SUBMENU_TEXT_COLOR);
        outtextxy(submenu[i].x1 + 10, 
                (submenu[i].y1 + submenu[i].y2) / 2, 
                submenu[i].title);
                
        // Línea separadora sutil entre opciones
        if (i < submenu_size - 1) {
            setcolor(LIGHTGRAY);
            line(submenu[i].x1 + 5, submenu[i].y2, 
                 submenu[i].x2 - 5, submenu[i].y2);
        }
    }
}

// Cierra todos los menús
void close_all_menus() {
    // Desactivar todos los menús principales
    for (int i = 0; i < sizeof(main_menu)/sizeof(MenuItem); i++) {
        main_menu[i].is_active = 0;
    }
    
    // Reiniciar variables de control
    active_menu = -1;
    current_submenu_open = 0;
}

// Dibuja todo el sistema de menús
void draw_menu() {
    // Calculamos la altura máxima de los submenús
    int max_height = 0;
    
    if (active_menu == MENU_DATOS) {
        // Encontrar la coordenada y2 del último elemento del submenú de datos
        max_height = data_submenu[sizeof(data_submenu)/sizeof(MenuItem) - 1].y2;
    } else if (active_menu == MENU_GRAFICOS) {
        // Encontrar la coordenada y2 del último elemento del submenú de gráficos
        max_height = graph_submenu[sizeof(graph_submenu)/sizeof(MenuItem) - 1].y2;
    } else {
        // Solo limpiar la barra de menú principal si no hay submenús
        max_height = 35;
    }
    
    // Añadir un pequeño margen de 5 píxeles para seguridad
    max_height += 5;
    
    // Limpia solo el área necesaria
    setfillstyle(SOLID_FILL, WHITE);
    bar(0, 0, WINDOW_WIDTH, max_height);
    
    // Dibuja la barra de menú principal
    draw_main_menu_bar();
    
    // Dibujar submenú correspondiente solo si hay un menú activo
    if (active_menu == MENU_DATOS && current_submenu_open) {
        draw_submenu(data_submenu, sizeof(data_submenu) / sizeof(MenuItem));
    } 
    else if (active_menu == MENU_GRAFICOS && current_submenu_open) {
        draw_submenu(graph_submenu, sizeof(graph_submenu) / sizeof(MenuItem));
    }
    // No hay submenú para Ayuda
}

// Maneja clicks en el menú
int handle_menu_click(int x, int y) {
    int action = 0;
    
    // Verificar si se hizo clic en el menú principal
    if (y < 35) {
        // Comprobar cada elemento del menú principal
        for (int i = 0; i < sizeof(main_menu) / sizeof(MenuItem); i++) {
            if (x >= main_menu[i].x1 && x <= main_menu[i].x2 &&
                y >= main_menu[i].y1 && y <= main_menu[i].y2) {
                
                // Si este menú ya está activo, cerrarlo
                if (active_menu == i && current_submenu_open) {
                    close_all_menus();
                } else {
                    // Cerrar cualquier menú abierto primero
                    close_all_menus();
                    
                    // Activar este menú
                    main_menu[i].is_active = 1;
                    active_menu = i;
                    current_submenu_open = 1;
                }
                
                draw_menu(); // Redibujar inmediatamente
                return 1;
            }
        }
        
        // Si se hizo clic en la barra de menú pero no en un elemento específico
        close_all_menus();
        draw_menu();
        return 0;
    } 
    else if (current_submenu_open) {
        // Verificar clicks en submenús según el menú activo
        if (active_menu == MENU_DATOS) {
            // Submenú de datos
            for (int i = 0; i < sizeof(data_submenu) / sizeof(MenuItem); i++) {
                if (x >= data_submenu[i].x1 && x <= data_submenu[i].x2 &&
                    y >= data_submenu[i].y1 && y <= data_submenu[i].y2) {
                    
                    // Cerrar todos los menús
                    close_all_menus();
                    draw_menu(); // Actualizar interfaz
                    
                    // Limpiar área de trabajo
                    clear_work_area();
                    
                    // Ejecutar la acción correspondiente
                    switch (i) {
                        case 0: // Insertar manualmente
                            input_data_manually();
                            return ACTION_DATA_MANUAL;
                            
                        case 1: // Cargar desde txt
                            load_data_from_file();
                            return ACTION_DATA_LOAD;
                            
                        case 2: // Limpiar datos
                            current_data.count = 0;
                            current_data.is_loaded = 0;
                            strcpy(current_data.source, "");
                            
                            setcolor(BLACK);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                            settextjustify(CENTER_TEXT, CENTER_TEXT);
                            outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, (char*)"Datos eliminados correctamente");
                            delay(1500);
                            clear_work_area();
                            return ACTION_DATA_CLEAR;
                    }
                }
            }
        } 
        else if (active_menu == MENU_GRAFICOS) {
            // Submenú de gráficos
            for (int i = 0; i < sizeof(graph_submenu) / sizeof(MenuItem); i++) {
                if (x >= graph_submenu[i].x1 && x <= graph_submenu[i].x2 &&
                    y >= graph_submenu[i].y1 && y <= graph_submenu[i].y2) {
                    
                    // Cerrar todos los menús
                    close_all_menus();
                    draw_menu(); // Actualizar interfaz
                    
                    // Limpiar área de trabajo
                    clear_work_area();
                    
                    // Verificar si hay datos disponibles
                    if (!current_data.is_loaded || current_data.count == 0) {
                        setcolor(RED);
                        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, (char*)"No hay datos para visualizar");
                        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 40, (char*)"Cargue datos primero");
                        delay(2000);
                        clear_work_area();
                        return 0;
                    }
                    
                    // Ejecutar la visualización correspondiente
                    switch (i) {
                        case 0: // Tallos y hojas
                            plot_stem_leaf(&current_data, 1, 0);
                            return ACTION_GRAPH_STEMLEAF;
                            
                        case 1: // Gráfica de puntos
                            plot_scatter(&current_data);
                            return ACTION_GRAPH_SCATTER;
                            
                        case 2: // Histograma
                            plot_histogram(&current_data, 5);
                            return ACTION_GRAPH_HISTOGRAM;
                    }
                }
            }
        }
    }
    
    // Si se hizo clic fuera de los menús activos
    if (y > 150 || x < 90 || x > 310) {
        close_all_menus();
        draw_menu();
    }
    
    return 0;
}
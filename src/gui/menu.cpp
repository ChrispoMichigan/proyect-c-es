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

// Menú principal
MenuItem main_menu[] = {
    {"Archivo", 10, 5, 80, 30, 0},
    {"Datos", 90, 5, 160, 30, 0},
    {"Graficos", 170, 5, 240, 30, 0},
    {"Ayuda", 250, 5, 320, 30, 0}
};

// Submenú para Archivo
MenuItem file_submenu[] = {
    {"Nuevo", 10, 35, 120, 60, 0},
    {"Abrir...", 10, 65, 120, 90, 0},
    {"Guardar", 10, 95, 120, 120, 0},
    {"Salir", 10, 125, 120, 150, 0}
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

// Variables de estado de menús
int active_menu = -1;

// Dibuja el menú principal
void draw_menu() {
    int i;
    
    // Área de fondo del menú
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, 0, WINDOW_WIDTH, 35);
    setcolor(BLACK);
    line(0, 35, WINDOW_WIDTH, 35);
    
    // Dibujar elementos de menú principal
    setcolor(BLACK);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    
    for (i = 0; i < sizeof(main_menu) / sizeof(MenuItem); i++) {
        if (main_menu[i].is_active) {
            setfillstyle(SOLID_FILL, WHITE);
            bar(main_menu[i].x1, main_menu[i].y1, 
                main_menu[i].x2, main_menu[i].y2);
        }
        
        outtextxy(main_menu[i].x1 + 10, 
                (main_menu[i].y1 + main_menu[i].y2) / 2, 
                main_menu[i].title);
    }
    
    // Dibujar submenús si están activos
    if (active_menu >= 0) {
        MenuItem *submenu = NULL;
        int submenu_size = 0;
        
        // Seleccionar el submenú correspondiente
        if (active_menu == 0) {
            submenu = file_submenu;
            submenu_size = sizeof(file_submenu) / sizeof(MenuItem);
        } else if (active_menu == 1) {
            submenu = data_submenu;
            submenu_size = sizeof(data_submenu) / sizeof(MenuItem);
        } else if (active_menu == 2) {
            submenu = graph_submenu;
            submenu_size = sizeof(graph_submenu) / sizeof(MenuItem);
        }
        
        if (submenu) {
            // Fondo del submenú
            setfillstyle(SOLID_FILL, WHITE);
            bar(submenu[0].x1, submenu[0].y1, 
                submenu[submenu_size-1].x2, submenu[submenu_size-1].y2);
                
            setcolor(BLACK);
            rectangle(submenu[0].x1, submenu[0].y1, 
                     submenu[submenu_size-1].x2, submenu[submenu_size-1].y2);
            
            // Dibujar elementos del submenú
            for (i = 0; i < submenu_size; i++) {
                if (submenu[i].is_active) {
                    setfillstyle(SOLID_FILL, LIGHTGRAY);
                    bar(submenu[i].x1, submenu[i].y1, 
                        submenu[i].x2, submenu[i].y2);
                }
                
                outtextxy(submenu[i].x1 + 10, 
                        (submenu[i].y1 + submenu[i].y2) / 2, 
                        submenu[i].title);
            }
        }
    }
}

// Maneja clicks en el menú
int handle_menu_click(int x, int y) {
    int i;
    
    // Verificar si se hizo clic en el menú principal
    if (y < 35) {
        for (i = 0; i < sizeof(main_menu) / sizeof(MenuItem); i++) {
            if (x >= main_menu[i].x1 && x <= main_menu[i].x2 &&
                y >= main_menu[i].y1 && y <= main_menu[i].y2) {
                
                // Si ya está activo, desactivar
                if (active_menu == i) {
                    main_menu[i].is_active = 0;
                    active_menu = -1;
                } else {
                    // Desactivar cualquier otro menú activo
                    if (active_menu >= 0) {
                        main_menu[active_menu].is_active = 0;
                    }
                    
                    // Activar este menú
                    main_menu[i].is_active = 1;
                    active_menu = i;
                }
                
                return 1;
            }
        }
    } else if (active_menu >= 0) {
        // Verificar clicks en submenús
        MenuItem *submenu = NULL;
        int submenu_size = 0;
        int action = -1;
        
        // Seleccionar el submenú correspondiente
        if (active_menu == 0) {
            submenu = file_submenu;
            submenu_size = sizeof(file_submenu) / sizeof(MenuItem);
        } else if (active_menu == 1) {
            submenu = data_submenu;
            submenu_size = sizeof(data_submenu) / sizeof(MenuItem);
        } else if (active_menu == 2) {
            submenu = graph_submenu;
            submenu_size = sizeof(graph_submenu) / sizeof(MenuItem);
        }
        
        if (submenu) {
            for (i = 0; i < submenu_size; i++) {
                if (x >= submenu[i].x1 && x <= submenu[i].x2 &&
                    y >= submenu[i].y1 && y <= submenu[i].y2) {
                    
                    // Calcular código de acción basado en menú y elemento
                    action = (active_menu + 1) * 100 + i;
                    
                    // Desactivar menús
                    main_menu[active_menu].is_active = 0;
                    active_menu = -1;
                    
                    // Ejecutar acción correspondiente
                    switch (action) {
                        case 101: // Abrir archivo
                            load_data_from_file();
                            break;
                        case 103: // Salir
                            return -1; // Código para salir
                        case 200: // Insertar datos manualmente
                            input_data_manually();
                            break;
                        case 201: // Cargar desde txt
                            load_data_from_file();
                            break;
                        case 300: // Tallos y hojas
                            // Configurar para tallo=decenas, hoja=unidades
                            plot_stem_leaf(&current_data, 1, 0);
                            break;
                        case 301: // Gráfica de puntos
                            plot_scatter(&current_data);
                            break;
                        case 302: // Histograma
                            plot_histogram(&current_data, 5); // 5 clases por defecto
                            break;
                    }
                    
                    return action;
                }
            }
        }
    }
    
    // Si se hizo clic fuera de los menús, cerrarlos
    if (active_menu >= 0) {
        main_menu[active_menu].is_active = 0;
        active_menu = -1;
        return 1;
    }
    
    return 0;
}
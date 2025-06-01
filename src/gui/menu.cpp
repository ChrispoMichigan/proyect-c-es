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

// Menú principal
MenuItem main_menu[] = {
    {"Datos", 90, 5, 160, 30, 0},
    {"Graficos", 170, 5, 240, 30, 0},
    {"Ayuda", 250, 5, 320, 30, 0}
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
    setfillstyle(SOLID_FILL, MENU_BG_COLOR);
    bar(0, 0, WINDOW_WIDTH, 35);
    setcolor(DARKGRAY);
    line(0, 35, WINDOW_WIDTH, 35);
    
    // Dibujar elementos de menú principal
    setcolor(MENU_TEXT_COLOR);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    
    for (i = 0; i < sizeof(main_menu) / sizeof(MenuItem); i++) {
        if (main_menu[i].is_active) {
            setfillstyle(SOLID_FILL, MENU_ITEM_ACTIVE);
        } else {
            setfillstyle(SOLID_FILL, MENU_BG_COLOR);
        }
        
        bar(main_menu[i].x1, main_menu[i].y1, 
            main_menu[i].x2, main_menu[i].y2);
            
        // Borde suave
        setcolor(main_menu[i].is_active ? WHITE : MENU_ITEM_COLOR);
        rectangle(main_menu[i].x1, main_menu[i].y1, 
                 main_menu[i].x2, main_menu[i].y2);
        
        // Texto
        setcolor(MENU_TEXT_COLOR);         
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
            submenu = data_submenu;
            submenu_size = sizeof(data_submenu) / sizeof(MenuItem);
        } else if (active_menu == 1) {
            submenu = graph_submenu;
            submenu_size = sizeof(graph_submenu) / sizeof(MenuItem);
        }
        
        if (submenu) {
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
            for (i = 0; i < submenu_size; i++) {
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
            submenu = data_submenu;
            submenu_size = sizeof(data_submenu) / sizeof(MenuItem);
        } else if (active_menu == 1) {
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
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../include/excel_c.h"
#include "../../include/statistical.h"
#include "../../include/tables.h"

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
const int MENU_TEXT_COLOR = COLOR(0, 0, 0);
const int SUBMENU_BG_COLOR = COLOR(240, 240, 245);
const int SUBMENU_TEXT_COLOR = COLOR(40, 40, 40);
const int SUBMENU_HIGHLIGHT = COLOR(210, 225, 245);

// Menú principal - Índices claros para cada menú
#define MENU_DATOS 0
#define MENU_GRAFICOS 1
#define MENU_ESTADISTICOS 2
#define MENU_TABLAS 3
#define MENU_CREDITOS 4

// Menú principal (modificado con nuevas opciones)
MenuItem main_menu[] = {
    {"Datos", 50, 5, 130, 30, 0},          // Índice 0
    {"Graficos", 140, 5, 220, 30, 0},      // Índice 1 
    {"Estadisticos", 230, 5, 330, 30, 0},  // Índice 2 (Nuevo)
    {"Tablas", 340, 5, 420, 30, 0},        // Índice 3 (Nuevo)
    {"Creditos", 430, 5, 510, 30, 0}       // Índice 4 (Nuevo)
};

// Submenú para Datos
MenuItem data_submenu[] = {
    {"Insertar Manualmente", 50, 35, 190, 60, 0},
    {"Cargar desde .txt", 50, 65, 190, 90, 0},
    {"Limpiar Datos", 50, 95, 190, 120, 0}
};

// Submenú para Gráficos (con Regresión Lineal añadida)
MenuItem graph_submenu[] = {
    {"Tallos y Hojas", 140, 35, 280, 60, 0},
    {"Grafica de Puntos", 140, 65, 280, 90, 0},
    {"Histograma", 140, 95, 280, 120, 0},
    {"Regresion Lineal", 140, 125, 280, 150, 0} // Nueva opción
};

// Submenú para Estadísticos (NUEVO)
MenuItem stats_submenu[] = {
    {"Calcular", 230, 35, 370, 60, 0}
};

// Submenú para opciones de cálculo (submenu anidado) (NUEVO)
MenuItem calc_submenu[] = {
    {"Media", 370, 35, 510, 60, 0},
    {"Mediana", 370, 65, 510, 90, 0},
    {"Moda", 370, 95, 510, 120, 0},
    {"Media recortada", 370, 125, 510, 150, 0},
    {"Varianza (muestra)", 370, 155, 510, 180, 0},
    {"Varianza (poblacion)", 370, 185, 510, 210, 0},
    {"Desv. Est. (muestra)", 370, 215, 510, 240, 0},
    {"Desv. Est. (poblacion)", 370, 245, 510, 270, 0}
};

// Submenú para Tablas (NUEVO)
MenuItem tables_submenu[] = {
    {"Calcular Alpha (tabla Z)", 340, 35, 510, 60, 0},
    {"Encontrar Z dado Alpha", 340, 65, 510, 90, 0},
    {"Buscar en tabla T", 340, 95, 510, 120, 0},
    {"Calcular GL y Alpha de T", 340, 125, 510, 150, 0}
};

// Submenú para Créditos (NUEVO)
MenuItem credits_submenu[] = {
    {"Desarrollador", 430, 35, 570, 60, 0}
};

// Constantes para las acciones/opciones
#define ACTION_NONE -1
#define ACTION_DATA_MANUAL 100
#define ACTION_DATA_LOAD 101
#define ACTION_DATA_CLEAR 102
#define ACTION_GRAPH_STEMLEAF 200
#define ACTION_GRAPH_SCATTER 201
#define ACTION_GRAPH_HISTOGRAM 202
#define ACTION_GRAPH_REGRESSION 203 // Nueva acción para regresión lineal

// Nuevas constantes para acciones de estadísticos
#define ACTION_STATS_MEAN 300
#define ACTION_STATS_MEDIAN 301
#define ACTION_STATS_MODE 302
#define ACTION_STATS_TRIMMED_MEAN 303
#define ACTION_STATS_VAR_SAMPLE 304
#define ACTION_STATS_VAR_POP 305
#define ACTION_STATS_STDEV_SAMPLE 306
#define ACTION_STATS_STDEV_POP 307

// Nuevas constantes para acciones de tablas
#define ACTION_TABLES_ALPHA_Z 400
#define ACTION_TABLES_Z_ALPHA 401
#define ACTION_TABLES_FIND_T 402
#define ACTION_TABLES_T_PARAMS 403

// Nueva constante para acción de créditos
#define ACTION_CREDITS_DEV 500

// Variables de control de menú globales
int active_menu = -1;           // Cuál menú principal está activo (-1 = ninguno)
int current_submenu_open = 0;   // Flag para indicar si hay algún submenú abierto
int calc_submenu_open = 0;      // Flag para submenu anidado de cálculos (NUEVO)

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
    calc_submenu_open = 0;  // Nueva variable para submenu de cálculo
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
    } else if (active_menu == MENU_ESTADISTICOS) {
        // Verificar si el submenú anidado está abierto
        if (calc_submenu_open) {
            max_height = calc_submenu[sizeof(calc_submenu)/sizeof(MenuItem) - 1].y2;
        } else {
            max_height = stats_submenu[sizeof(stats_submenu)/sizeof(MenuItem) - 1].y2;
        }
    } else if (active_menu == MENU_TABLAS) {
        max_height = tables_submenu[sizeof(tables_submenu)/sizeof(MenuItem) - 1].y2;
    } else if (active_menu == MENU_CREDITOS) {
        max_height = credits_submenu[sizeof(credits_submenu)/sizeof(MenuItem) - 1].y2;
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
    else if (active_menu == MENU_ESTADISTICOS && current_submenu_open) {
        draw_submenu(stats_submenu, sizeof(stats_submenu) / sizeof(MenuItem));
        
        // Si además está activo el submenú de cálculos
        if (calc_submenu_open) {
            draw_submenu(calc_submenu, sizeof(calc_submenu) / sizeof(MenuItem));
        }
    }
    else if (active_menu == MENU_TABLAS && current_submenu_open) {
        draw_submenu(tables_submenu, sizeof(tables_submenu) / sizeof(MenuItem));
    }
    else if (active_menu == MENU_CREDITOS && current_submenu_open) {
        draw_submenu(credits_submenu, sizeof(credits_submenu) / sizeof(MenuItem));
    }
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
                            
                        case 3: //Regresion Lineal
                            clear_work_area();
    
                            // Verificar si hay datos cargados
                            if (!current_data.is_loaded || current_data.count == 0) {
                                setcolor(RED);
                                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                                settextjustify(CENTER_TEXT, CENTER_TEXT);
                                outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 20, (char*)"No hay datos para visualizar");
                                outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 20, (char*)"Cargue datos primero");
                                delay(2000);
                                clear_work_area();
                                return 0;
                            }
                            
                            // Verificar que el número de datos sea par
                            if (current_data.count % 2 != 0) {
                                setcolor(RED);
                                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                                settextjustify(CENTER_TEXT, CENTER_TEXT);
                                outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 40, (char*)"El numero de datos debe ser par");
                                outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, (char*)"Para regresion lineal se usara:");
                                outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 40, (char*)"Primera mitad de datos = X");
                                outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 80, (char*)"Segunda mitad de datos = Y");
                                
                                // Botón para continuar o cancelar
                                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                                setcolor(BLUE);
                                outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT - 60, (char*)"Cargue un numero par de datos e intente nuevamente");
                                delay(3000);
                                clear_work_area();
                                return 0;
                            }
                            
                            // Mostrar información sobre el método
                            setcolor(BLUE);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                            settextjustify(CENTER_TEXT, TOP_TEXT);
                            outtextxy(WINDOW_WIDTH/2, 50, (char*)"Regresion Lineal");
                            
                            setcolor(BLACK);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                            settextjustify(CENTER_TEXT, TOP_TEXT);
                            outtextxy(WINDOW_WIDTH/2, 100, (char*)"El calculo se realizara de la siguiente manera:");
                            outtextxy(WINDOW_WIDTH/2, 130, (char*)"Primera mitad de datos = valores X");
                            outtextxy(WINDOW_WIDTH/2, 160, (char*)"Segunda mitad de datos = valores Y");
                            
                            // Mostrar ejemplo
                            settextjustify(LEFT_TEXT, TOP_TEXT);
                            int exampleX = WINDOW_WIDTH/2 - 100;
                            outtextxy(exampleX, 200, (char*)"Ejemplo:");
                            outtextxy(exampleX, 230, (char*)"X    | Y");
                            outtextxy(exampleX, 250, (char*)"dato1 | dato(n/2+1)");
                            outtextxy(exampleX, 270, (char*)"dato2 | dato(n/2+2)");
                            outtextxy(exampleX, 290, (char*)"...   | ...");
                            
                            // Botones
                            int btnWidth = 120;
                            int btnHeight = 40;
                            
                            // Botón Continuar
                            setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                            bar(WINDOW_WIDTH/2 - btnWidth - 20, WINDOW_HEIGHT - 80, WINDOW_WIDTH/2 - 20, WINDOW_HEIGHT - 80 + btnHeight);
                            setcolor(WHITE);
                            rectangle(WINDOW_WIDTH/2 - btnWidth - 20, WINDOW_HEIGHT - 80, WINDOW_WIDTH/2 - 20, WINDOW_HEIGHT - 80 + btnHeight);
                            settextjustify(CENTER_TEXT, CENTER_TEXT);
                            outtextxy(WINDOW_WIDTH/2 - btnWidth/2 - 20, WINDOW_HEIGHT - 80 + btnHeight/2, (char*)"Continuar");
                            
                            // Botón Cancelar
                            setfillstyle(SOLID_FILL, COLOR(200, 100, 100));
                            bar(WINDOW_WIDTH/2 + 20, WINDOW_HEIGHT - 80, WINDOW_WIDTH/2 + btnWidth + 20, WINDOW_HEIGHT - 80 + btnHeight);
                            setcolor(WHITE);
                            rectangle(WINDOW_WIDTH/2 + 20, WINDOW_HEIGHT - 80, WINDOW_WIDTH/2 + btnWidth + 20, WINDOW_HEIGHT - 80 + btnHeight);
                            outtextxy(WINDOW_WIDTH/2 + btnWidth/2 + 20, WINDOW_HEIGHT - 80 + btnHeight/2, (char*)"Cancelar");
                            
                            // Esperar clic y procesar selección
                            while (1) {
                                if (ismouseclick(WM_LBUTTONDOWN)) {
                                    int x, y;
                                    getmouseclick(WM_LBUTTONDOWN, x, y);
                                    
                                    // Continuar
                                    if (x >= WINDOW_WIDTH/2 - btnWidth - 20 && x <= WINDOW_WIDTH/2 - 20 && 
                                        y >= WINDOW_HEIGHT - 80 && y <= WINDOW_HEIGHT - 80 + btnHeight) {
                                        plot_regression(&current_data);
                                        return ACTION_GRAPH_REGRESSION;
                                    }
                                    // Cancelar
                                    else if (x >= WINDOW_WIDTH/2 + 20 && x <= WINDOW_WIDTH/2 + btnWidth + 20 && 
                                            y >= WINDOW_HEIGHT - 80 && y <= WINDOW_HEIGHT - 80 + btnHeight) {
                                        clear_work_area();
                                        return 0;
                                    }
                                }
                                
                                delay(50);
                            }
                            return ACTION_GRAPH_REGRESSION;     
                    }
                }
            }
        }
        else if (active_menu == MENU_ESTADISTICOS) {
            // Verificar clicks en el submenú de estadísticos
            for (int i = 0; i < sizeof(stats_submenu) / sizeof(MenuItem); i++) {
                if (x >= stats_submenu[i].x1 && x <= stats_submenu[i].x2 &&
                    y >= stats_submenu[i].y1 && y <= stats_submenu[i].y2) {
                    
                    // Activar submenú anidado de cálculos
                    if (i == 0) {  // "Calcular"
                        calc_submenu_open = 1;
                        draw_menu();
                        return 1;
                    }
                }
            }
            
            // Verificar clicks en el submenú de cálculos si está abierto
            if (calc_submenu_open) {
                for (int i = 0; i < sizeof(calc_submenu) / sizeof(MenuItem); i++) {
                    if (x >= calc_submenu[i].x1 && x <= calc_submenu[i].x2 &&
                        y >= calc_submenu[i].y1 && y <= calc_submenu[i].y2) {
                        
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
                            outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, (char*)"No hay datos para calcular estadísticos");
                            outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 40, (char*)"Cargue datos primero");
                            delay(2000);
                            clear_work_area();
                            return 0;
                        }
                        
                        // Ejecutar la función correspondiente
                        switch (i) {
                            case 0: // Media
                                {
                                    StatResult result = calc_mean(&current_data);
                                    setcolor(APP_COLOR_TITLE);
                                    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                                    settextjustify(CENTER_TEXT, TOP_TEXT);
                                    outtextxy(WINDOW_WIDTH/2, 50, (char*)"Resultado - Media");
                                    display_stat_result(result, 100);
                                    
                                    // Botón para volver
                                    int btnWidth = 150;
                                    int btnHeight = 40;
                                    int btnX = (WINDOW_WIDTH - btnWidth) / 2;
                                    int btnY = 200;
                                    
                                    setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                                    bar(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
                                    setcolor(WHITE);
                                    rectangle(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
                                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                                    outtextxy(btnX + btnWidth/2, btnY + btnHeight/2, (char*)"Volver");
                                    
                                    // Esperar clic para continuar
                                    while (!ismouseclick(WM_LBUTTONDOWN)) {
                                        delay(50);
                                    }
                                    clearmouseclick(WM_LBUTTONDOWN);
                                }
                                return ACTION_STATS_MEAN;
                                
                            case 1: // Mediana
                                {
                                    StatResult result = calc_median(&current_data);
                                    setcolor(APP_COLOR_TITLE);
                                    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                                    settextjustify(CENTER_TEXT, TOP_TEXT);
                                    outtextxy(WINDOW_WIDTH/2, 50, (char*)"Resultado - Mediana");
                                    display_stat_result(result, 100);
                                    
                                    // Botón para volver
                                    int btnWidth = 150;
                                    int btnHeight = 40;
                                    int btnX = (WINDOW_WIDTH - btnWidth) / 2;
                                    int btnY = 200;
                                    
                                    setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                                    bar(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
                                    setcolor(WHITE);
                                    rectangle(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
                                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                                    outtextxy(btnX + btnWidth/2, btnY + btnHeight/2, (char*)"Volver");
                                    
                                    // Esperar clic para continuar
                                    while (!ismouseclick(WM_LBUTTONDOWN)) {
                                        delay(50);
                                    }
                                    clearmouseclick(WM_LBUTTONDOWN);
                                }
                                return ACTION_STATS_MEDIAN;
                                
                            case 2: // Moda
                                {
                                    StatResult result = calc_mode(&current_data);
                                    setcolor(APP_COLOR_TITLE);
                                    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                                    settextjustify(CENTER_TEXT, TOP_TEXT);
                                    outtextxy(WINDOW_WIDTH/2, 50, (char*)"Resultado - Moda");
                                    display_stat_result(result, 100);
                                    
                                    // Botón para volver
                                    int btnWidth = 150;
                                    int btnHeight = 40;
                                    int btnX = (WINDOW_WIDTH - btnWidth) / 2;
                                    int btnY = 200;
                                    
                                    setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                                    bar(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
                                    setcolor(WHITE);
                                    rectangle(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
                                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                                    outtextxy(btnX + btnWidth/2, btnY + btnHeight/2, (char*)"Volver");
                                    
                                    // Esperar clic para continuar
                                    while (!ismouseclick(WM_LBUTTONDOWN)) {
                                        delay(50);
                                    }
                                    clearmouseclick(WM_LBUTTONDOWN);
                                }
                                return ACTION_STATS_MODE;
                                
                            case 3: // Media recortada
                                show_trimmed_mean_options();
                                return ACTION_STATS_TRIMMED_MEAN;
                                
                            case 4: // Varianza (muestra)
                            case 5: // Varianza (población)
                            case 6: // Desv. Est. (muestra)
                            case 7: // Desv. Est. (población)
                                show_dispersion_stats();
                                return i == 4 ? ACTION_STATS_VAR_SAMPLE : 
                                    i == 5 ? ACTION_STATS_VAR_POP : 
                                    i == 6 ? ACTION_STATS_STDEV_SAMPLE : ACTION_STATS_STDEV_POP;
                        }
                    }
                }
            }
        }
        else if (active_menu == MENU_TABLAS) {
            // Verificar clicks en el submenú de tablas
            for (int i = 0; i < sizeof(tables_submenu) / sizeof(MenuItem); i++) {
                if (x >= tables_submenu[i].x1 && x <= tables_submenu[i].x2 &&
                    y >= tables_submenu[i].y1 && y <= tables_submenu[i].y2) {
                    
                    // Cerrar todos los menús
                    close_all_menus();
                    draw_menu(); // Actualizar interfaz
                    
                    // Limpiar área de trabajo
                    clear_work_area();
                    
                    // Ejecutar la función correspondiente
                    switch (i) {
                        case 0: // Calcular Alpha (tabla Z)
                        {
                            // Interfaz para solicitar el valor de Z
                            setcolor(APP_COLOR_TITLE);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                            settextjustify(CENTER_TEXT, TOP_TEXT);
                            outtextxy(WINDOW_WIDTH/2, 50, (char*)"Calcular Alpha (tabla Z)");
                            
                            setcolor(BLACK);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                            outtextxy(WINDOW_WIDTH/2, 100, (char*)"Introduzca el valor de Z:");
                            
                            // Campo de entrada para Z
                            char z_value_str[20] = "0.00";
                            double z_value = 0.0;
                            
                            // Dibujar campo de entrada
                            int input_width = 100;
                            int input_height = 30;
                            int input_x = WINDOW_WIDTH/2 - input_width/2;
                            int input_y = 130;
                            
                            setfillstyle(SOLID_FILL, WHITE);
                            bar(input_x, input_y, input_x + input_width, input_y + input_height);
                            rectangle(input_x, input_y, input_x + input_width, input_y + input_height);
                            
                            settextjustify(CENTER_TEXT, CENTER_TEXT);
                            outtextxy(input_x + input_width/2, input_y + input_height/2, z_value_str);
                            
                            // Botones + y -
                            int btn_width = 30;
                            int btn_height = 30;
                            
                            setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                            bar(input_x - btn_width - 10, input_y, input_x - 10, input_y + btn_height);
                            rectangle(input_x - btn_width - 10, input_y, input_x - 10, input_y + btn_height);
                            outtextxy(input_x - btn_width/2 - 10, input_y + btn_height/2, (char*)"-");
                            
                            bar(input_x + input_width + 10, input_y, input_x + input_width + btn_width + 10, input_y + btn_height);
                            rectangle(input_x + input_width + 10, input_y, input_x + input_width + btn_width + 10, input_y + btn_height);
                            outtextxy(input_x + input_width + btn_width/2 + 10, input_y + btn_height/2, (char*)"+");
                            
                            // Botón calcular
                            int calc_btn_width = 120;
                            int calc_btn_height = 40;
                            int calc_btn_x = WINDOW_WIDTH/2 - calc_btn_width/2;
                            int calc_btn_y = 180;
                            
                            setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                            bar(calc_btn_x, calc_btn_y, calc_btn_x + calc_btn_width, calc_btn_y + calc_btn_height);
                            rectangle(calc_btn_x, calc_btn_y, calc_btn_x + calc_btn_width, calc_btn_y + calc_btn_height);
                            outtextxy(calc_btn_x + calc_btn_width/2, calc_btn_y + calc_btn_height/2, (char*)"Calcular");
                            
                            // Esperar clic o entrada de usuario
                            while (1) {
                                if (ismouseclick(WM_LBUTTONDOWN)) {
                                    int mx, my;
                                    getmouseclick(WM_LBUTTONDOWN, mx, my);
                                    
                                    // Botón -
                                    if (mx >= input_x - btn_width - 10 && mx <= input_x - 10 &&
                                        my >= input_y && my <= input_y + btn_height) {
                                        z_value -= 0.1;
                                        sprintf(z_value_str, "%.2f", z_value);
                                        
                                        // Actualizar visualización
                                        setfillstyle(SOLID_FILL, WHITE);
                                        bar(input_x + 1, input_y + 1, input_x + input_width - 1, input_y + input_height - 1);
                                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                                        outtextxy(input_x + input_width/2, input_y + input_height/2, z_value_str);
                                    }
                                    
                                    // Botón +
                                    else if (mx >= input_x + input_width + 10 && mx <= input_x + input_width + btn_width + 10 &&
                                            my >= input_y && my <= input_y + btn_height) {
                                        z_value += 0.1;
                                        sprintf(z_value_str, "%.2f", z_value);
                                        
                                        // Actualizar visualización
                                        setfillstyle(SOLID_FILL, WHITE);
                                        bar(input_x + 1, input_y + 1, input_x + input_width - 1, input_y + input_height - 1);
                                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                                        outtextxy(input_x + input_width/2, input_y + input_height/2, z_value_str);
                                    }
                                    
                                    // Botón calcular
                                    else if (mx >= calc_btn_x && mx <= calc_btn_x + calc_btn_width &&
                                            my >= calc_btn_y && my <= calc_btn_y + calc_btn_height) {
                                        calculate_alpha_z(z_value);
                                        break;
                                    }
                                }
                                
                                delay(50);
                            }
                        }
                        break;
                        
                        case 1: // Encontrar Z dado Alpha
                        {
                            // Interfaz para solicitar el valor de Alpha
                            setcolor(APP_COLOR_TITLE);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                            settextjustify(CENTER_TEXT, TOP_TEXT);
                            outtextxy(WINDOW_WIDTH/2, 50, (char*)"Encontrar Z dado Alpha");
                            
                            setcolor(BLACK);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                            outtextxy(WINDOW_WIDTH/2, 100, (char*)"Introduzca el valor de Alpha (0-1):");
                            
                            // Campo de entrada para Alpha
                            char alpha_value_str[20] = "0.05";
                            double alpha_value = 0.05;
                            
                            // Dibujar campo de entrada
                            int input_width = 100;
                            int input_height = 30;
                            int input_x = WINDOW_WIDTH/2 - input_width/2;
                            int input_y = 130;
                            
                            setfillstyle(SOLID_FILL, WHITE);
                            bar(input_x, input_y, input_x + input_width, input_y + input_height);
                            rectangle(input_x, input_y, input_x + input_width, input_y + input_height);
                            
                            settextjustify(CENTER_TEXT, CENTER_TEXT);
                            outtextxy(input_x + input_width/2, input_y + input_height/2, alpha_value_str);
                            
                            // Botones + y -
                            int btn_width = 30;
                            int btn_height = 30;
                            
                            setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                            bar(input_x - btn_width - 10, input_y, input_x - 10, input_y + btn_height);
                            rectangle(input_x - btn_width - 10, input_y, input_x - 10, input_y + btn_height);
                            outtextxy(input_x - btn_width/2 - 10, input_y + btn_height/2, (char*)"-");
                            
                            bar(input_x + input_width + 10, input_y, input_x + input_width + btn_width + 10, input_y + btn_height);
                            rectangle(input_x + input_width + 10, input_y, input_x + input_width + btn_width + 10, input_y + btn_height);
                            outtextxy(input_x + input_width + btn_width/2 + 10, input_y + btn_height/2, (char*)"+");
                            
                            // Botón calcular
                            int calc_btn_width = 120;
                            int calc_btn_height = 40;
                            int calc_btn_x = WINDOW_WIDTH/2 - calc_btn_width/2;
                            int calc_btn_y = 180;
                            
                            setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                            bar(calc_btn_x, calc_btn_y, calc_btn_x + calc_btn_width, calc_btn_y + calc_btn_height);
                            rectangle(calc_btn_x, calc_btn_y, calc_btn_x + calc_btn_width, calc_btn_y + calc_btn_height);
                            outtextxy(calc_btn_x + calc_btn_width/2, calc_btn_y + calc_btn_height/2, (char*)"Calcular");
                            
                            // Esperar clic o entrada de usuario
                            while (1) {
                                if (ismouseclick(WM_LBUTTONDOWN)) {
                                    int mx, my;
                                    getmouseclick(WM_LBUTTONDOWN, mx, my);
                                    
                                    // Botón -
                                    if (mx >= input_x - btn_width - 10 && mx <= input_x - 10 &&
                                        my >= input_y && my <= input_y + btn_height) {
                                        alpha_value -= 0.01;
                                        if (alpha_value < 0.01) alpha_value = 0.01;
                                        sprintf(alpha_value_str, "%.2f", alpha_value);
                                        
                                        // Actualizar visualización
                                        setfillstyle(SOLID_FILL, WHITE);
                                        bar(input_x + 1, input_y + 1, input_x + input_width - 1, input_y + input_height - 1);
                                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                                        outtextxy(input_x + input_width/2, input_y + input_height/2, alpha_value_str);
                                    }
                                    
                                    // Botón +
                                    else if (mx >= input_x + input_width + 10 && mx <= input_x + input_width + btn_width + 10 &&
                                            my >= input_y && my <= input_y + btn_height) {
                                        alpha_value += 0.01;
                                        if (alpha_value > 0.99) alpha_value = 0.99;
                                        sprintf(alpha_value_str, "%.2f", alpha_value);
                                        
                                        // Actualizar visualización
                                        setfillstyle(SOLID_FILL, WHITE);
                                        bar(input_x + 1, input_y + 1, input_x + input_width - 1, input_y + input_height - 1);
                                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                                        outtextxy(input_x + input_width/2, input_y + input_height/2, alpha_value_str);
                                    }
                                    
                                    // Botón calcular
                                    else if (mx >= calc_btn_x && mx <= calc_btn_x + calc_btn_width &&
                                            my >= calc_btn_y && my <= calc_btn_y + calc_btn_height) {
                                        find_z_from_alpha(alpha_value);
                                        break;
                                    }
                                }
                                
                                delay(50);
                            }
                        }
                        break;
                        
                        case 2: // Buscar en tabla T
                        {
                            // Interfaz para solicitar GL y Alpha
                            setcolor(APP_COLOR_TITLE);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                            settextjustify(CENTER_TEXT, TOP_TEXT);
                            outtextxy(WINDOW_WIDTH/2, 50, (char*)"Buscar en Tabla T");
                            
                            setcolor(BLACK);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                            outtextxy(WINDOW_WIDTH/2, 100, (char*)"Introduzca Grados de Libertad (GL):");
                            
                            // Campo de entrada para GL
                            char df_str[20] = "10";
                            int df = 10;
                            
                            int input_width = 100;
                            int input_height = 30;
                            int input_x = WINDOW_WIDTH/2 - input_width/2;
                            int input_y = 130;
                            
                            setfillstyle(SOLID_FILL, WHITE);
                            bar(input_x, input_y, input_x + input_width, input_y + input_height);
                            rectangle(input_x, input_y, input_x + input_width, input_y + input_height);
                            
                            settextjustify(CENTER_TEXT, CENTER_TEXT);
                            outtextxy(input_x + input_width/2, input_y + input_height/2, df_str);
                            
                            // Botones + y - para GL
                            int btn_width = 30;
                            int btn_height = 30;
                            
                            setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                            bar(input_x - btn_width - 10, input_y, input_x - 10, input_y + btn_height);
                            rectangle(input_x - btn_width - 10, input_y, input_x - 10, input_y + btn_height);
                            outtextxy(input_x - btn_width/2 - 10, input_y + btn_height/2, (char*)"-");
                            
                            bar(input_x + input_width + 10, input_y, input_x + input_width + btn_width + 10, input_y + btn_height);
                            rectangle(input_x + input_width + 10, input_y, input_x + input_width + btn_width + 10, input_y + btn_height);
                            outtextxy(input_x + input_width + btn_width/2 + 10, input_y + btn_height/2, (char*)"+");
                            
                            // Pedir Alpha
                            setcolor(BLACK);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                            outtextxy(WINDOW_WIDTH/2, 180, (char*)"Introduzca Alpha (0-1):");
                            
                            // Campo de entrada para Alpha
                            char alpha_str[20] = "0.05";
                            double alpha = 0.05;
                            
                            int alpha_input_y = 210;
                            
                            setfillstyle(SOLID_FILL, WHITE);
                            bar(input_x, alpha_input_y, input_x + input_width, alpha_input_y + input_height);
                            rectangle(input_x, alpha_input_y, input_x + input_width, alpha_input_y + input_height);
                            
                            settextjustify(CENTER_TEXT, CENTER_TEXT);
                            outtextxy(input_x + input_width/2, alpha_input_y + input_height/2, alpha_str);
                            
                            // Botones + y - para Alpha
                            setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                            bar(input_x - btn_width - 10, alpha_input_y, input_x - 10, alpha_input_y + btn_height);
                            rectangle(input_x - btn_width - 10, alpha_input_y, input_x - 10, alpha_input_y + btn_height);
                            outtextxy(input_x - btn_width/2 - 10, alpha_input_y + btn_height/2, (char*)"-");
                            
                            bar(input_x + input_width + 10, alpha_input_y, input_x + input_width + btn_width + 10, alpha_input_y + btn_height);
                            rectangle(input_x + input_width + 10, alpha_input_y, input_x + input_width + btn_width + 10, alpha_input_y + btn_height);
                            outtextxy(input_x + input_width + btn_width/2 + 10, alpha_input_y + btn_height/2, (char*)"+");
                            
                            // Botón buscar
                            int calc_btn_width = 120;
                            int calc_btn_height = 40;
                            int calc_btn_x = WINDOW_WIDTH/2 - calc_btn_width/2;
                            int calc_btn_y = 260;
                            
                            setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                            bar(calc_btn_x, calc_btn_y, calc_btn_x + calc_btn_width, calc_btn_y + calc_btn_height);
                            rectangle(calc_btn_x, calc_btn_y, calc_btn_x + calc_btn_width, calc_btn_y + calc_btn_height);
                            outtextxy(calc_btn_x + calc_btn_width/2, calc_btn_y + calc_btn_height/2, (char*)"Buscar");
                            
                            // Esperar clic o entrada de usuario
                            while (1) {
                                if (ismouseclick(WM_LBUTTONDOWN)) {
                                    int mx, my;
                                    getmouseclick(WM_LBUTTONDOWN, mx, my);
                                    
                                    // Botón - para GL
                                    if (mx >= input_x - btn_width - 10 && mx <= input_x - 10 &&
                                        my >= input_y && my <= input_y + btn_height) {
                                        df--;
                                        if (df < 1) df = 1;
                                        sprintf(df_str, "%d", df);
                                        
                                        // Actualizar visualización
                                        setfillstyle(SOLID_FILL, WHITE);
                                        bar(input_x + 1, input_y + 1, input_x + input_width - 1, input_y + input_height - 1);
                                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                                        outtextxy(input_x + input_width/2, input_y + input_height/2, df_str);
                                    }
                                    
                                    // Botón + para GL
                                    else if (mx >= input_x + input_width + 10 && mx <= input_x + input_width + btn_width + 10 &&
                                            my >= input_y && my <= input_y + btn_height) {
                                        df++;
                                        if (df > 30) df = 30;
                                        sprintf(df_str, "%d", df);
                                        
                                        // Actualizar visualización
                                        setfillstyle(SOLID_FILL, WHITE);
                                        bar(input_x + 1, input_y + 1, input_x + input_width - 1, input_y + input_height - 1);
                                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                                        outtextxy(input_x + input_width/2, input_y + input_height/2, df_str);
                                    }
                                    
                                    // Botón - para Alpha
                                    if (mx >= input_x - btn_width - 10 && mx <= input_x - 10 &&
                                        my >= alpha_input_y && my <= alpha_input_y + btn_height) {
                                        alpha -= 0.01;
                                        if (alpha < 0.01) alpha = 0.01;
                                        sprintf(alpha_str, "%.2f", alpha);
                                        
                                        // Actualizar visualización
                                        setfillstyle(SOLID_FILL, WHITE);
                                        bar(input_x + 1, alpha_input_y + 1, input_x + input_width - 1, alpha_input_y + input_height - 1);
                                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                                        outtextxy(input_x + input_width/2, alpha_input_y + input_height/2, alpha_str);
                                    }
                                    
                                    // Botón + para Alpha
                                    else if (mx >= input_x + input_width + 10 && mx <= input_x + input_width + btn_width + 10 &&
                                            my >= alpha_input_y && my <= alpha_input_y + btn_height) {
                                        alpha += 0.01;
                                        if (alpha > 0.99) alpha = 0.99;
                                        sprintf(alpha_str, "%.2f", alpha);
                                        
                                        // Actualizar visualización
                                        setfillstyle(SOLID_FILL, WHITE);
                                        bar(input_x + 1, alpha_input_y + 1, input_x + input_width - 1, alpha_input_y + input_height - 1);
                                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                                        outtextxy(input_x + input_width/2, alpha_input_y + input_height/2, alpha_str);
                                    }
                                    
                                    // Botón buscar
                                    else if (mx >= calc_btn_x && mx <= calc_btn_x + calc_btn_width &&
                                            my >= calc_btn_y && my <= calc_btn_y + calc_btn_height) {
                                        search_t_table(df, alpha);
                                        break;
                                    }
                                }
                                
                                delay(50);
                            }
                        }
                        break;
                        
                        case 3: // Calcular GL y Alpha de T
                        {
                            // Interfaz para solicitar el valor de T
                            setcolor(APP_COLOR_TITLE);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                            settextjustify(CENTER_TEXT, TOP_TEXT);
                            outtextxy(WINDOW_WIDTH/2, 50, (char*)"Calcular GL y Alpha de T");
                            
                            setcolor(BLACK);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                            outtextxy(WINDOW_WIDTH/2, 100, (char*)"Introduzca el valor de T:");
                            
                            // Campo de entrada para T
                            char t_value_str[20] = "2.00";
                            double t_value = 2.0;
                            
                            // Dibujar campo de entrada
                            int input_width = 100;
                            int input_height = 30;
                            int input_x = WINDOW_WIDTH/2 - input_width/2;
                            int input_y = 130;
                            
                            setfillstyle(SOLID_FILL, WHITE);
                            bar(input_x, input_y, input_x + input_width, input_y + input_height);
                            rectangle(input_x, input_y, input_x + input_width, input_y + input_height);
                            
                            settextjustify(CENTER_TEXT, CENTER_TEXT);
                            outtextxy(input_x + input_width/2, input_y + input_height/2, t_value_str);
                            
                            // Botones + y -
                            int btn_width = 30;
                            int btn_height = 30;
                            
                            setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                            bar(input_x - btn_width - 10, input_y, input_x - 10, input_y + btn_height);
                            rectangle(input_x - btn_width - 10, input_y, input_x - 10, input_y + btn_height);
                            outtextxy(input_x - btn_width/2 - 10, input_y + btn_height/2, (char*)"-");
                            
                            bar(input_x + input_width + 10, input_y, input_x + input_width + btn_width + 10, input_y + btn_height);
                            rectangle(input_x + input_width + 10, input_y, input_x + input_width + btn_width + 10, input_y + btn_height);
                            outtextxy(input_x + input_width + btn_width/2 + 10, input_y + btn_height/2, (char*)"+");
                            
                            // Botón calcular
                            int calc_btn_width = 120;
                            int calc_btn_height = 40;
                            int calc_btn_x = WINDOW_WIDTH/2 - calc_btn_width/2;
                            int calc_btn_y = 180;
                            
                            setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                            bar(calc_btn_x, calc_btn_y, calc_btn_x + calc_btn_width, calc_btn_y + calc_btn_height);
                            rectangle(calc_btn_x, calc_btn_y, calc_btn_x + calc_btn_width, calc_btn_y + calc_btn_height);
                            outtextxy(calc_btn_x + calc_btn_width/2, calc_btn_y + calc_btn_height/2, (char*)"Calcular");
                            
                            // Esperar clic o entrada de usuario
                            while (1) {
                                if (ismouseclick(WM_LBUTTONDOWN)) {
                                    int mx, my;
                                    getmouseclick(WM_LBUTTONDOWN, mx, my);
                                    
                                    // Botón -
                                    if (mx >= input_x - btn_width - 10 && mx <= input_x - 10 &&
                                        my >= input_y && my <= input_y + btn_height) {
                                        t_value -= 0.1;
                                        if (t_value < 0.1) t_value = 0.1;
                                        sprintf(t_value_str, "%.2f", t_value);
                                        
                                        // Actualizar visualización
                                        setfillstyle(SOLID_FILL, WHITE);
                                        bar(input_x + 1, input_y + 1, input_x + input_width - 1, input_y + input_height - 1);
                                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                                        outtextxy(input_x + input_width/2, input_y + input_height/2, t_value_str);
                                    }
                                    
                                    // Botón +
                                    else if (mx >= input_x + input_width + 10 && mx <= input_x + input_width + btn_width + 10 &&
                                            my >= input_y && my <= input_y + btn_height) {
                                        t_value += 0.1;
                                        sprintf(t_value_str, "%.2f", t_value);
                                        
                                        // Actualizar visualización
                                        setfillstyle(SOLID_FILL, WHITE);
                                        bar(input_x + 1, input_y + 1, input_x + input_width - 1, input_y + input_height - 1);
                                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                                        outtextxy(input_x + input_width/2, input_y + input_height/2, t_value_str);
                                    }
                                    
                                    // Botón calcular
                                    else if (mx >= calc_btn_x && mx <= calc_btn_x + calc_btn_width &&
                                            my >= calc_btn_y && my <= calc_btn_y + calc_btn_height) {
                                        find_df_alpha_from_t(t_value);
                                        break;
                                    }
                                }
                                
                                delay(50);
                            }
                        }
                        break;
                    }
                    
                    return 1;
                }
            }
        }
        else if (active_menu == MENU_CREDITOS) {
            for (int i = 0; i < sizeof(credits_submenu) / sizeof(MenuItem); i++) {
                if (x >= credits_submenu[i].x1 && x <= credits_submenu[i].x2 &&
                    y >= credits_submenu[i].y1 && y <= credits_submenu[i].y2) {
                    
                    // Cerrar todos los menús
                    close_all_menus();
                    draw_menu(); // Actualizar interfaz
                    
                    // Si es "Desarrollador"
                    if (i == 0) {
                        // Llamar a la función especializada para mostrar el perfil
                        show_developer_profile();
                        return ACTION_CREDITS_DEV;
                    }
                }
            }
        }
    }
    
    // Si se hizo clic fuera de los menús activos
    // Ajustar la detección según la nueva disposición de menús
    if (y > 270 || x < 50 || x > 570) {
        close_all_menus();
        draw_menu();
    }
    
    return 0;
}


// Función para abrir una URL en el navegador predeterminado
void open_url(const char* url) {
#ifdef _WIN32
    char command[256];
    sprintf(command, "start %s", url);
    system(command);
#else
    // Para sistemas Unix/Linux
    char command[256];
    sprintf(command, "xdg-open %s", url);
    system(command);
#endif
}

// Función para dibujar un botón interactivo (adaptada para usar UIButton)
void draw_ui_button(UIButton* btn) {
    // Color de fondo según estado hover
    setfillstyle(SOLID_FILL, btn->is_hover ? APP_COLOR_BUTTON_HOVER : APP_COLOR_BUTTON);
    bar(btn->x1, btn->y1, btn->x2, btn->y2);
    
    // Borde del botón
    setcolor(WHITE);
    line(btn->x1, btn->y1, btn->x2, btn->y1); // Superior
    line(btn->x1, btn->y1, btn->x1, btn->y2); // Izquierdo
    
    setcolor(btn->is_hover ? WHITE : DARKGRAY);
    line(btn->x1, btn->y2, btn->x2, btn->y2); // Inferior
    line(btn->x2, btn->y1, btn->x2, btn->y2); // Derecho
    
    // Texto del botón
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy((btn->x1 + btn->x2) / 2, (btn->y1 + btn->y2) / 2, btn->text);
}

// Función para verificar si un punto está dentro de un botón
int is_point_in_button(UIButton* btn, int x, int y) {
    return (x >= btn->x1 && x <= btn->x2 && y >= btn->y1 && y <= btn->y2);
}

// Función principal para mostrar los créditos del desarrollador
void show_developer_profile() {
    // Limpiar área de trabajo
    clear_work_area();
    
    // Crear botón para GitHub
    UIButton github_button = {
        WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT - 80, 
        WINDOW_WIDTH/2 + 200, WINDOW_HEIGHT - 50,
        "Visitar GitHub", 0
    };
    
    // Fondo con gradiente suave
    for (int y = 40; y < WINDOW_HEIGHT - 10; y++) {
        setcolor(COLOR(250, 250, 255 - y/12));
        line(10, y, WINDOW_WIDTH - 10, y);
    }
    
    // Decoración visual - marcos
    setcolor(LIGHTBLUE);
    rectangle(20, 50, WINDOW_WIDTH - 20, WINDOW_HEIGHT - 20);
    rectangle(25, 55, WINDOW_WIDTH - 25, WINDOW_HEIGHT - 25);
    
    // Título principal
    setcolor(APP_COLOR_TITLE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(WINDOW_WIDTH/2, 70, (char*)"Perfil del Desarrollador");
    
    // Línea divisoria
    setcolor(LIGHTGRAY);
    line(WINDOW_WIDTH/4, 160, WINDOW_WIDTH*3/4, 160);
    
    // Información del desarrollador
    settextjustify(LEFT_TEXT, TOP_TEXT);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);

    
    // Sección de proyecto
    setcolor(APP_COLOR_SUBTITLE);
    outtextxy(WINDOW_WIDTH/2 - 60, 175, (char*)"Visita mi Github Aqui");
    
    // Dibujar botón de GitHub
    draw_ui_button(&github_button);
    
    // Loop principal para interactividad
    bool done = false;
    while (!done) {
        // Verificar si el ratón está sobre el botón
        if (ismouseclick(WM_MOUSEMOVE)) {
            int x, y;
            getmouseclick(WM_MOUSEMOVE, x, y);
            
            // Actualizar estado hover del botón
            int was_hover = github_button.is_hover;
            github_button.is_hover = is_point_in_button(&github_button, x, y);
            
            // Si el estado hover cambió, redibujar botón
            if (was_hover != github_button.is_hover) {
                draw_ui_button(&github_button);
            }
        }
        
        // Verificar clic en botón de GitHub
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            if (is_point_in_button(&github_button, x, y)) {
                // Abrir GitHub en el navegador
                open_url("https://github.com/ChrispoMichigan");
            } else {
                // Cualquier otro clic cierra la pantalla de créditos
                done = true;
            }
        }
        
        delay(50);
    }
    
    // Limpiar área al salir
    clear_work_area();
}

// Función para mostrar interfaz de Media Recortada con opción de porcentaje o cantidad
void show_trimmed_mean_options() {
    clear_work_area();
    
    // Variables para opciones
    int selected_option = 0; // 0 = porcentaje, 1 = cantidad
    double percentage = 10.0; // Porcentaje predeterminado
    int count = 1; // Cantidad predeterminada
    
    char title[100];
    sprintf(title, "Media Recortada - Seleccion de Metodo");
    
    // Dibujar interfaz
    setcolor(APP_COLOR_TITLE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(WINDOW_WIDTH/2, 50, title);
    
    // Opciones de método
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    
    int radioY1 = 120;
    int radioY2 = 160;
    int radioX = 100;
    
    // Círculos de radio button
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, selected_option == 0 ? BLUE : WHITE);
    fillellipse(radioX, radioY1, 8, 8);
    circle(radioX, radioY1, 8);
    
    setfillstyle(SOLID_FILL, selected_option == 1 ? BLUE : WHITE);
    fillellipse(radioX, radioY2, 8, 8);
    circle(radioX, radioY2, 8);
    
    // Etiquetas
    setcolor(BLACK);
    outtextxy(radioX + 20, radioY1, (char*)"Por porcentaje (%)");
    outtextxy(radioX + 20, radioY2, (char*)"Por cantidad de valores");
    
    // Campos para valores
    char percent_text[20], count_text[20];
    sprintf(percent_text, "%.1f%%", percentage);
    sprintf(count_text, "%d", count);
    
    // Dibujar campos y valores
    // Campo porcentaje
    setfillstyle(SOLID_FILL, WHITE);
    bar(radioX + 200, radioY1 - 15, radioX + 260, radioY1 + 15);
    rectangle(radioX + 200, radioY1 - 15, radioX + 260, radioY1 + 15);
    setcolor(selected_option == 0 ? BLACK : DARKGRAY);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(radioX + 230, radioY1, percent_text);
    
    // Campo cantidad
    setfillstyle(SOLID_FILL, WHITE);
    bar(radioX + 200, radioY2 - 15, radioX + 260, radioY2 + 15);
    rectangle(radioX + 200, radioY2 - 15, radioX + 260, radioY2 + 15);
    setcolor(selected_option == 1 ? BLACK : DARKGRAY);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(radioX + 230, radioY2, count_text);
    
    // Botones para ajustar valores
    // Porcentaje
    int btnX1 = radioX + 270;
    int btnX2 = radioX + 290;
    setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
    bar(btnX1, radioY1 - 15, btnX2, radioY1);
    bar(btnX1, radioY1, btnX2, radioY1 + 15);
    setcolor(WHITE);
    rectangle(btnX1, radioY1 - 15, btnX2, radioY1);
    rectangle(btnX1, radioY1, btnX2, radioY1 + 15);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(btnX1 + 10, radioY1 - 8, (char*)"+");
    outtextxy(btnX1 + 10, radioY1 + 7, (char*)"-");
    
    // Cantidad
    setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
    bar(btnX1, radioY2 - 15, btnX2, radioY2);
    bar(btnX1, radioY2, btnX2, radioY2 + 15);
    setcolor(WHITE);
    rectangle(btnX1, radioY2 - 15, btnX2, radioY2);
    rectangle(btnX1, radioY2, btnX2, radioY2 + 15);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(btnX1 + 10, radioY2 - 8, (char*)"+");
    outtextxy(btnX1 + 10, radioY2 + 7, (char*)"-");
    
    // Botón calcular
    int calcBtnWidth = 150;
    int calcBtnHeight = 40;
    int calcBtnX = (WINDOW_WIDTH - calcBtnWidth) / 2;
    int calcBtnY = 240;
    
    setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
    bar(calcBtnX, calcBtnY, calcBtnX + calcBtnWidth, calcBtnY + calcBtnHeight);
    setcolor(WHITE);
    rectangle(calcBtnX, calcBtnY, calcBtnX + calcBtnWidth, calcBtnY + calcBtnHeight);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(calcBtnX + calcBtnWidth/2, calcBtnY + calcBtnHeight/2, (char*)"Calcular");
    
    // Botón cancelar
    int cancelBtnY = calcBtnY + calcBtnHeight + 20;
    setfillstyle(SOLID_FILL, COLOR(200, 100, 100));
    bar(calcBtnX, cancelBtnY, calcBtnX + calcBtnWidth, cancelBtnY + calcBtnHeight);
    setcolor(WHITE);
    rectangle(calcBtnX, cancelBtnY, calcBtnX + calcBtnWidth, cancelBtnY + calcBtnHeight);
    outtextxy(calcBtnX + calcBtnWidth/2, cancelBtnY + calcBtnHeight/2, (char*)"Cancelar");
    
    // Esperar clic y procesar selección
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            // Verificar radio buttons
            if (pow(x - radioX, 2) + pow(y - radioY1, 2) <= 64) { // Área del círculo para opción porcentaje
                selected_option = 0;
                // Redibujar radio buttons
                setfillstyle(SOLID_FILL, BLUE);
                fillellipse(radioX, radioY1, 8, 8);
                setfillstyle(SOLID_FILL, WHITE);
                fillellipse(radioX, radioY2, 8, 8);
                
                // Actualizar color de texto
                setcolor(BLACK);
                settextjustify(CENTER_TEXT, CENTER_TEXT);
                outtextxy(radioX + 230, radioY1, percent_text);
                setcolor(DARKGRAY);
                outtextxy(radioX + 230, radioY2, count_text);
            }
            else if (pow(x - radioX, 2) + pow(y - radioY2, 2) <= 64) { // Área del círculo para opción cantidad
                selected_option = 1;
                // Redibujar radio buttons
                setfillstyle(SOLID_FILL, WHITE);
                fillellipse(radioX, radioY1, 8, 8);
                setfillstyle(SOLID_FILL, BLUE);
                fillellipse(radioX, radioY2, 8, 8);
                
                // Actualizar color de texto
                setcolor(DARKGRAY);
                settextjustify(CENTER_TEXT, CENTER_TEXT);
                outtextxy(radioX + 230, radioY1, percent_text);
                setcolor(BLACK);
                outtextxy(radioX + 230, radioY2, count_text);
            }
            
            // Botones para ajustar porcentaje
            else if (x >= btnX1 && x <= btnX2 && y >= radioY1 - 15 && y <= radioY1) { // + porcentaje
                if (percentage < 40.0) {
                    percentage += 5.0;
                    sprintf(percent_text, "%.1f%%", percentage);
                    
                    // Actualizar valor en pantalla
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(radioX + 200, radioY1 - 15, radioX + 260, radioY1 + 15);
                    rectangle(radioX + 200, radioY1 - 15, radioX + 260, radioY1 + 15);
                    setcolor(selected_option == 0 ? BLACK : DARKGRAY);
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                    outtextxy(radioX + 230, radioY1, percent_text);
                }
            }
            else if (x >= btnX1 && x <= btnX2 && y > radioY1 && y <= radioY1 + 15) { // - porcentaje
                if (percentage > 5.0) {
                    percentage -= 5.0;
                    sprintf(percent_text, "%.1f%%", percentage);
                    
                    // Actualizar valor en pantalla
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(radioX + 200, radioY1 - 15, radioX + 260, radioY1 + 15);
                    rectangle(radioX + 200, radioY1 - 15, radioX + 260, radioY1 + 15);
                    setcolor(selected_option == 0 ? BLACK : DARKGRAY);
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                    outtextxy(radioX + 230, radioY1, percent_text);
                }
            }
            
            // Botones para ajustar cantidad
            else if (x >= btnX1 && x <= btnX2 && y >= radioY2 - 15 && y <= radioY2) { // + cantidad
                if (count < current_data.count / 2 - 1) {
                    count++;
                    sprintf(count_text, "%d", count);
                    
                    // Actualizar valor en pantalla
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(radioX + 200, radioY2 - 15, radioX + 260, radioY2 + 15);
                    rectangle(radioX + 200, radioY2 - 15, radioX + 260, radioY2 + 15);
                    setcolor(selected_option == 1 ? BLACK : DARKGRAY);
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                    outtextxy(radioX + 230, radioY2, count_text);
                }
            }
            else if (x >= btnX1 && x <= btnX2 && y > radioY2 && y <= radioY2 + 15) { // - cantidad
                if (count > 1) {
                    count--;
                    sprintf(count_text, "%d", count);
                    
                    // Actualizar valor en pantalla
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(radioX + 200, radioY2 - 15, radioX + 260, radioY2 + 15);
                    rectangle(radioX + 200, radioY2 - 15, radioX + 260, radioY2 + 15);
                    setcolor(selected_option == 1 ? BLACK : DARKGRAY);
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                    outtextxy(radioX + 230, radioY2, count_text);
                }
            }
            
            // Botón calcular
            else if (x >= calcBtnX && x <= calcBtnX + calcBtnWidth && 
                     y >= calcBtnY && y <= calcBtnY + calcBtnHeight) {
                
                clear_work_area();
                StatResult result;
                
                // Calcular según el método seleccionado
                if (selected_option == 0) { // Por porcentaje
                    result = calc_trimmed_mean(&current_data, percentage);
                } else { // Por cantidad
                    result = calc_trimmed_mean_by_count(&current_data, count);
                }
                
                // Mostrar resultado
                setcolor(APP_COLOR_TITLE);
                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                settextjustify(CENTER_TEXT, TOP_TEXT);
                outtextxy(WINDOW_WIDTH/2, 50, (char*)"Resultado - Media Recortada");
                
                // Mostrar el resultado con la función auxiliar
                display_stat_result(result, 100);
                
                // Botón volver
                setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
                bar(calcBtnX, cancelBtnY, calcBtnX + calcBtnWidth, cancelBtnY + calcBtnHeight);
                setcolor(WHITE);
                rectangle(calcBtnX, cancelBtnY, calcBtnX + calcBtnWidth, cancelBtnY + calcBtnHeight);
                settextjustify(CENTER_TEXT, CENTER_TEXT);
                outtextxy(calcBtnX + calcBtnWidth/2, cancelBtnY + calcBtnHeight/2, (char*)"Volver");
                
                // Esperar clic para volver
                while (1) {
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        getmouseclick(WM_LBUTTONDOWN, x, y);
                        if (x >= calcBtnX && x <= calcBtnX + calcBtnWidth && 
                            y >= cancelBtnY && y <= cancelBtnY + calcBtnHeight) {
                            clear_work_area();
                            return;
                        }
                    }
                    delay(50);
                }
                
                return;
            }
            
            // Botón cancelar
            else if (x >= calcBtnX && x <= calcBtnX + calcBtnWidth && 
                     y >= cancelBtnY && y <= cancelBtnY + calcBtnHeight) {
                clear_work_area();
                return;
            }
        }
        
        delay(50);
    }
}

// Función para mostrar resultados de cálculos estadísticos básicos
void show_basic_stats() {
    if (!current_data.is_loaded || current_data.count == 0) {
        setcolor(RED);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, (char*)"No hay datos para calcular");
        delay(2000);
        clear_work_area();
        return;
    }
    
    clear_work_area();
    
    // Título
    setcolor(APP_COLOR_TITLE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(WINDOW_WIDTH/2, 50, (char*)"Estadísticas Básicas");
    
    // Calcular y mostrar Media
    StatResult mean_result = calc_mean(&current_data);
    display_stat_result(mean_result, 100);
    
    // Calcular y mostrar Mediana
    StatResult median_result = calc_median(&current_data);
    display_stat_result(median_result, 170);
    
    // Calcular y mostrar Moda
    StatResult mode_result = calc_mode(&current_data);
    display_stat_result(mode_result, 240);
    
    // Botón para volver
    int btnWidth = 150;
    int btnHeight = 40;
    int btnX = (WINDOW_WIDTH - btnWidth) / 2;
    int btnY = 320;
    
    setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
    bar(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
    setcolor(WHITE);
    rectangle(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(btnX + btnWidth/2, btnY + btnHeight/2, (char*)"Volver");
    
    // Esperar clic en botón
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            if (x >= btnX && x <= btnX + btnWidth && 
                y >= btnY && y <= btnY + btnHeight) {
                clear_work_area();
                return;
            }
        }
        delay(50);
    }
}

// Función para mostrar resultados de varianza y desviación estándar
void show_dispersion_stats() {
    if (!current_data.is_loaded || current_data.count == 0) {
        setcolor(RED);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, (char*)"No hay datos para calcular");
        delay(2000);
        clear_work_area();
        return;
    }
    
    clear_work_area();
    
    // Título
    setcolor(APP_COLOR_TITLE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(WINDOW_WIDTH/2, 20, (char*)"Medidas de Dispersion");
    
    // Calcular y mostrar Varianza (muestra)
    StatResult var_sample_result = calc_variance_sample(&current_data);
    display_stat_result(var_sample_result, 70);
    
    // Calcular y mostrar Desviación Estándar (muestra)
    StatResult stddev_sample_result = calc_stddev_sample(&current_data);
    display_stat_result(stddev_sample_result, 140);
    
    // Calcular y mostrar Varianza (población)
    StatResult var_pop_result = calc_variance_population(&current_data);
    display_stat_result(var_pop_result, 210);
    
    // Calcular y mostrar Desviación Estándar (población)
    StatResult stddev_pop_result = calc_stddev_population(&current_data);
    display_stat_result(stddev_pop_result, 280);
    
    // Botón para volver
    int btnWidth = 150;
    int btnHeight = 40;
    int btnX = (WINDOW_WIDTH - btnWidth) / 2;
    int btnY = 350;
    
    setfillstyle(SOLID_FILL, APP_COLOR_BUTTON);
    bar(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
    setcolor(WHITE);
    rectangle(btnX, btnY, btnX + btnWidth, btnY + btnHeight);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(btnX + btnWidth/2, btnY + btnHeight/2, (char*)"Volver");
    
    // Esperar clic en botón
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            if (x >= btnX && x <= btnX + btnWidth && 
                y >= btnY && y <= btnY + btnHeight) {
                clear_work_area();
                return;
            }
        }
        delay(50);
    }
}
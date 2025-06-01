#include <graphics.h>
#include <stdio.h>
#include "../../include/excel_c.h"

// Variables globales 
DataSet current_data = { {0}, 0, "", 0 };
GraphType current_graph = GRAPH_NONE;

// Dibujar fondo decorativo
void draw_background() {
    // Fondo degradado suave
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        setcolor(COLOR(240, 240, 245 - (i/10)));
        line(0, i, WINDOW_WIDTH, i);
    }
    
    // Marco elegante para la pantalla principal
    setcolor(DARKGRAY);
    rectangle(20, 60, WINDOW_WIDTH-20, WINDOW_HEIGHT-20);
    setcolor(LIGHTGRAY);
    rectangle(22, 62, WINDOW_WIDTH-22, WINDOW_HEIGHT-22);
}

// Inicialización de la aplicación
void app_init() {
    // Configurar modo gráfico
    setbkcolor(WHITE);
    cleardevice();
    
    // Dibujar fondo decorativo
    draw_background();
    
    // Dibujar menú inicial
    draw_menu();
    
    // Dibujar título más elegante
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    setcolor(BLUE);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/3 - 70, (char*)"Sistema de Visualizacion");
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/3 - 20, (char*)"Estadistica");
    
    // Línea decorativa
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    setcolor(DARKGRAY);
    line(WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 + 150, WINDOW_WIDTH/2 + 150, WINDOW_HEIGHT/2 + 150);
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    
    // Mensaje de bienvenida con mejor estilo
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    setcolor(DARKGRAY);
    outtextxy(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 60, (char*)"Seleccione una opcion del menu para comenzar");
    
    // Versión y créditos
    settextstyle(SMALL_FONT, HORIZ_DIR, 5);
    settextjustify(RIGHT_TEXT, BOTTOM_TEXT);
    setcolor(DARKGRAY);
    outtextxy(WINDOW_WIDTH-100, WINDOW_HEIGHT-10, (char*)"v1.0");
}

// Limpieza antes de cerrar la aplicación
void app_cleanup() {
    // Por ahora no hay nada que limpiar específicamente
}

// Bucle principal de la aplicación
void main_loop() {
    int running = 1;
    
    while (running) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            // Manejar clics en el menú
            int result = handle_menu_click(x, y);
            
            if (result == -1) {
                running = 0; // Código para salir
            }
            
            // Redibujar menú después de las acciones
            draw_menu();
        }
        
        delay(100); // Pequeña pausa para evitar uso excesivo de CPU
    }
}
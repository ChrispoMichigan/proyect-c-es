@echo off
echo Compilando el proyecto de visualizacion estadistica...

REM Crear directorios necesarios
mkdir build 2>nul

echo Compilando archivos cpp...
g++ -c src/main.cpp -o build/main.o -I./include
g++ -c src/core/app.cpp -o build/app.o -I./include
g++ -c src/gui/menu.cpp -o build/menu.o -I./include
g++ -c src/core/data_input.cpp -o build/data_input.o -I./include
g++ -c src/graphics/visualizations.cpp -o build/visualizations.o -I./include
g++ -c src/core/statistical.cpp -o build/statistical.o -I./include
g++ -c src/core/tables.cpp -o build/tables.o -I./include

echo Enlazando ejecutable...
g++ build/main.o build/app.o build/menu.o build/data_input.o build/visualizations.o build/statistical.o build/tables.o -o build/excel_c.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32

echo.
if %ERRORLEVEL% == 0 (
    echo Compilacion exitosa! El ejecutable esta en la carpeta build/excel_c.exe
    echo.
    echo Ejecutar programa? (S/N)
    choice /C SN /M "Seleccione una opcion:" 
    if ERRORLEVEL 1 if not ERRORLEVEL 2 build\excel_c.exe
) else (
    echo Error durante la compilacion.
    echo.
)

echo.
pause
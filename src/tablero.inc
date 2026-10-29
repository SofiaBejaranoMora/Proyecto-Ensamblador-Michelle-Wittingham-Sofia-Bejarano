TAB_FILAS EQU 12
TAB_POSICIONES EQU 4
TAB_PASO_FILA EQU 13


PantallaJuego PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    PUSH DI

    CALL LimpiarPantalla

    PINTAR_RECT 4, 4, 312, 192, 1

    ;cuadradito de afuera
    PINTAR_RECT 4, 4, 312, 1, 4
    PINTAR_RECT 4, 195, 312, 1, 4
    PINTAR_RECT 4, 4, 1, 192, 4
    PINTAR_RECT 315, 4, 1, 192, 4

    ; Titulo
    PINTAR_IMAGEN 65, 8, 70, 14, TextoClave, TextoClaveAncho, TextoClaveAlto
    PINTAR_IMAGEN 144, 8, 110, 14, TextoEspectral, TextoEspectralAncho, TextoEspectralAlto

    ;encabezado
    PINTAR_RECT 10, 25, 300, 1, 5

    PINTAR_RECT 8, 29, 204, 158, 2
    PINTAR_RECT 8, 29, 204, 1, 4
    PINTAR_RECT 8, 186, 204, 1, 4
    PINTAR_RECT 8, 29, 1, 158, 4
    PINTAR_RECT 211, 29, 1, 158, 4

    ; Primera fila resaltada
    PINTAR_RECT 10, 31, 200, 12, 4

    ; SI guarda la coordenada Y de cada fila
    MOV SI, 31
    MOV CX, TAB_FILAS

TabDibujarFila:
    PUSH CX

    ; Marca a la izquierda de cada intento
    PINTAR_RECT 14, SI, 3, 12, 5

    MOV BX, 24
    MOV CX, TAB_POSICIONES

TabDibujarCasilla:
    PINTAR_RECT BX, SI, 16, 12, 1

    ADD BX, 22
    LOOP TabDibujarCasilla

    PINTAR_RECT 148, SI, 20, 12, 1
    PINTAR_RECT 180, SI, 20, 12, 1

    ADD SI, TAB_PASO_FILA

    POP CX
    LOOP TabDibujarFila

    PINTAR_RECT 139, 31, 1, 154, 4
    PINTAR_RECT 24, 42, 16, 1, 7

    ; Panel derecho
    PINTAR_RECT 220, 29, 91, 158, 2

    ; Bordecito
    PINTAR_RECT 220, 29, 91, 1, 4
    PINTAR_RECT 220, 186, 91, 1, 4
    PINTAR_RECT 220, 29, 1, 158, 4
    PINTAR_RECT 310, 29, 1, 158, 4

    ; Casillas de las fichas disponibles
    PINTAR_RECT 230, 37, 28, 28, 1
    PINTAR_RECT 273, 37, 28, 28, 1

    PINTAR_RECT 230, 72, 28, 28, 1
    PINTAR_RECT 273, 72, 28, 28, 1

    PINTAR_RECT 230, 107, 28, 28, 1
    PINTAR_RECT 273, 107, 28, 28, 1

    PINTAR_RECT 230, 142, 28, 28, 1
    PINTAR_RECT 273, 142, 28, 28, 1

    ; PAREJITAS
    ;1
    PINTAR_IMAGEN 236, 43, 16, 16, SpriteGato, 16, 16
    PINTAR_IMAGEN 279, 43, 16, 16, SpriteCalabaza, 16, 16
    ;2
    PINTAR_IMAGEN 236, 78, 16, 16, SpriteCaldero, 16, 16
    PINTAR_IMAGEN 279, 78, 16, 16, SpriteDulce, 16, 16
    ;3
    PINTAR_IMAGEN 236, 113, 16, 16, SpriteSombrero, 16, 16
    PINTAR_IMAGEN 279, 113, 16, 16, SpriteCuervo, 16, 16
    ;4
    PINTAR_IMAGEN 236, 148, 16, 16, SpriteBuho, 16, 16
    PINTAR_IMAGEN 279, 148, 16, 16, SpriteMolly, 16, 16

    POP DI
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PantallaJuego ENDP
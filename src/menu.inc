; Pantalla del menu principal.
; Requiere macros.inc, figuras.inc y video.inc.

DibujarSelector PROC
    PUSH AX
    PUSH BX

    MOV AL, OpcionMenu
    MOV BL, 25
    MUL BL
    ADD AX, 59
    MOV BX, AX

    ; Borde superior.
    PINTAR_RECT 87, BX, 146, 1, 4

    ; Borde inferior.
    ADD BX, 22
    PINTAR_RECT 87, BX, 146, 1, 4
    SUB BX, 22

    ; Bordes laterales.
    PINTAR_RECT 87, BX, 1, 23, 4
    PINTAR_RECT 232, BX, 1, 23, 4

    ; Flecha de seleccion.
    ADD BX, 4
    PINTAR_IMAGEN 63, BX, 16, 16, SpriteFlecha, 8, 8

    POP BX
    POP AX
    RET
DibujarSelector ENDP


; Borra el selector de su posicion actual.
BorrarSelector PROC
    PUSH AX
    PUSH BX

    ; Calcular posicion Y actual.
    MOV AL, OpcionMenu
    MOV BL, 25
    MUL BL
    ADD AX, 59
    MOV BX, AX

    ; Borrar borde superior.
    PINTAR_RECT 87, BX, 146, 1, 1

    ; Borrar borde inferior.
    ADD BX, 22
    PINTAR_RECT 87, BX, 146, 1, 1
    SUB BX, 22

    ; Borrar bordes laterales.
    PINTAR_RECT 87, BX, 1, 23, 1
    PINTAR_RECT 232, BX, 1, 23, 1

    ; Borrar imagen de la flecha.
    ADD BX, 4
    PINTAR_RECT 63, BX, 16, 16, 1

    POP BX
    POP AX
    RET
BorrarSelector ENDP


PantallaMenu PROC
    PUSH AX

    PINTAR_RECT 4, 4, 309, 192, 1

    PINTAR_RECT 4, 4, 309, 1, 4
    PINTAR_RECT 4, 195, 309, 1, 4
    PINTAR_RECT 4, 4, 1, 192, 4
    PINTAR_RECT 312, 4, 1, 192, 4

    PINTAR_IMAGEN 54, 13, 85, 26, TextoClave, TextoClaveAncho, TextoClaveAlto
    PINTAR_IMAGEN 150, 13, 115, 26, TextoEspectral, TextoEspectralAncho, TextoEspectralAlto

    PINTAR_RECT 24, 48, 268, 1, 5

    ; Obciones del menu
    PINTAR_IMAGEN 98, 63, 62, 16, TextoJugar, TextoJugarAncho, TextoJugarAlto
    PINTAR_IMAGEN 94, 88, 105, 14, TextoDificultad, TextoDificultadAncho, TextoDificultadAlto
    PINTAR_IMAGEN 94, 113, 136, 14, TextoInstrucciones, TextoInstruccionesAncho, TextoInstruccionesAlto
    PINTAR_IMAGEN 94, 138, 90, 14, TextoCreditos, TextoCreditosAncho, TextoCreditosAlto
    PINTAR_IMAGEN 94, 163, 55, 14, TextoSalir, TextoSalirAncho, TextoSalirAlto

    PINTAR_IMAGEN 12, 140, 42, 42, SpriteGato, 16, 16
    PINTAR_IMAGEN 250, 140, 42, 42, SpriteCalabaza, 16, 16

    CALL DibujarSelector

    POP AX

    RET
PantallaMenu ENDP

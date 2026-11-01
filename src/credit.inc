; Entrada: ES=A000h, DS=DATA. Salida: creditos; conserva registros.
PantallaCreditos PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI

    CALL LimpiarPantalla

    PINTAR_RECT 4, 4, 309, 192, 1

    PINTAR_RECT 4, 4, 309, 1, 4
    PINTAR_RECT 4, 195, 309, 1, 4
    PINTAR_RECT 4, 4, 1, 192, 4
    PINTAR_RECT 312, 4, 1, 192, 4

    PINTAR_IMAGEN 93, 18, 134, 18, TextoCreditos, TextoCreditosAncho, TextoCreditosAlto
    PINTAR_RECT 24, 48, 268, 1, 5

    PINTAR_RECT 30, 61, 260, 83, 2

    ESCRIBIR_TEXTO 9, 12, 'DESARROLLADO POR'

    ESCRIBIR_TEXTO 12, 10, 'MICHELLE WITTINGHAM'

    ESCRIBIR_TEXTO 15, 12, 'SOFIA BEJARANO'

    PINTAR_IMAGEN 30, 151, 24, 24, SpriteGato, 16, 16
    PINTAR_IMAGEN 266, 151, 24, 24, SpriteCalabaza, 16, 16

    ESCRIBIR_TEXTO 22, 11, 'ESC: VOLVER AL MENU'

    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PantallaCreditos ENDP
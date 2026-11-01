; Entrada: ES=A000h, DS=DATA. Salida: portada; conserva AX.
PantallaInicio PROC
    PUSH AX

    PINTAR_RECT 4, 4, 309, 192, 1
    PINTAR_RECT 4, 4, 309, 1, 4
    PINTAR_RECT 4, 195, 309, 1, 4
    PINTAR_RECT 4, 4, 1, 192, 4
    PINTAR_RECT 312, 4, 1, 192, 4

    PINTAR_IMAGEN 116, 13, 88, 25, TextoClave, TextoClaveAncho, TextoClaveAlto
    PINTAR_IMAGEN 91, 37, 138, 25, TextoEspectral, TextoEspectralAncho, TextoEspectralAlto

    PINTAR_IMAGEN 74, 70, 65, 70, SpriteFantasma, 16, 16
    PINTAR_IMAGEN 160, 72, 79, 67, SpriteCalabaza, 16, 16
    PINTAR_IMAGEN 42, 155, 237, 25, TextoEnter, 202, 14
    ESCRIBIR_TEXTO 23, 1, 'MICHELLE WITTINGHAM / SOFIA BEJARANO'
    POP AX
    RET
PantallaInicio ENDP

PantallaInicio PROC
    PUSH AX
    CALL AbrirLienzo
    PINTAR_IMAGEN 116, 13, 88, 25, TextoClave, TextoClaveAncho, TextoClaveAlto
    PINTAR_IMAGEN 91, 37, 138, 25, TextoEspectral, TextoEspectralAncho, TextoEspectralAlto
    PINTAR_RECT 40, 26, 3, 9, 26
    PINTAR_RECT 37, 29, 9, 3, 26
    PINTAR_RECT 270, 48, 3, 9, 20
    PINTAR_RECT 267, 51, 9, 3, 20
    PINTAR_IMAGEN 74, 70, 65, 70, SpriteFantasma, 16, 16
    PINTAR_IMAGEN 160, 72, 79, 67, SpriteCalabaza, 16, 16
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 18, 3, 'Una clave. Ocho pequenos misterios.'
    PINTAR_RECT 40, 156, 240, 24, 25
    PINTAR_RECT 40, 156, 2, 24, 7
    MOV FondoTexto, 25
    MOV ColorTexto, 3
    ESCRIBIR_TEXTO 20, 8, 'ENTER  Empezar la magia'
    MOV FondoTexto, 21
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 23, 3, 'MICHELLE WITTINGHAM / SOFIA BEJARANO'
    ESCRIBIR_TEXTO 24, 15, 'ESC  Salir'
    CALL CerrarEstilo
    POP AX
    RET
PantallaInicio ENDP

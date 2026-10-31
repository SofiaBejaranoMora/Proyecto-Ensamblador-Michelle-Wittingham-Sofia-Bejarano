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

    MOV DH, 9
    MOV DL, 12
    MOV SI, OFFSET CredDesarrollo
    CALL EscribirTextoDificultad

    MOV DH, 12
    MOV DL, 10
    MOV SI, OFFSET CredMichelle
    CALL EscribirTextoDificultad

    MOV DH, 15
    MOV DL, 12
    MOV SI, OFFSET CredSofia
    CALL EscribirTextoDificultad

    PINTAR_IMAGEN 30, 151, 24, 24, SpriteGato, 16, 16
    PINTAR_IMAGEN 266, 151, 24, 24, SpriteCalabaza, 16, 16

    MOV DH, 22
    MOV DL, 11
    MOV SI, OFFSET CredVolver
    CALL EscribirTextoDificultad

    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PantallaCreditos ENDP
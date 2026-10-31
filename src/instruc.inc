PantallaInstrucciones PROC
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

    PINTAR_IMAGEN 57, 18, 206, 18, TextoInstrucciones, TextoInstruccionesAncho, TextoInstruccionesAlto
    PINTAR_RECT 24, 48, 268, 1, 5

    PINTAR_RECT 16, 59, 288, 103, 2

    MOV DH, 8
    MOV DL, 3
    MOV SI, OFFSET InstObjetivo
    CALL EscribirTextoDificultad

    MOV DH, 10
    MOV DL, 3
    MOV SI, OFFSET InstCombinacion
    CALL EscribirTextoDificultad

    MOV DH, 12
    MOV DL, 3
    MOV SI, OFFSET InstIntento
    CALL EscribirTextoDificultad

    MOV DH, 14
    MOV DL, 3
    MOV SI, OFFSET InstPistas
    CALL EscribirTextoDificultad

    MOV DH, 16
    MOV DL, 3
    MOV SI, OFFSET InstPosicion
    CALL EscribirTextoDificultad

    MOV DH, 18
    MOV DL, 3
    MOV SI, OFFSET InstGanar
    CALL EscribirTextoDificultad

    MOV DH, 22
    MOV DL, 11
    MOV SI, OFFSET InstVolver
    CALL EscribirTextoDificultad

    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PantallaInstrucciones ENDP
; Pantalla de dificultad.
; Requiere macros.inc, video.inc y menuDato.inc.
PantallaDificultad PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI

    CALL LimpiarPantalla

    ; Fondo y bordes.
    PINTAR_RECT 4, 4, 309, 192, 1

    PINTAR_RECT 4, 4, 309, 1, 4
    PINTAR_RECT 4, 195, 309, 1, 4
    PINTAR_RECT 4, 4, 1, 192, 4
    PINTAR_RECT 312, 4, 1, 192, 4

    ; Titulo.
    PINTAR_IMAGEN 81, 18, 158, 18, TextoDificultad, TextoDificultadAncho, TextoDificultadAlto

    PINTAR_RECT 24, 48, 268, 1, 5

    ; Cuadros de las opciones.
    PINTAR_RECT 88, 62, 144, 24, 2
    PINTAR_RECT 88, 94, 144, 24, 2
    PINTAR_RECT 88, 126, 144, 24, 2

    ; Texto FACIL: fila 9, columna 17.
    MOV DH, 9
    MOV DL, 17
    MOV SI, OFFSET DifFacil
    CALL EscribirTextoDificultad

    ; Texto NORMAL: fila 13, columna 17.
    MOV DH, 13
    MOV DL, 17
    MOV SI, OFFSET DifNormal
    CALL EscribirTextoDificultad

    ; Texto DIFICIL: fila 17, columna 17.
    MOV DH, 17
    MOV DL, 17
    MOV SI, OFFSET DifDificil
    CALL EscribirTextoDificultad

    ; Indicacion inferior.
    MOV DH, 22
    MOV DL, 9
    MOV SI, OFFSET DifVolver
    CALL EscribirTextoDificultad

    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PantallaDificultad ENDP

EscribirTextoDificultad PROC
    PUSH AX
    PUSH BX
    PUSH DX
    PUSH SI

DifSiguienteLetra:
    LODSB
    CMP AL, 0
    JE DifFinTexto

    PUSH AX
    MOV AH, 02H
    XOR BH, BH
    INT 10H
    POP AX

    MOV AH, 09H
    XOR BH, BH
    MOV BL, 3
    PUSH CX
    MOV CX, 1
    INT 10H
    POP CX

    INC DL
    JMP DifSiguienteLetra

DifFinTexto:
    POP SI
    POP DX
    POP BX
    POP AX
    RET
EscribirTextoDificultad ENDP
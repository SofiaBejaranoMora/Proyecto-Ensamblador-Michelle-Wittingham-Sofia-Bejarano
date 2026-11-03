PintarOpcionMenu PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    MOV AL, OpcionMenu
    MOV BL, 24
    MUL BL
    ADD AX, 56
    MOV BX, AX
    XOR DX, DX
    MOV DL, FondoTexto
    PINTAR_RECT 64, BX, 192, 22, DX
    CMP FondoTexto, 25
    JNE MenuSinAcento
    PINTAR_RECT 64, BX, 2, 22, 7
MenuSinAcento:
    ADD BX, 8
    MOV AX, BX
    MOV CL, 3
    SHR AX, CL
    MOV DH, AL
    MOV ColorTexto, 3
    CMP OpcionMenu, 0
    JNE MenuTextoDificultad
    ESCRIBIR_TEXTO DH, 11, 'Jugar'
    JMP MenuTextoFin
MenuTextoDificultad:
    CMP OpcionMenu, 1
    JNE MenuTextoAyuda
    ESCRIBIR_TEXTO DH, 11, 'Dificultad'
    JMP MenuTextoFin
MenuTextoAyuda:
    CMP OpcionMenu, 2
    JNE MenuTextoCreditos
    ESCRIBIR_TEXTO DH, 11, 'Como se juega'
    JMP MenuTextoFin
MenuTextoCreditos:
    CMP OpcionMenu, 3
    JNE MenuTextoSalir
    ESCRIBIR_TEXTO DH, 11, 'Creditos'
    JMP MenuTextoFin
MenuTextoSalir:
    ESCRIBIR_TEXTO DH, 11, 'Salir'
MenuTextoFin:
    CALL CerrarEstilo
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PintarOpcionMenu ENDP

DibujarSelector PROC
    PUSH AX
    PUSH BX
    MOV FondoTexto, 25
    CALL PintarOpcionMenu
    MOV AL, OpcionMenu
    MOV BL, 24
    MUL BL
    ADD AX, 63
    MOV BX, AX
    PINTAR_IMAGEN 44, BX, 8, 8, SpriteFlecha, 8, 8
    POP BX
    POP AX
    RET
DibujarSelector ENDP

BorrarSelector PROC
    PUSH AX
    PUSH BX
    MOV FondoTexto, 22
    CALL PintarOpcionMenu
    MOV AL, OpcionMenu
    MOV BL, 24
    MUL BL
    ADD AX, 63
    MOV BX, AX
    PINTAR_RECT 44, BX, 8, 8, 21
    POP BX
    POP AX
    RET
BorrarSelector ENDP

PantallaMenu PROC
    PUSH AX
    PUSH CX
    CALL AbrirLienzo
    PINTAR_IMAGEN 24, 8, 48, 16, TextoClave, TextoClaveAncho, TextoClaveAlto
    PINTAR_IMAGEN 78, 8, 82, 16, TextoEspectral, TextoEspectralAncho, TextoEspectralAlto
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 4, 3, 'Que hacemos esta noche?'
    PINTAR_RECT 24, 47, 272, 1, 24
    XOR AX, AX
    MOV AL, OpcionMenu
    PUSH AX
    MOV OpcionMenu, 0
    MOV CX, 5
MenuPintarTarjetas:
    MOV FondoTexto, 22
    CALL PintarOpcionMenu
    INC OpcionMenu
    LOOP MenuPintarTarjetas
    POP AX
    MOV OpcionMenu, AL
    PINTAR_IMAGEN 16, 142, 28, 28, SpriteGato, 16, 16
    PINTAR_IMAGEN 276, 142, 28, 28, SpriteCalabaza, 16, 16
    MOV FondoTexto, 21
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 23, 3, 'F2 Prueba manual   F3 Records'
    ESCRIBIR_TEXTO 24, 3, 'ENTER abrir   ESC salir'
    CALL DibujarSelector
    CALL CerrarEstilo
    CALL TerminarTransicion
    POP CX
    POP AX
    RET
PantallaMenu ENDP

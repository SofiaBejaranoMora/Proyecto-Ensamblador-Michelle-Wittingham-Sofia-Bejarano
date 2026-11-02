CargarPaleta PROC
    PUSH AX
    PUSH CX
    PUSH DX
    PUSH SI
    MOV DX, 03C8H
    XOR AL, AL
    OUT DX, AL
    INC DX
    MOV SI, OFFSET Paleta
    MOV CX, 27 * 3
    CLD
CargarComponente:
    LODSB
    OUT DX, AL
    LOOP CargarComponente
    POP SI
    POP DX
    POP CX
    POP AX
    RET
CargarPaleta ENDP

LimpiarPantalla PROC
    PUSH AX
    PUSH CX
    PUSH DI
    XOR DI, DI
    MOV AL, 8
    MOV CX, 64000
    CLD
    REP STOSB
    POP DI
    POP CX
    POP AX
    RET
LimpiarPantalla ENDP

DibujarRectangulo PROC
    PUSH BP
    MOV BP, SP
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH DI
    CLD
    MOV AX, 320
    MUL WORD PTR [BP+10]
    ADD AX, [BP+12]
    MOV DI, AX
    MOV AX, [BP+4]
    MOV BX, [BP+6]
FilaRectangulo:
    MOV CX, [BP+8]
    REP STOSB
    ADD DI, 320
    SUB DI, [BP+8]
    DEC BX
    JNZ FilaRectangulo
    POP DI
    POP DX
    POP CX
    POP BX
    POP AX
    POP BP
    RET 10
DibujarRectangulo ENDP

DibujarSprite PROC
    PUSH BP
    MOV BP, SP
    SUB SP, 8
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    PUSH DI

    MOV WORD PTR [BP-2], 0
    MOV AX, 320
    MUL WORD PTR [BP+14]
    ADD AX, [BP+16]
    MOV [BP-6], AX
FilaImagen:

    MOV AX, [BP-2]
    MUL WORD PTR [BP+6]
    DIV WORD PTR [BP+10]
    MUL WORD PTR [BP+8]
    ADD AX, [BP+4]
    MOV [BP-8], AX
    MOV DI, [BP-6]
    MOV WORD PTR [BP-4], 0
ColumnaImagen:

    MOV AX, [BP-4]
    MUL WORD PTR [BP+8]
    DIV WORD PTR [BP+12]
    MOV SI, [BP-8]
    ADD SI, AX
    MOV AL, [SI]
    CMP AL, 0
    JNE EscribirPixelSprite

    MOV AL, FondoSprite
    CMP AL, 0
    JE OmitirPixel
EscribirPixelSprite:
    MOV ES:[DI], AL
OmitirPixel:
    INC DI
    INC WORD PTR [BP-4]
    MOV AX, [BP-4]
    CMP AX, [BP+12]
    JB ColumnaImagen
    ADD WORD PTR [BP-6], 320
    INC WORD PTR [BP-2]
    MOV AX, [BP-2]
    CMP AX, [BP+10]
    JB FilaImagen
    POP DI
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    MOV SP, BP
    POP BP
    RET 14
DibujarSprite ENDP

CargarFuenteTexto PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    PUSH DI
    PUSH BP
    PUSH DS
    PUSH ES
    MOV AX, 1130H
    MOV BH, 3
    INT 10H
    MOV SI, BP
    MOV AX, ES
    PUSH DS
    POP ES
    MOV DS, AX
    MOV DI, OFFSET FuenteTexto
    MOV CX, 2048
    CLD
    REP MOVSB
    POP ES
    POP DS
    POP BP
    POP DI
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
CargarFuenteTexto ENDP

EscribirTextoEnPanel PROC
    PUSH BP
    MOV BP, SP
    SUB SP, 2
    MOV [BP-2], DS
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    PUSH DI
    PUSH DS
    MOV AX, SEG FuenteTexto
    MOV DS, AX
    XOR AX, AX
    MOV AL, DH
    MOV CX, 2560
    PUSH DX
    MUL CX
    POP DX
    MOV DI, AX
    XOR AX, AX
    MOV AL, DL
    SHL AX, 1
    SHL AX, 1
    SHL AX, 1
    ADD DI, AX
PanelTextoLetra:

    PUSH DS
    MOV AX, [BP-2]
    MOV DS, AX
    LODSB
    POP DS
    CMP AL, 0
    JE PanelTextoFin
    XOR AH, AH
    SHL AX, 1
    SHL AX, 1
    SHL AX, 1
    MOV BX, AX
    ADD BX, OFFSET FuenteTexto
    PUSH DI
    MOV CH, 8
PanelTextoFila:
    MOV DL, [BX]
    MOV CL, 8
PanelTextoPixel:
    MOV AL, FondoTexto
    TEST DL, 80H
    JZ PanelTextoFondo
    MOV AL, ColorTexto
PanelTextoFondo:
    MOV ES:[DI], AL
    INC DI
    SHL DL, 1
    DEC CL
    JNZ PanelTextoPixel
    ADD DI, 312
    INC BX
    DEC CH
    JNZ PanelTextoFila
    POP DI
    ADD DI, 8
    JMP PanelTextoLetra
PanelTextoFin:
    POP DS
    POP DI
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    MOV SP, BP
    POP BP
    RET
EscribirTextoEnPanel ENDP

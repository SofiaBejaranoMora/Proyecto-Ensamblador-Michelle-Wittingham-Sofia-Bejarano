EsperarCuadroVGA PROC
    PUSH AX
    PUSH CX
    PUSH DX
    MOV DX, 03DAH
    MOV CX, 0FFFFH
FundidoFinCuadro:
    IN AL, DX
    TEST AL, 8
    JZ FundidoEsperarCuadro
    LOOP FundidoFinCuadro
    JMP FundidoEsperaFin
FundidoEsperarCuadro:
    MOV CX, 0FFFFH
FundidoInicioCuadro:
    IN AL, DX
    TEST AL, 8
    JNZ FundidoEsperaFin
    LOOP FundidoInicioCuadro
FundidoEsperaFin:
    POP DX
    POP CX
    POP AX
    RET
EsperarCuadroVGA ENDP

AplicarBrillo PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    PUSH DI
    MOV BL, BrilloActual
    MOV SI, OFFSET Paleta
    MOV DI, OFFSET PaletaPaso
    MOV CX, 81
FundidoCalcularColor:
    LODSB
    MUL BL

    SHR AX, 1
    SHR AX, 1
    SHR AX, 1
    SHR AX, 1
    MOV [DI], AL
    INC DI
    LOOP FundidoCalcularColor
    CALL EsperarCuadroVGA
    MOV DX, 03C8H
    XOR AL, AL
    OUT DX, AL
    INC DX
    MOV SI, OFFSET PaletaPaso
    MOV CX, 81
FundidoEnviarColor:
    LODSB
    OUT DX, AL
    LOOP FundidoEnviarColor
    POP DI
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
AplicarBrillo ENDP

IniciarTransicion PROC
    CMP ModoPruebas, 0
    JNE FundidoSalidaFin
    CALL ApagarSonido
FundidoOscurecer:
    CMP BrilloActual, 0
    JE FundidoSalidaFin
    DEC BrilloActual
    CALL AplicarBrillo
    JMP FundidoOscurecer
FundidoSalidaFin:
    RET
IniciarTransicion ENDP

TerminarTransicion PROC
    CMP ModoPruebas, 0
    JNE FundidoEntradaFin
FundidoIluminar:
    CMP BrilloActual, 16
    JAE FundidoEntradaFin
    INC BrilloActual
    CALL AplicarBrillo
    JMP FundidoIluminar
FundidoEntradaFin:
    RET
TerminarTransicion ENDP

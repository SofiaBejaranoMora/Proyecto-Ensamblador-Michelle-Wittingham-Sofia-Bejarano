; Transiciones VGA: se cambia la paleta, nunca los pixeles de los personajes.
; La tabla Paleta permanece intacta para recuperar exactamente sus colores.

; Espera el comienzo de un cuadro VGA. Conserva AX,CX,DX.
; Las esperas tienen limite para no colgarse si el puerto no responde.
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

; Entrada: BrilloActual (0..16). Salida: DAC actualizado; conserva registros.
; Primero prepara las 81 componentes en RAM; luego las envia durante el retrazo.
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
    ; componente = componenteOriginal * brillo / 16.
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

; Antes de dibujar otra pantalla: bajar a negro en 16 cuadros (~0.23 s).
; Conserva registros. /T evita las esperas en capturas y pruebas ajenas al efecto.
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

; Al terminar de dibujar: revelar la nueva pantalla hasta su brillo original.
; Conserva registros. No se llama al mover selectores ni animar personajes.
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

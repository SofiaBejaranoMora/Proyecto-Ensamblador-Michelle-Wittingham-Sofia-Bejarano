; Reloj sin esperas. INT 1Ah devuelve CX:DX ticks desde medianoche.
; Entrada: partida dibujada. Salida: reloj en cero y activo. Conserva registros.
IniciarCronometro PROC
    PUSH AX
    PUSH CX
    PUSH DX
    MOV AH, 00H
    INT 1AH
    MOV TickReloj, DX
    MOV TickReloj+2, CX
    MOV TicksPartida, 0
    MOV TicksPartida+2, 0
    MOV SegundosJuego, 0
    MOV SegundosMostrados, 0FFFFH
    MOV RelojActivo, 1
    CALL MostrarTiempoJuego
    POP DX
    POP CX
    POP AX
    RET
IniciarCronometro ENDP

; Entrada: CX:DX=ticks actuales. Salida: acumulador actualizado; conserva registros.
AcumularTicks PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    MOV AX, DX
    MOV BX, CX
    SUB AX, TickReloj
    SBB BX, TickReloj+2
    JNC TiempoSinMedianoche
    ; Un dia BIOS contiene 0018:00B0h ticks.
    ADD AX, 00B0H
    ADC BX, 0018H
TiempoSinMedianoche:
    MOV TickReloj, DX
    MOV TickReloj+2, CX
    ADD TicksPartida, AX
    ADC TicksPartida+2, BX
    POP DX
    POP CX
    POP BX
    POP AX
    RET
AcumularTicks ENDP

; Entrada: TicksPartida. Salida: AX=segundos, hasta 5999; conserva BX,DX,SI.
; 182 ticks equivalen aproximadamente a diez segundos (error menor al 0.04%).
TicksASegundos PROC
    PUSH BX
    PUSH DX
    PUSH SI
    MOV AX, TicksPartida
    MOV DX, TicksPartida+2
    CMP DX, 1
    JA TiempoLimite
    JB TiempoConvertir
    CMP AX, 43646
    JAE TiempoLimite
TiempoConvertir:
    MOV SI, DX
    MOV BX, 10
    MUL BX
    CMP SI, 0
    JE TiempoDividir
    ADD DX, 10
TiempoDividir:
    MOV BX, 182
    DIV BX
    CMP AX, 5999
    JBE TiempoConversionFin
TiempoLimite:
    MOV AX, 5999
TiempoConversionFin:
    POP SI
    POP DX
    POP BX
    RET
TicksASegundos ENDP

; Sin parametros. Solo redibuja si cambio el segundo; conserva AX,CX,DX.
ActualizarCronometro PROC
    PUSH AX
    PUSH CX
    PUSH DX
    CMP RelojActivo, 0
    JE TiempoActualizarFin
    MOV AH, 00H
    INT 1AH
    CALL AcumularTicks
    CALL TicksASegundos
    MOV SegundosJuego, AX
    CMP AX, SegundosMostrados
    JE TiempoActualizarFin
    CALL MostrarTiempoJuego
TiempoActualizarFin:
    POP DX
    POP CX
    POP AX
    RET
ActualizarCronometro ENDP

; AX=segundos, DH=fila, DL=columna. Dibuja MM:SS; conserva registros.
EscribirTiempo PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    MOV BX, 60
    PUSH DX
    XOR DX, DX
    DIV BX
    MOV CX, DX
    POP DX
    CALL EscribirNumero
    ADD DL, 2
    ; Escribir ':' desde una cadena literal local en CODE.
    PUSH SI
    PUSH DS
    PUSH CS
    POP DS
    MOV SI, OFFSET SeparadorTiempo
    CALL EscribirTextoDificultad
    POP DS
    POP SI
    INC DL
    MOV AX, CX
    CALL EscribirNumero
    POP DX
    POP CX
    POP BX
    POP AX
    RET
SeparadorTiempo DB ':',0
EscribirTiempo ENDP

; Sin parametros. Muestra el tiempo lateral; conserva AX,DX.
MostrarTiempoJuego PROC
    PUSH AX
    PUSH DX
    ESCRIBIR_TEXTO 20, 26, 'TIEMPO'
    MOV AX, SegundosJuego
    MOV SegundosMostrados, AX
    MOV DH, 20
    MOV DL, 33
    CALL EscribirTiempo
    POP DX
    POP AX
    RET
MostrarTiempoJuego ENDP

; Sin parametros. Resultado final con el tiempo detenido; conserva AX,DX.
MostrarTiempoFinal PROC
    PUSH AX
    PUSH DX
    ESCRIBIR_TEXTO 13, 10, 'TIEMPO:'
    MOV AX, SegundosJuego
    MOV DH, 13
    MOV DL, 20
    CALL EscribirTiempo
    POP DX
    POP AX
    RET
MostrarTiempoFinal ENDP

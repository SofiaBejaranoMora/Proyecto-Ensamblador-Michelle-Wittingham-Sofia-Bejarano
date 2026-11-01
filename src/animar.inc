; Entrada: reloj BIOS. Salida: CF=1 cada cuatro ticks (~0.22 s).
; Conserva AX,CX,DX; alterna CuadroAnimacion, sin esperas que bloqueen teclas.
AvanzarAnimacion PROC
    PUSH AX
    PUSH CX
    PUSH DX
    MOV AH, 00H
    INT 1AH
    MOV AX, DX
    SUB AX, TickAnimacion
    CMP AX, 4
    JB AnimacionTodaviaNo
    MOV TickAnimacion, DX
    XOR CuadroAnimacion, 1
    POP DX
    POP CX
    POP AX
    STC
    RET
AnimacionTodaviaNo:
    POP DX
    POP CX
    POP AX
    CLC
    RET
AvanzarAnimacion ENDP

; Mascota de portada: dos cuadros, sin redibujar el resto. Conserva AX.
AnimarPortada PROC
    PUSH AX
    CALL AvanzarAnimacion
    JNC PortadaAnimacionFin
    PINTAR_RECT 74, 70, 65, 70, 1
    CMP CuadroAnimacion, 0
    JNE PortadaSegundoCuadro
    PINTAR_IMAGEN 74, 70, 65, 70, SpriteFantasma, 16, 16
    JMP PortadaAnimacionFin
PortadaSegundoCuadro:
    PINTAR_IMAGEN 74, 70, 65, 70, SpriteFantasma2, 16, 16
PortadaAnimacionFin:
    POP AX
    RET
AnimarPortada ENDP

; Ficha seleccionada del lateral: dos cuadros. Conserva AX,BX,CX,DX.
AnimarSeleccion PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    CALL AvanzarAnimacion
    JNC SeleccionAnimacionFin
    CALL CalcularMarcaFicha
    ADD BX, 6
    ADD DX, 2
    PINTAR_RECT BX, DX, 16, 16, TAB_CASILLA
    MOV AL, FichaSeleccionada
    MOV CX, 16
    CALL DibujarPersonaje
SeleccionAnimacionFin:
    POP DX
    POP CX
    POP BX
    POP AX
    RET
AnimarSeleccion ENDP

; Victoria: calabaza que salta. Derrota: fantasma que se sacude.
; Sin parametros ni retorno; conserva AX.
AnimarFinal PROC
    PUSH AX
    CALL AvanzarAnimacion
    JNC FinalAnimacionFin
    CALL DibujarCuadroFinal
FinalAnimacionFin:
    POP AX
    RET
AnimarFinal ENDP

; Dibuja inmediatamente el cuadro actual del resultado. Conserva AX.
DibujarCuadroFinal PROC
    PUSH AX
    PINTAR_RECT 132, 108, 56, 48, 21
    CMP PartidaTerminada, 1
    JNE FinalAnimacionDerrota
    CMP CuadroAnimacion, 0
    JNE FinalVictoriaSegundo
    PINTAR_IMAGEN 140, 116, 40, 32, SpriteCalabaza, 16, 16
    JMP CuadroFinalListo
FinalVictoriaSegundo:
    PINTAR_IMAGEN 140, 108, 40, 32, SpriteCalabaza2, 16, 16
    JMP CuadroFinalListo
FinalAnimacionDerrota:
    CMP CuadroAnimacion, 0
    JNE FinalDerrotaSegundo
    PINTAR_IMAGEN 136, 116, 40, 32, SpriteFantasma, 16, 16
    JMP CuadroFinalListo
FinalDerrotaSegundo:
    PINTAR_IMAGEN 148, 116, 32, 32, SpriteFantasma2, 16, 16
CuadroFinalListo:
    POP AX
    RET
DibujarCuadroFinal ENDP

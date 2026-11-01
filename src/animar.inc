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
    CMP CuadroAnimacion, 0
    JNE PortadaSegundoCuadro
    PINTAR_ANIMACION 74, 70, 65, 70, SpriteFantasma, 16, 16, 1
    JMP PortadaAnimacionFin
PortadaSegundoCuadro:
    PINTAR_ANIMACION 74, 70, 65, 70, SpriteFantasma2, 16, 16, 1
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
    MOV FondoSprite, TAB_CASILLA
    MOV AL, FichaSeleccionada
    MOV CX, 16
    CALL DibujarPersonaje
    MOV FondoSprite, 0
SeleccionAnimacionFin:
    POP DX
    POP CX
    POP BX
    POP AX
    RET
AnimarSeleccion ENDP

; Victoria y derrota usan personajes distintos con dos posturas.
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
    CMP PartidaTerminada, 1
    JNE FinalAnimacionDerrota
    CMP CuadroAnimacion, 0
    JNE FinalVictoriaSegundo
    PINTAR_ANIMACION 140, 112, 40, 40, SpriteCalabaza, 16, 16, 21
    JMP CuadroFinalListo
FinalVictoriaSegundo:
    PINTAR_ANIMACION 140, 112, 40, 40, SpriteCalabaza2, 16, 16, 21
    JMP CuadroFinalListo
FinalAnimacionDerrota:
    CMP CuadroAnimacion, 0
    JNE FinalDerrotaSegundo
    PINTAR_ANIMACION 140, 112, 40, 40, SpriteFantasma, 16, 16, 21
    JMP CuadroFinalListo
FinalDerrotaSegundo:
    PINTAR_ANIMACION 140, 112, 40, 40, SpriteFantasma2, 16, 16, 21
CuadroFinalListo:
    POP AX
    RET
DibujarCuadroFinal ENDP

; Rutinas de video. Incluir dentro de .CODE.
; DS apunta a datos y ES a A000H. Preservan registros como en cada encabezado.

; Espera el inicio del refresco vertical.
EsperarRefresco PROC
    PUSH AX
    PUSH DX

    MOV DX, 03DAH

RefrescoFinActual:
    IN AL, DX
    TEST AL, 08H
    JNZ RefrescoFinActual

RefrescoComienzo:
    IN AL, DX
    TEST AL, 08H
    JZ RefrescoComienzo

    POP DX
    POP AX
    RET
EsperarRefresco ENDP

; Carga componentes VGA de 0..63. Conserva AX,CX,DX,SI; deja DF=0.
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
    MOV CX, CANTIDAD_COLORES * 3
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

; Recibe ES=A000H. Fondo negro (indice 8); conserva AX,CX,DI. DF=0.
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

; Recibe por pila X,Y,ancho,alto,color (5 palabras), ES=A000H.
; Dimensiones positivas, todo dentro de 320x200.
; Devuelve rectangulo relleno. Conserva AX,BX,CX,DX,DI,BP.
; Cambia flags, deja DF=0. RET 10 retira los parametros.
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

; Recibe 7 palabras: X,Y,anchoDestino,altoDestino,anchoOrigen,
; altoOrigen,puntero. DS contiene los datos y ES=A000H.
; +4 puntero, +6 altoOrigen, +8 anchoOrigen, +10 altoDestino,
; +12 anchoDestino, +14 Y, +16 X. Dimensiones positivas.
; Requiere imagen dentro de pantalla y datos dentro del segmento DS.
; Devuelve imagen escalada por vecino mas cercano; 0 transparente.
; Conserva AX,BX,CX,DX,SI,DI,BP. Cambia flags. RET 14 limpia parametros.
DibujarImagen PROC
    PUSH BP
    MOV BP, SP
    SUB SP, 8
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    PUSH DI
    ; Locales: -2 fila destino, -4 columna destino,
    ; -6 inicio fila en video, -8 inicio fila en los datos.
    MOV WORD PTR [BP-2], 0
    MOV AX, 320
    MUL WORD PTR [BP+14]
    ADD AX, [BP+16]
    MOV [BP-6], AX
FilaImagen:
    ; filaOrigen = filaDestino * altoOrigen / altoDestino.
    MOV AX, [BP-2]
    MUL WORD PTR [BP+6]
    DIV WORD PTR [BP+10]
    MUL WORD PTR [BP+8]
    ADD AX, [BP+4]
    MOV [BP-8], AX
    MOV DI, [BP-6]
    MOV WORD PTR [BP-4], 0
ColumnaImagen:
    ; columnaOrigen = columnaDestino * anchoOrigen / anchoDestino.
    MOV AX, [BP-4]
    MUL WORD PTR [BP+8]
    DIV WORD PTR [BP+12]
    MOV SI, [BP-8]
    ADD SI, AX
    MOV AL, [SI]
    CMP AL, 0
    JE OmitirPixel
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
DibujarImagen ENDP


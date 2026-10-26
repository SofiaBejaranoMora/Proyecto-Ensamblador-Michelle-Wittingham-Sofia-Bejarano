; Macros para pasar parametros al dibujo. Incluir antes de .DATA.
PINTAR_RECT MACRO x, y, ancho, alto, color
    MOV AX, x
    PUSH AX
    MOV AX, y
    PUSH AX
    MOV AX, ancho
    PUSH AX
    MOV AX, alto
    PUSH AX
    MOV AX, color
    PUSH AX
    CALL DibujarRectangulo
ENDM

PINTAR_IMAGEN MACRO x, y, ancho, alto, datos, anchoOrigen, altoOrigen
    MOV AX, x
    PUSH AX
    MOV AX, y
    PUSH AX
    MOV AX, ancho
    PUSH AX
    MOV AX, alto
    PUSH AX
    MOV AX, anchoOrigen
    PUSH AX
    MOV AX, altoOrigen
    PUSH AX
    MOV AX, OFFSET datos
    PUSH AX
    CALL DibujarImagen
ENDM

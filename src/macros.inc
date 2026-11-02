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
    CALL DibujarSprite
ENDM

ESCRIBIR_TEXTO MACRO fila, columna, texto
    LOCAL cadena, mostrar
    JMP NEAR PTR mostrar
cadena DB texto, 0
mostrar:
    PUSH DS
    PUSH DX
    PUSH SI
    PUSH CS
    POP DS
    MOV DH, fila
    MOV DL, columna
    MOV SI, OFFSET cadena
    CALL EscribirTextoDificultad
    POP SI
    POP DX
    POP DS
ENDM

PINTAR_ANIMACION MACRO x, y, ancho, alto, datos, anchoOrigen, altoOrigen, fondo
    MOV FondoSprite, fondo
    PINTAR_IMAGEN x, y, ancho, alto, datos, anchoOrigen, altoOrigen
    MOV FondoSprite, 0
ENDM

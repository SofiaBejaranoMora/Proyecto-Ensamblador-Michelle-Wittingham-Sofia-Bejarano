; Instrucciones con controles y leyenda grafica. Conserva AX.
PantallaInstrucciones PROC
    PUSH AX
    CALL AbrirLienzo
    ESCRIBIR_TEXTO 1, 3, 'UN POQUITO DE MAGIA'
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 3, 3, 'Descubre la clave antes de quedarte'
    ESCRIBIR_TEXTO 4, 3, 'sin intentos.'
    PINTAR_RECT 16, 48, 288, 64, 22
    PINTAR_RECT 16, 48, 2, 64, 7
    MOV FondoTexto, 22
    MOV ColorTexto, 3
    ESCRIBIR_TEXTO 7, 3, '1  Elige: numeros o arriba/abajo'
    ESCRIBIR_TEXTO 9, 3, '2  Mueve: izquierda/derecha'
    ESCRIBIR_TEXTO 11, 3, '3  ENTER prueba; Backspace borra'
    PINTAR_RECT 16, 120, 136, 52, 22
    PINTAR_RECT 168, 120, 136, 52, 22
    MOV ColorTexto, 7
    ESCRIBIR_TEXTO 16, 3, 'TORO'
    MOV ColorTexto, 12
    ESCRIBIR_TEXTO 16, 22, 'VACA'
    PINTAR_IMAGEN 128, 124, 16, 16, SpriteGato, 16, 16
    PINTAR_IMAGEN 280, 124, 16, 16, SpriteCalabaza, 16, 16
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 18, 3, 'Ficha y lugar'
    ESCRIBIR_TEXTO 20, 3, 'correctos'
    ESCRIBIR_TEXTO 18, 22, 'Ficha bien,'
    ESCRIBIR_TEXTO 20, 22, 'otro lugar'
    MOV FondoTexto, 21
    ESCRIBIR_TEXTO 24, 3, 'ESC  Volver al menu'
    CALL CerrarEstilo
    CALL TerminarTransicion
    POP AX
    RET
PantallaInstrucciones ENDP

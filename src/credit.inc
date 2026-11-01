; Creditos de ambas integrantes. Conserva AX.
PantallaCreditos PROC
    PUSH AX
    CALL AbrirLienzo
    ESCRIBIR_TEXTO 1, 3, 'HECHO ENTRE HECHIZOS'
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 3, 3, 'Dos mentes, una clave por descubrir.'
    PINTAR_RECT 16, 56, 288, 44, 22
    PINTAR_RECT 16, 56, 2, 44, 7
    PINTAR_RECT 16, 112, 288, 44, 22
    PINTAR_RECT 16, 112, 2, 44, 18
    PINTAR_IMAGEN 32, 64, 24, 24, SpriteGato, 16, 16
    PINTAR_IMAGEN 32, 120, 24, 24, SpriteCalabaza, 16, 16
    MOV FondoTexto, 22
    MOV ColorTexto, 3
    ESCRIBIR_TEXTO 8, 10, 'MICHELLE WITTINGHAM'
    ESCRIBIR_TEXTO 15, 10, 'SOFIA BEJARANO'
    MOV ColorTexto, 26
    ESCRIBIR_TEXTO 11, 10, 'Desarrollo y diseno'
    ESCRIBIR_TEXTO 18, 10, 'Desarrollo y diseno'
    MOV FondoTexto, 21
    ESCRIBIR_TEXTO 21, 3, 'Gracias por jugar nuestra historia.'
    ESCRIBIR_TEXTO 24, 3, 'ESC  Volver al menu'
    CALL CerrarEstilo
    CALL TerminarTransicion
    POP AX
    RET
PantallaCreditos ENDP

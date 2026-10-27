; Pantalla del menu principal.
; Requiere macros.inc, figuras.inc y video.inc.

PantallaMenu PROC
    PUSH AX

    ; Fondo 
    PINTAR_RECT 4, 4, 309, 192, 1

    ; Borde
    PINTAR_RECT 4, 4, 309, 1, 4
    PINTAR_RECT 4, 195, 309, 1, 4
    PINTAR_RECT 4, 4, 1, 192, 4
    PINTAR_RECT 312, 4, 1, 192, 4

    ; Titulo CLAVE ESPECTRAL.
    PINTAR_IMAGEN 54, 13, 85, 26, TextoClave, TextoClaveAncho, TextoClaveAlto
    PINTAR_IMAGEN 150, 13, 115, 26, TextoEspectral, TextoEspectralAncho, TextoEspectralAlto

    ; Linea decorativa debajo del titulo.
    PINTAR_RECT 24, 48, 268, 1, 5

    ;rectangulo de seleccion.
    PINTAR_RECT 87, 59, 146, 1, 4 ; arriba
    PINTAR_RECT 87, 81, 146, 1, 4 ; abajo
    PINTAR_RECT 87, 59, 1, 23, 4 ; izquierda
    PINTAR_RECT 232, 59, 1, 23, 4 ; derecha

    ; Flecha de seleccion.
    PINTAR_RECT 63, 63, 2, 16, 4
    PINTAR_RECT 65, 64, 2, 14, 4
    PINTAR_RECT 67, 65, 2, 12, 4
    PINTAR_RECT 69, 66, 2, 10, 4
    PINTAR_RECT 71, 67, 2, 8, 4
    PINTAR_RECT 73, 68, 2, 6, 4
    PINTAR_RECT 75, 69, 2, 4, 4
    PINTAR_RECT 77, 70, 2, 2, 4

    ; Obciones del menu
    PINTAR_IMAGEN 98, 63, 62, 16, TextoJugar, TextoJugarAncho, TextoJugarAlto
    PINTAR_IMAGEN 94, 88, 105, 14, TextoDificultad, TextoDificultadAncho, TextoDificultadAlto
    PINTAR_IMAGEN 94, 113, 136, 14, TextoInstrucciones, TextoInstruccionesAncho, TextoInstruccionesAlto
    PINTAR_IMAGEN 94, 138, 90, 14, TextoCreditos, TextoCreditosAncho, TextoCreditosAlto
    PINTAR_IMAGEN 94, 163, 55, 14, TextoSalir, TextoSalirAncho, TextoSalirAlto

    ; Personajes decorativos.
    PINTAR_IMAGEN 12, 140, 42, 42, SpriteGato, 16, 16
    PINTAR_IMAGEN 250, 140, 42, 42, SpriteCalabaza, 16, 16

    POP AX
    RET
PantallaMenu ENDP

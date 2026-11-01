; Logica principal del juego.

IniciarPartida PROC
    PUSH AX
    PUSH CX
    PUSH SI

    ; Reiniciar el estado de la partida.
    MOV TurnoActual, 0
    MOV PosicionActual, 0
    MOV FichaSeleccionada, 1

    ; Limpiar el intento anterior.
    XOR SI, SI
    MOV CX, 5

    LimpiarIntentoInicial:
        MOV IntentoActual[SI], 0
        INC SI
        LOOP LimpiarIntentoInicial

    ; Configurar dificultad.
    CMP NivelActual, 0
    JNE RevisarNormal

    ; FACIL
    MOV CantidadPosiciones, 4
    MOV CantidadFichas, 6
    MOV IntentosMaximos, 12
    MOV PermitirRepetidos, 0
    JMP FinIniciarPartida

    RevisarNormal:
        CMP NivelActual, 1
        JNE ConfigurarDificil

        ; NORMAL
        MOV CantidadPosiciones, 4
        MOV CantidadFichas, 6
        MOV IntentosMaximos, 10
        MOV PermitirRepetidos, 1
        JMP FinIniciarPartida

    ConfigurarDificil:
        ; DIFICIL
        MOV CantidadPosiciones, 5
        MOV CantidadFichas, 8
        MOV IntentosMaximos, 10
        MOV PermitirRepetidos, 1

    FinIniciarPartida:
        POP SI
        POP CX
        POP AX
        RET
IniciarPartida ENDP

; Calcular posicion de la marca en el panel de fichas.
; Devuelve X en BX e Y en DX.
CalcularMarcaFicha PROC
    PUSH AX

    MOV AL, FichaSeleccionada
    DEC AL

    ; Dividir entre las dos columnas del panel.
    XOR AH, AH
    MOV BL, 2
    DIV BL

    ; AH = columna, AL = fila.
    MOV BX, 230
    CMP AH, 0
    JE MarcaPrimeraColumna
    ADD BX, 43

    MarcaPrimeraColumna:
        MOV AH, 0
        MOV DL, 35
        MUL DL
        ADD AX, 65
        MOV DX, AX

        POP AX
        RET
CalcularMarcaFicha ENDP

; Marcar la ficha seleccionada.
DibujarMarcaFicha PROC
    PUSH BX
    PUSH DX

    CALL CalcularMarcaFicha
    PINTAR_RECT BX, DX, 28, 1, 7

    POP DX
    POP BX
    RET
DibujarMarcaFicha ENDP

; Borrar la marca anterior.
BorrarMarcaFicha PROC
    PUSH BX
    PUSH DX

    CALL CalcularMarcaFicha
    PINTAR_RECT BX, DX, 28, 1, TAB_PANEL

    POP DX
    POP BX
    RET
BorrarMarcaFicha ENDP

; Dibujar cursor debajo de la casilla.
DibujarCursorJuego PROC
    PUSH AX
    PUSH BX

    MOV AL, PosicionActual
    MOV BL, 22
    MUL BL
    ADD AX, 24
    MOV BX, AX

    PINTAR_RECT BX, 42, 16, 1, 7

    POP BX
    POP AX
    RET
DibujarCursorJuego ENDP

; Borrar cursor anterior.
BorrarCursorJuego PROC
    PUSH AX
    PUSH BX

    MOV AL, PosicionActual
    MOV BL, 22
    MUL BL
    ADD AX, 24
    MOV BX, AX

    PINTAR_RECT BX, 42, 16, 1, TAB_CASILLA

    POP BX
    POP AX
    RET
BorrarCursorJuego ENDP

; Dibujar el personaje seleccionado en la casilla actual.

; Dibujar el personaje seleccionado en la casilla actual.
DibujarFichaActual PROC
    PUSH AX
    PUSH BX

    ; Calcular la coordenada X.
    MOV AL, PosicionActual
    MOV BL, 22
    MUL BL
    ADD AX, 27

    ; Guardar la coordenada calculada.
    MOV CoordenadaFicha, AX

    ; Limpiar el dibujo anterior.
    PINTAR_RECT CoordenadaFicha, 32, 10, 10, TAB_CASILLA

    ; Revisar el personaje seleccionado.
    MOV AL, FichaSeleccionada

    CMP AL, 1
    JNE RevisarFicha2
    JMP DibujarGato

    RevisarFicha2:
        CMP AL, 2
        JNE RevisarFicha3
        JMP DibujarCalabaza

    RevisarFicha3:
        CMP AL, 3
        JNE RevisarFicha4
        JMP DibujarCaldero

    RevisarFicha4:
        CMP AL, 4
        JNE RevisarFicha5
        JMP DibujarDulce

    RevisarFicha5:
        CMP AL, 5
        JNE RevisarFicha6
        JMP DibujarSombrero

    RevisarFicha6:
        CMP AL, 6
        JNE RevisarFicha7
        JMP DibujarCuervo

    RevisarFicha7:
        CMP AL, 7
        JNE RevisarFicha8
        JMP DibujarBuho

    RevisarFicha8:
        CMP AL, 8
        JNE FichaNoEsMolly
        JMP DibujarMolly

    FichaNoEsMolly:
        JMP FinDibujarFicha

    DibujarGato:
        PINTAR_IMAGEN CoordenadaFicha, 32, 10, 10, SpriteGato, 16, 16
        JMP FinDibujarFicha

    DibujarCalabaza:
        PINTAR_IMAGEN CoordenadaFicha, 32, 10, 10, SpriteCalabaza, 16, 16
        JMP FinDibujarFicha

    DibujarCaldero:
        PINTAR_IMAGEN CoordenadaFicha, 32, 10, 10, SpriteCaldero, 16, 16
        JMP FinDibujarFicha

    DibujarDulce:
        PINTAR_IMAGEN CoordenadaFicha, 32, 10, 10, SpriteDulce, 16, 16
        JMP FinDibujarFicha

    DibujarSombrero:
        PINTAR_IMAGEN CoordenadaFicha, 32, 10, 10, SpriteSombrero, 16, 16
        JMP FinDibujarFicha

    DibujarCuervo:
        PINTAR_IMAGEN CoordenadaFicha, 32, 10, 10, SpriteCuervo, 16, 16
        JMP FinDibujarFicha

    DibujarBuho:
        PINTAR_IMAGEN CoordenadaFicha, 32, 10, 10, SpriteBuho, 16, 16
        JMP FinDibujarFicha

    DibujarMolly:
        PINTAR_IMAGEN CoordenadaFicha, 32, 10, 10, SpriteMolly, 16, 16

    FinDibujarFicha:
        POP BX
        POP AX
        RET
DibujarFichaActual ENDP

; Guardar el personaje en el intento actual.
ColocarFicha PROC
    PUSH AX
    PUSH SI

    XOR SI, SI
    MOV AL, PosicionActual
    MOV SI, AX
    AND SI, 00FFH

    MOV AL, FichaSeleccionada
    MOV IntentoActual[SI], AL

    CALL DibujarFichaActual

    POP SI
    POP AX
    RET
ColocarFicha ENDP

; Control del teclado durante el juego.
ControlarJuego PROC
    PUSH AX

    CALL DibujarCursorJuego
    CALL DibujarMarcaFicha

    JuegoEsperar:
        CALL ActualizarSonido

        MOV AH, 01H
        INT 16H
        JZ JuegoEsperar

        MOV AH, 10H
        INT 16H

        ; ESC: regresar al menu.
        CMP AL, 27
        JNE RevisarIzquierdaJuego
        JMP JuegoSalir

    RevisarIzquierdaJuego:
        CMP AH, 4BH
        JNE RevisarDerechaJuego
        JMP JuegoIzquierda

    RevisarDerechaJuego:
        CMP AH, 4DH
        JNE RevisarArribaJuego
        JMP JuegoDerecha

    RevisarArribaJuego:
        CMP AH, 48H
        JNE RevisarAbajoJuego
        JMP JuegoFichaAnterior

    RevisarAbajoJuego:
        CMP AH, 50H
        JNE RevisarEnterJuego
        JMP JuegoFichaSiguiente

    RevisarEnterJuego:
        CMP AL, 13
        JNE VolverAEsperarJuego
        JMP JuegoColocar

    VolverAEsperarJuego:
        JMP JuegoEsperar

        ; Mover cursor a la izquierda.
    JuegoIzquierda:
        CMP PosicionActual, 0
        JNE JuegoMoverIzquierda
        JMP JuegoEsperar

    JuegoMoverIzquierda:
        CALL BorrarCursorJuego
        DEC PosicionActual
        CALL DibujarCursorJuego
        CALL SonidoSeleccion
        JMP JuegoEsperar

        ; Mover cursor a la derecha.
    JuegoDerecha:
        MOV AL, CantidadPosiciones
        DEC AL

        CMP PosicionActual, AL
        JB JuegoMoverDerecha
        JMP JuegoEsperar

    JuegoMoverDerecha:
        CALL BorrarCursorJuego
        INC PosicionActual
        CALL DibujarCursorJuego
        CALL SonidoSeleccion
        JMP JuegoEsperar

        ; Elegir personaje anterior.
    JuegoFichaAnterior:
        CALL BorrarMarcaFicha

        CMP FichaSeleccionada, 1
        JE JuegoUltimaFicha

        DEC FichaSeleccionada
        JMP JuegoActualizarFicha

    JuegoUltimaFicha:
        MOV AL, CantidadFichas
        MOV FichaSeleccionada, AL
        JMP JuegoActualizarFicha

        ; Elegir personaje siguiente.
    JuegoFichaSiguiente:
        CALL BorrarMarcaFicha

        MOV AL, FichaSeleccionada
        CMP AL, CantidadFichas
        JB JuegoIncrementarFicha

        MOV FichaSeleccionada, 1
        JMP JuegoActualizarFicha

    JuegoIncrementarFicha:
        INC FichaSeleccionada

    JuegoActualizarFicha:
        CALL DibujarMarcaFicha
        CALL SonidoSeleccion
        JMP JuegoEsperar

        ; Colocar personaje con ENTER.
    JuegoColocar:
        CALL ColocarFicha
        CALL SonidoSeleccion
        JMP JuegoEsperar

        ; Salir del tablero.
    JuegoSalir:
        CALL SonidoSeleccion
        POP AX
        RET
ControlarJuego ENDP

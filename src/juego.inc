; Cada intento ocupa cinco bytes; los niveles de cuatro dejan el ultimo en cero.
; Las rutinas de este archivo reciben el estado en DATA y conservan registros.

; Entrada: NivelActual. Salida: reglas, clave y estado inicializados; conserva AX,CX,SI.
IniciarPartida PROC
    PUSH AX
    PUSH CX
    PUSH SI

    ; Reiniciar el estado de la partida.
    MOV TurnoActual, 0
    MOV FilaJuego, 24
    MOV PartidaTerminada, 0
    MOV PosicionActual, 0
    MOV FichaSeleccionada, 1
    MOV ClaveManual, 0

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
        ; Limpiar historial y pistas de la partida anterior.
        XOR SI, SI
        MOV CX, 60
InicialLimpiarHistorial:
        MOV Historial[SI], 0
        INC SI
        LOOP InicialLimpiarHistorial
        XOR SI, SI
        MOV CX, 12
InicialLimpiarPistas:
        MOV HistorialToros[SI], 0
        MOV HistorialVacas[SI], 0
        INC SI
        LOOP InicialLimpiarPistas
        CALL GenerarCodigo
        POP SI
        POP CX
        POP AX
        RET
IniciarPartida ENDP

; Entrada: reglas y ticks BIOS. Salida: CodigoSecreto nuevo; conserva registros.
GenerarCodigo PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    MOV AH, 00H
    INT 1AH
    MOV Semilla, DX
GenerarNuevaClave:
    XOR SI, SI
GenerarOtraFicha:
    ; Generador sencillo: semilla = semilla * 25173 + 13849.
    MOV AX, Semilla
    MOV BX, 25173
    MUL BX
    ADD AX, 13849
    MOV Semilla, AX
    ; Usar el byte alto para evitar patrones de los bits bajos.
    MOV AL, AH
    XOR AH, AH
    XOR DX, DX
    XOR BX, BX
    MOV BL, CantidadFichas
    DIV BX
    MOV AL, DL
    INC AL
    CMP PermitirRepetidos, 0
    JNE GuardarCodigo
    XOR BX, BX
RevisarCodigoRepetido:
    CMP BX, SI
    JE GuardarCodigo
    CMP AL, CodigoSecreto[BX]
    JE GenerarOtraFicha
    INC BX
    JMP RevisarCodigoRepetido
GuardarCodigo:
    MOV CodigoSecreto[SI], AL
    INC SI
    XOR AX, AX
    MOV AL, CantidadPosiciones
    CMP SI, AX
    JB GenerarOtraFicha
    ; Evitar repetir exactamente la clave de la partida anterior.
    CMP HayCodigoAnterior, 0
    JE GuardarClaveAnterior
    XOR SI, SI
CompararClaveAnterior:
    MOV AL, CodigoSecreto[SI]
    CMP AL, CodigoAnterior[SI]
    JNE GuardarClaveAnterior
    INC SI
    XOR AX, AX
    MOV AL, CantidadPosiciones
    CMP SI, AX
    JB CompararClaveAnterior
    JMP GenerarNuevaClave
GuardarClaveAnterior:
    XOR SI, SI
    MOV CX, 5
CopiarClaveAnterior:
    MOV AL, CodigoSecreto[SI]
    MOV CodigoAnterior[SI], AL
    INC SI
    LOOP CopiarClaveAnterior
    MOV HayCodigoAnterior, 1
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
GenerarCodigo ENDP

; Entrada: CodigoSecreto, IntentoActual y CantidadPosiciones.
; Salida: AciertosExactos y AciertosOtraPosicion. Conserva registros.
EvaluarIntento PROC
    PUSH AX
    PUSH CX
    PUSH SI
    PUSH DI
    MOV AciertosExactos, 0
    MOV AciertosOtraPosicion, 0
    XOR SI, SI
    MOV CX, 5
LimpiarMarcasPistas:
    MOV UsadoSecreto[SI], 0
    MOV UsadoIntento[SI], 0
    INC SI
    LOOP LimpiarMarcasPistas

    ; Primero reservar las coincidencias de ficha y posicion.
    XOR SI, SI
    XOR CX, CX
    MOV CL, CantidadPosiciones
BuscarExactos:
    MOV AL, IntentoActual[SI]
    CMP AL, CodigoSecreto[SI]
    JNE SiguienteExacto
    INC AciertosExactos
    MOV UsadoSecreto[SI], 1
    MOV UsadoIntento[SI], 1
SiguienteExacto:
    INC SI
    LOOP BuscarExactos

    ; Cada ficha restante puede contarse una sola vez.
    XOR SI, SI
BuscarOtraPosicion:
    CMP UsadoIntento[SI], 1
    JE SiguienteFichaPista
    XOR DI, DI
BuscarEnSecreto:
    CMP UsadoSecreto[DI], 1
    JE SiguienteSecreto
    MOV AL, IntentoActual[SI]
    CMP AL, CodigoSecreto[DI]
    JNE SiguienteSecreto
    INC AciertosOtraPosicion
    MOV UsadoSecreto[DI], 1
    JMP SiguienteFichaPista
SiguienteSecreto:
    INC DI
    XOR AX, AX
    MOV AL, CantidadPosiciones
    CMP DI, AX
    JB BuscarEnSecreto
SiguienteFichaPista:
    INC SI
    XOR AX, AX
    MOV AL, CantidadPosiciones
    CMP SI, AX
    JB BuscarOtraPosicion

    POP DI
    POP SI
    POP CX
    POP AX
    RET
EvaluarIntento ENDP

; Guardar el intento y las dos pistas antes de pasar de fila.
; Entrada: estado de partida. Salida: historial actualizado; conserva AX,BX,CX,SI,DI.
GuardarHistorial PROC
    PUSH DX
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH SI
    PUSH DI
    XOR AX, AX
    MOV AL, TurnoActual
    MOV SI, AX
    MOV AL, AciertosExactos
    MOV HistorialToros[SI], AL
    MOV AL, AciertosOtraPosicion
    MOV HistorialVacas[SI], AL
    MOV AX, SI
    MOV BX, 5
    MUL BX
    MOV DI, AX
    XOR SI, SI
    MOV CX, 5
CopiarIntentoHistorial:
    MOV AL, IntentoActual[SI]
    MOV Historial[DI], AL
    INC SI
    INC DI
    LOOP CopiarIntentoHistorial
    POP DI
    POP SI
    POP CX
    POP BX
    POP AX
    POP DX
    RET
GuardarHistorial ENDP

; Confirmar solo una fila completa; un rechazo no consume turno.
; Entrada: estado de partida. Salida: turno/resultado actualizados; conserva registros.
ConfirmarIntento PROC
    PUSH AX
    PUSH CX
    PUSH SI
    XOR SI, SI
    XOR CX, CX
    MOV CL, CantidadPosiciones
ValidarIntento:
    CMP IntentoActual[SI], 0
    JNE ValidarSiguiente
    CALL AvisarIncompleto
    JMP ConfirmarFin
ValidarSiguiente:
    INC SI
    LOOP ValidarIntento
    CALL EvaluarIntento
    CALL GuardarHistorial
    CALL DibujarPistas
    CALL SonidoSeleccion
    INC TurnoActual
    MOV AL, AciertosExactos
    CMP AL, CantidadPosiciones
    JNE ConfirmarQuedanIntentos
    MOV PartidaTerminada, 1
    JMP ConfirmarTermino
ConfirmarQuedanIntentos:
    MOV AL, TurnoActual
    CMP AL, IntentosMaximos
    JB ConfirmarOtraFila
    MOV PartidaTerminada, 2
ConfirmarTermino:
    CALL PantallaFin
    JMP ConfirmarFin
ConfirmarOtraFila:
    CALL BorrarCursorJuego
    ADD FilaJuego, 13
    MOV PosicionActual, 0
    XOR SI, SI
    MOV CX, 5
VaciarSiguienteIntento:
    MOV IntentoActual[SI], 0
    INC SI
    LOOP VaciarSiguienteIntento
    CALL DibujarCursorJuego
    CALL MostrarEstado
    CALL AyudaJuego
ConfirmarFin:
    POP SI
    POP CX
    POP AX
    RET
ConfirmarIntento ENDP

; Colocar en la casilla actual. Conserva AX,BX,SI.
ColocarFicha PROC
    PUSH AX
    PUSH BX
    PUSH SI
    XOR AX, AX
    MOV AL, PosicionActual
    MOV SI, AX
    CMP PermitirRepetidos, 0
    JNE ColocarGuardar
    XOR BX, BX
ColocarBuscarRepetida:
    CMP BX, SI
    JE ColocarSiguiente
    MOV AL, IntentoActual[BX]
    CMP AL, FichaSeleccionada
    JNE ColocarSiguiente
    CALL AvisarRepetida
    JMP ColocarFin
ColocarSiguiente:
    INC BL
    CMP BL, CantidadPosiciones
    JB ColocarBuscarRepetida
ColocarGuardar:
    MOV AL, FichaSeleccionada
    MOV IntentoActual[SI], AL
    CALL DibujarFichaActual
    CALL AyudaJuego
ColocarFin:
    POP SI
    POP BX
    POP AX
    RET
ColocarFicha ENDP

; BACKSPACE vacia solo la casilla actual. Conserva AX,SI.
BorrarFichaActual PROC
    PUSH AX
    PUSH SI
    XOR AX, AX
    MOV AL, PosicionActual
    MOV SI, AX
    MOV IntentoActual[SI], 0
    CALL DibujarFichaActual
    CALL DibujarCursorJuego
    CALL AyudaJuego
    POP SI
    POP AX
    RET
BorrarFichaActual ENDP

; Entrada: tecla en AX. Salida: estado de edicion; conserva registros.
EditarIntento PROC
    PUSH AX
    CMP AL, 8
    JNE EditarEnter
    CALL BorrarFichaActual
    JMP EditarFin
EditarEnter:
    CMP AL, 13
    JNE EditarDigito
    CALL ConfirmarIntento
    JMP EditarFin
EditarDigito:
    CMP AL, '1'
    JB EditarFlechas
    CMP AL, '8'
    JA EditarFlechas
    SUB AL, '0'
    CMP AL, CantidadFichas
    JA SHORT EditarFin
    MOV FichaSeleccionada, AL
    CALL RedibujarPanelFichas
    CALL ColocarFicha
    JMP SHORT EditarFin
EditarFlechas:
    CMP AH, 4BH
    JNE EditarDerecha
    CMP PosicionActual, 0
    JE EditarFin
    CALL BorrarCursorJuego
    DEC PosicionActual
    JMP EditarCursor
EditarDerecha:
    CMP AH, 4DH
    JNE EditarArriba
    MOV AL, CantidadPosiciones
    DEC AL
    CMP PosicionActual, AL
    JAE EditarFin
    CALL BorrarCursorJuego
    INC PosicionActual
EditarCursor:
    CALL DibujarCursorJuego
    JMP EditarFin
EditarArriba:
    CMP AH, 48H
    JNE EditarAbajo
    DEC FichaSeleccionada
    CMP FichaSeleccionada, 0
    JNE EditarColocar
    MOV AL, CantidadFichas
    MOV FichaSeleccionada, AL
    JMP EditarColocar
EditarAbajo:
    CMP AH, 50H
    JNE EditarFin
    INC FichaSeleccionada
    MOV AL, FichaSeleccionada
    CMP AL, CantidadFichas
    JBE EditarColocar
    MOV FichaSeleccionada, 1
EditarColocar:
    CALL RedibujarPanelFichas
    CALL ColocarFicha
EditarFin:
    POP AX
    RET
EditarIntento ENDP

; Entrada: partida inicializada. Salida: ESC/M vuelve al menu. Conserva AX.
ControlarJuego PROC
    PUSH AX
JuegoEsperar:
    CALL ActualizarSonido
    CMP PartidaTerminada, 0
    JNE JuegoAnimarFin
    CALL AnimarSeleccion
    JMP JuegoConsultar
JuegoAnimarFin:
    CALL AnimarFinal
JuegoConsultar:
    MOV AH, 01H
    INT 16H
    JNZ JuegoLeer
    JMP JuegoEsperar
JuegoLeer:
    MOV AH, 00H
    INT 16H
    CMP AL, 27
    JE JuegoSalir
    CMP PartidaTerminada, 0
    JE JuegoEditar
    CMP AL, 'm'
    JE JuegoSalir
    CMP AL, 'M'
    JE JuegoSalir
    CMP AL, 13
    JNE JuegoEsperar
    CALL IniciarPartida
    CALL PantallaJuego
    JMP JuegoEsperar
JuegoEditar:
    CALL EditarIntento
    JMP JuegoEsperar
JuegoSalir:
    CALL ApagarSonido
    POP AX
    RET
ControlarJuego ENDP

; /T es un modo de verificacion; no se ejecuta durante una partida normal.
; Entrada: cola PSP. Salida: vuelve al juego, o termina DOS con codigo de prueba.
RevisarPruebas PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    PUSH ES
    MOV AH, 62H
    INT 21H
    MOV ES, BX
    XOR CX, CX
    MOV CL, ES:[80H]
    MOV SI, 81H
PruebaBuscarParametro:
    JCXZ PruebaSinParametro
    MOV AL, ES:[SI]
    CMP AL, 'T'
    JE PruebaEjecutar
    CMP AL, 't'
    JE PruebaEjecutar
    INC SI
    LOOP PruebaBuscarParametro
PruebaSinParametro:
    POP ES
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
PruebaEjecutar:
    ; Recuperar el segmento de video, no el PSP.
    MOV AX, 0A000H
    MOV ES, AX
    CALL ProbarEvaluacion
    CALL ProbarGeneracion
    CALL ProbarValidacion
    CALL ProbarAnimacion
    CALL ProbarPartidaCompleta
    CALL CapturarNiveles
    MOV AL, ModoAnterior
    MOV AH, 00H
    INT 10H
    CMP PruebasFallidas, 0
    JNE PruebaMensajeError
    PUSH CS
    POP DS
    MOV DX, OFFSET PruebaOK
    MOV AH, 09H
    INT 21H
    MOV AX, 4C00H
    INT 21H
PruebaMensajeError:
    PUSH CS
    POP DS
    MOV DX, OFFSET PruebaError
    MOV AH, 09H
    INT 21H
    MOV AX, 4C01H
    INT 21H
PruebaOK DB 'OK: 4 casos, 3 niveles, validacion, historial, victoria y derrota.',13,10,'$'
PruebaError DB 'ERROR en las pruebas.',13,10,'$'
RevisarPruebas ENDP

; Los cuatro casos de la tabla 2 del PDF, ejecutados por la rutina real.
; Sin parametros; incrementa PruebasFallidas. Conserva registros.
ProbarEvaluacion PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH SI
    PUSH DI
    MOV CantidadPosiciones, 4
    MOV PruebasFallidas, 0
    XOR DI, DI
    MOV CX, 4
PruebaOtroCaso:
    XOR SI, SI
PruebaCopiarCaso:
    MOV BX, DI
    ADD BX, SI
    MOV AL, CasosPrueba[BX]
    MOV CodigoSecreto[SI], AL
    MOV AL, CasosPrueba[BX+4]
    MOV IntentoActual[SI], AL
    INC SI
    CMP SI, 4
    JB PruebaCopiarCaso
    CALL EvaluarIntento
    MOV AL, CasosPrueba[DI+8]
    CMP AL, AciertosExactos
    JNE PruebaCasoFallo
    MOV AL, CasosPrueba[DI+9]
    CMP AL, AciertosOtraPosicion
    JE PruebaCasoSiguiente
PruebaCasoFallo:
    INC PruebasFallidas
PruebaCasoSiguiente:
    ADD DI, 10
    LOOP PruebaOtroCaso
    POP DI
    POP SI
    POP CX
    POP BX
    POP AX
    RET
ProbarEvaluacion ENDP

; Comprueba las tres dificultades, limites y ausencia de repetidos en facil.
; Sin parametros; incrementa PruebasFallidas. Conserva registros.
ProbarGeneracion PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH SI
    MOV NivelActual, 0
PruebaOtroNivel:
    CALL IniciarPartida
    XOR SI, SI
PruebaOtraPosicion:
    MOV AL, CodigoSecreto[SI]
    CMP AL, 1
    JB PruebaGeneracionFallo
    CMP AL, CantidadFichas
    JA PruebaGeneracionFallo
    CMP NivelActual, 0
    JNE PruebaGeneracionSiguiente
    XOR BX, BX
PruebaBuscarDuplicado:
    CMP BX, SI
    JE PruebaGeneracionSiguiente
    CMP AL, CodigoSecreto[BX]
    JE PruebaGeneracionFallo
    INC BX
    JMP PruebaBuscarDuplicado
PruebaGeneracionFallo:
    INC PruebasFallidas
PruebaGeneracionSiguiente:
    INC SI
    XOR AX, AX
    MOV AL, CantidadPosiciones
    CMP SI, AX
    JB PruebaOtraPosicion
    INC NivelActual
    CMP NivelActual, 3
    JB PruebaOtroNivel
    MOV NivelActual, 0
    POP SI
    POP CX
    POP BX
    POP AX
    RET
ProbarGeneracion ENDP

; Una fila vacia no consume intento; BACKSPACE limpia solo la posicion activa.
; Sin parametros; incrementa PruebasFallidas. Conserva AX.
ProbarValidacion PROC
    PUSH AX
    CALL IniciarPartida
    CALL ConfirmarIntento
    CMP TurnoActual, 0
    JE PruebaBorrar
    INC PruebasFallidas
PruebaBorrar:
    MOV IntentoActual[0], 1
    MOV IntentoActual[1], 2
    MOV PosicionActual, 1
    CALL BorrarFichaActual
    CMP IntentoActual[1], 0
    JNE PruebaValidacionFallo
    CMP IntentoActual[0], 1
    JE PruebaValidacionFin
PruebaValidacionFallo:
    INC PruebasFallidas
PruebaValidacionFin:
    POP AX
    RET
ProbarValidacion ENDP

; Volcado de 64000 bytes para revisar el dibujo del modo /T. Conserva registros.
CapturarTablero PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH DS
    MOV DX, OFFSET NombreFacil
    CMP NivelActual, 0
    JE CapturaNombreListo
    MOV DX, OFFSET NombreNormal
    CMP NivelActual, 1
    JE CapturaNombreListo
    MOV DX, OFFSET NombreDificil
CapturaNombreListo:
    CMP PartidaTerminada, 0
    JE CapturaCrear
    MOV DX, OFFSET NombreVictoria
    CMP PartidaTerminada, 1
    JE CapturaCrear
    MOV DX, OFFSET NombreDerrota
CapturaCrear:
    PUSH CS
    POP DS
    XOR CX, CX
    MOV AH, 3CH
    INT 21H
    JC CapturaFin
    MOV BX, AX
    PUSH ES
    POP DS
    XOR DX, DX
    MOV CX, 64000
    MOV AH, 40H
    INT 21H
    MOV AH, 3EH
    INT 21H
CapturaFin:
    POP DS
    POP DX
    POP CX
    POP BX
    POP AX
    RET
NombreFacil DB 'FACIL.RAW',0
NombreNormal DB 'NORMAL.RAW',0
NombreDificil DB 'DIFICIL.RAW',0
NombreVictoria DB 'GANAR.RAW',0
NombreDerrota DB 'PERDER.RAW',0
CapturarTablero ENDP

; Simula teclas del editor y comprueba una victoria y una derrota completas.
; Sin parametros; incrementa PruebasFallidas. Conserva registros.
ProbarPartidaCompleta PROC
    PUSH AX
    PUSH CX
    PUSH SI
    MOV NivelActual, 1
    CALL IniciarPartida
    CALL PantallaJuego
    XOR SI, SI
PruebaColocarSecreto:
    MOV AX, SI
    MOV PosicionActual, AL
    MOV AL, CodigoSecreto[SI]
    ADD AL, '0'
    CALL EditarIntento
    INC SI
    CMP SI, 4
    JB PruebaColocarSecreto
    MOV AX, 000DH
    CALL EditarIntento
    CMP PartidaTerminada, 1
    JE SaltoLocalPruebas1
    JMP PruebaPartidaFallo
SaltoLocalPruebas1:
    CMP TurnoActual, 1
    JE SaltoLocalPruebas2
    JMP PruebaPartidaFallo
SaltoLocalPruebas2:
    CMP HistorialToros[0], 4
    JE SaltoLocalPruebas3
    JMP PruebaPartidaFallo
SaltoLocalPruebas3:
    CALL AnimarFinal
    CALL CapturarTablero
    ; Derrota: secreto 1111, diez intentos 2222.
    CALL IniciarPartida
    CALL PantallaJuego
    MOV CodigoSecreto[0], 1
    MOV CodigoSecreto[1], 1
    MOV CodigoSecreto[2], 1
    MOV CodigoSecreto[3], 1
    MOV CX, 10
PruebaIntentoFallido:
    MOV IntentoActual[0], 2
    MOV IntentoActual[1], 2
    MOV IntentoActual[2], 2
    MOV IntentoActual[3], 2
    CALL ConfirmarIntento
    LOOP PruebaIntentoFallido
    CMP PartidaTerminada, 2
    JE SaltoLocalPruebas4
    JMP PruebaPartidaFallo
SaltoLocalPruebas4:
    CMP TurnoActual, 10
    JE SaltoLocalPruebas5
    JMP PruebaPartidaFallo
SaltoLocalPruebas5:
    CMP Historial[45], 2
    JE SaltoLocalPruebas6
    JMP PruebaPartidaFallo
SaltoLocalPruebas6:
    CMP HistorialToros[9], 0
    JE SaltoLocalPruebas7
    JMP PruebaPartidaFallo
SaltoLocalPruebas7:
    CALL AnimarFinal
    CALL CapturarTablero
    JMP PruebaPartidaFin
PruebaPartidaFallo:
    INC PruebasFallidas
PruebaPartidaFin:
    POP SI
    POP CX
    POP AX
    RET
ProbarPartidaCompleta ENDP

; Tres capturas con las dimensiones reales de cada nivel. Conserva registros.
CapturarNiveles PROC
    PUSH AX
    MOV NivelActual, 0
PruebaCapturarNivel:
    CALL IniciarPartida
    CALL PantallaJuego
    CALL CapturarTablero
    INC NivelActual
    CMP NivelActual, 3
    JB PruebaCapturarNivel
    MOV NivelActual, 0
    POP AX
    RET
CapturarNiveles ENDP

; Simula un intervalo vencido y comprueba el cambio real de cuadro.
; Sin parametros; incrementa PruebasFallidas. Conserva registros.
ProbarAnimacion PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    MOV AH, 00H
    INT 1AH
    MOV TickAnimacion, DX
    SUB TickAnimacion, 4
    MOV BL, CuadroAnimacion
    XOR BL, 1
    CALL AvanzarAnimacion
    CMP BL, CuadroAnimacion
    JE PruebaAnimacionFin
    INC PruebasFallidas
PruebaAnimacionFin:
    POP DX
    POP CX
    POP BX
    POP AX
    RET
ProbarAnimacion ENDP

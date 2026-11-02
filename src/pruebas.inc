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
    MOV ModoPruebas, 1

    MOV AX, 0A000H
    MOV ES, AX
    CALL ProbarEvaluacion
    CALL ProbarGeneracion
    CALL ProbarValidacion
    CALL ProbarAnimacion
    CALL ProbarTransiciones
    CALL ProbarTiempo
    CALL ProbarRanking
    CALL ProbarArchivoRecords
    CALL ProbarPartidaCompleta
    CALL ProbarDuelo
    CALL ProbarComparacionDuelo
    CALL CapturarNiveles
    CALL CapturarVistas
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
PruebaOK DB 'OK: 4 casos, 3 niveles, validacion, historial, victoria, derrota, tiempo, records, transiciones y duelo.',13,10,'$'
PruebaError DB 'ERROR en las pruebas.',13,10,'$'
RevisarPruebas ENDP

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

ProbarTiempo PROC
    PUSH AX
    PUSH CX
    PUSH DX
    MOV TicksPartida, 182
    MOV TicksPartida+2, 0
    CALL TicksASegundos
    CMP AX, 10
    JNE PruebaTiempoFallo
    MOV TicksPartida, 18
    CALL TicksASegundos
    CMP AX, 0
    JNE PruebaTiempoFallo
    MOV TicksPartida+2, 2
    CALL TicksASegundos
    CMP AX, 5999
    JNE PruebaTiempoFallo
    MOV TicksPartida, 0
    MOV TicksPartida+2, 0
    MOV TickReloj, 00A0H
    MOV TickReloj+2, 0018H
    XOR CX, CX
    MOV DX, 5
    CALL AcumularTicks
    CMP TicksPartida, 21
    JNE PruebaTiempoFallo
    CMP TicksPartida+2, 0
    JNE PruebaTiempoFallo
    MOV TicksPartida, 0
    MOV TicksPartida+2, 0
    MOV TickReloj, 0FFF0H
    MOV TickReloj+2, 0
    MOV CX, 1
    MOV DX, 4
    CALL AcumularTicks
    CMP TicksPartida, 20
    JNE PruebaTiempoFallo
    CMP TicksPartida+2, 0
    JNE PruebaTiempoFallo

    CALL IniciarCronometro
    MOV TicksPartida, 182
    CALL ActualizarCronometro
    MOV AX, SegundosJuego
    CMP AX, 10
    JB PruebaTiempoFallo
    CMP AX, 11
    JA PruebaTiempoFallo
    CMP AX, SegundosMostrados
    JNE PruebaTiempoFallo
    MOV RelojActivo, 0
    MOV SegundosJuego, 123
    CALL ActualizarCronometro
    CMP SegundosJuego, 123
    JE PruebaTiempoFin
PruebaTiempoFallo:
    INC PruebasFallidas
PruebaTiempoFin:
    POP DX
    POP CX
    POP AX
    RET
ProbarTiempo ENDP

ProbarRanking PROC
    PUSH AX
    CALL VaciarRecords
    MOV NivelActual, 0
    MOV TurnoActual, 4
    MOV SegundosJuego, 90
    CALL InsertarRecord
    MOV TurnoActual, 3
    MOV SegundosJuego, 120
    CALL InsertarRecord
    MOV TurnoActual, 4
    MOV SegundosJuego, 60
    CALL InsertarRecord
    MOV SegundosJuego, 75
    CALL InsertarRecord
    CMP Records[0], 3
    JNE PruebaRankingFallo
    CMP WORD PTR Records[1], 120
    JNE PruebaRankingFallo
    CMP Records[3], 4
    JNE PruebaRankingFallo
    CMP WORD PTR Records[4], 60
    JNE PruebaRankingFallo
    CMP WORD PTR Records[7], 75
    JNE PruebaRankingFallo
    MOV TurnoActual, 5
    MOV SegundosJuego, 1
    CALL InsertarRecord
    CMP NuevoRecord, 0
    JNE PruebaRankingFallo
    MOV NivelActual, 1
    MOV TurnoActual, 2
    MOV SegundosJuego, 180
    CALL InsertarRecord
    CMP Records[9], 2
    JNE PruebaRankingFallo
    CMP Records[0], 3
    JNE PruebaRankingFallo

    MOV PartidaTerminada, 1
    MOV TurnoActual, 1
    CALL RegistrarVictoria
    CMP Records[9], 2
    JE PruebaRankingFin
PruebaRankingFallo:
    INC PruebasFallidas
PruebaRankingFin:
    POP AX
    RET
ProbarRanking ENDP

ProbarArchivoRecords PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    MOV RutaRecords, OFFSET NombreRecordsPrueba
    CALL GuardarRecords
    CMP EstadoArchivo, 0
    JNE PruebaArchivoFallo
    CALL VaciarRecords
    CALL CargarRecords
    CMP EstadoArchivo, 0
    JNE PruebaArchivoFallo
    CMP Records[0], 3
    JNE PruebaArchivoFallo
    CMP WORD PTR Records[4], 60
    JNE PruebaArchivoFallo
    CMP Records[9], 2
    JNE PruebaArchivoFallo

    MOV ModoPruebas, 0
    MOV NivelActual, 1
    MOV TurnoActual, 1
    MOV SegundosJuego, 20
    MOV PartidaTerminada, 1
    MOV ClaveManual, 1
    CALL RegistrarVictoria
    CMP Records[9], 2
    JNE PruebaArchivoFallo
    MOV ClaveManual, 0
    MOV PartidaTerminada, 2
    CALL RegistrarVictoria
    CMP Records[9], 2
    JNE PruebaArchivoFallo
    MOV PartidaTerminada, 1
    CALL RegistrarVictoria
    MOV ModoPruebas, 1
    CMP NuevoRecord, 1
    JNE PruebaArchivoFallo
    CALL CargarRecords
    CMP Records[9], 1
    JNE PruebaArchivoFallo
    CMP WORD PTR Records[10], 20
    JNE PruebaArchivoFallo

    MOV ModoPruebas, 0
    MOV ModoMulti, 1
    MOV SegundosJuego, 5
    MOV NuevoRecord, 0
    CALL RegistrarVictoria
    MOV ModoMulti, 0
    MOV ModoPruebas, 1
    CMP NuevoRecord, 0
    JNE PruebaArchivoFallo
    CALL CargarRecords
    CMP WORD PTR Records[10], 20
    JNE PruebaArchivoFallo
    CALL PantallaRecords
    CALL CapturarPuntajes

    MOV RutaRecords, OFFSET NombreRecordsBloqueado
    CALL GuardarRecords
    CMP EstadoArchivo, 2
    JNE PruebaArchivoFallo
    MOV RutaRecords, OFFSET NombreRecordsPrueba

    MOV DX, RutaRecords
    XOR CX, CX
    MOV AH, 3CH
    INT 21H
    JC PruebaArchivoFallo
    MOV BX, AX
    MOV CX, 5
    MOV DX, OFFSET BufferRecords
    MOV AH, 40H
    INT 21H
    MOV AH, 3EH
    INT 21H
    CALL CargarRecords
    CMP EstadoArchivo, 1
    JNE PruebaArchivoFallo
    CMP Records[0], 255
    JE PruebaArchivoLimpiar
PruebaArchivoFallo:
    INC PruebasFallidas
PruebaArchivoLimpiar:
    MOV ModoMulti, 0
    MOV ModoPruebas, 1
    MOV DX, OFFSET NombreRecordsPrueba
    MOV AH, 41H
    INT 21H
    MOV RutaRecords, OFFSET NombreRecords
    CALL VaciarRecords
    MOV EstadoArchivo, 0
    MOV NivelActual, 0
    POP DX
    POP CX
    POP BX
    POP AX
    RET
ProbarArchivoRecords ENDP

CapturarPuntajes PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH DS
    PUSH CS
    POP DS
    MOV DX, OFFSET NombrePuntajes
    XOR CX, CX
    MOV AH, 3CH
    INT 21H
    JC PuntajeCapturaFin
    MOV BX, AX
    PUSH ES
    POP DS
    XOR DX, DX
    MOV CX, 64000
    MOV AH, 40H
    INT 21H
    MOV AH, 3EH
    INT 21H
PuntajeCapturaFin:
    POP DS
    POP DX
    POP CX
    POP BX
    POP AX
    RET
NombrePuntajes DB 'PUNTOS.RAW',0
CapturarPuntajes ENDP

CAPTURAR_VISTA MACRO nombre
    LOCAL ruta, continuar
    JMP NEAR PTR continuar
ruta DB nombre,0
continuar:
    PUSH DS
    PUSH DX
    PUSH CS
    POP DS
    MOV DX, OFFSET ruta
    CALL GuardarVista
    POP DX
    POP DS
ENDM

GuardarVista PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH DS
    XOR CX, CX
    MOV AH, 3CH
    INT 21H
    JC GuardarVistaFin
    MOV BX, AX
    PUSH ES
    POP DS
    XOR DX, DX
    MOV CX, 64000
    MOV AH, 40H
    INT 21H
    MOV AH, 3EH
    INT 21H
GuardarVistaFin:
    POP DS
    POP DX
    POP CX
    POP BX
    POP AX
    RET
GuardarVista ENDP

CapturarVistas PROC
    PUSH AX
    CALL PantallaInicio
    CAPTURAR_VISTA 'PORTADA.RAW'
    CALL PantallaMenu
    CAPTURAR_VISTA 'MENU.RAW'

    CALL BorrarSelector
    MOV OpcionMenu, 2
    CALL DibujarSelector
    CAPTURAR_VISTA 'MENU2.RAW'
    MOV OpcionMenu, 0
    CALL PantallaDificultad
    CAPTURAR_VISTA 'NIVEL.RAW'
    CALL BorrarSelectorDificultad
    MOV OpcionDificultad, 2
    CALL DibujarSelectorDificultad
    CAPTURAR_VISTA 'NIVEL2.RAW'
    MOV OpcionDificultad, 0
    CALL PantallaInstrucciones
    CAPTURAR_VISTA 'AYUDA.RAW'
    CALL PantallaCreditos
    CAPTURAR_VISTA 'CREDITOS.RAW'
    CALL CapturarEditores
    POP AX
    RET
CapturarVistas ENDP

CapturarEditores PROC
    PUSH AX
    PUSH SI
    CALL IniciarPartida
    MOV ClaveManual, 1
    MOV PermitirRepetidos, 1
    CALL DibujarEditorClave
    MOV CodigoSecreto[0], 1
    MOV CodigoSecreto[1], 2
    MOV CodigoSecreto[2], 3
    MOV SI, 3
    CALL MostrarClaveManual
    CAPTURAR_VISTA 'PRUEBA.RAW'
    CALL CerrarEstilo
    MOV ClaveManual, 0
    MOV PermitirRepetidos, 0
    CALL DibujarEditorClave
    CALL MostrarClaveManual
    CAPTURAR_VISTA 'INVERSO.RAW'
    CALL CerrarEstilo
    POP SI
    POP AX
    RET
CapturarEditores ENDP

ProbarTransiciones PROC
    PUSH AX
    PUSH CX
    PUSH DX
    PUSH SI
    MOV ModoPruebas, 0
    CALL PantallaMenu
    CALL IniciarTransicion
    CMP BrilloActual, 0
    JNE PruebaFundidoFallo
    MOV DX, 03C7H
    XOR AL, AL
    OUT DX, AL
    ADD DX, 2
    MOV CX, 81
PruebaFundidoNegro:
    IN AL, DX
    CMP AL, 0
    JNE PruebaFundidoFallo
    LOOP PruebaFundidoNegro
    CALL TerminarTransicion
    CMP BrilloActual, 16
    JNE PruebaFundidoFallo
    MOV DX, 03C7H
    XOR AL, AL
    OUT DX, AL
    ADD DX, 2
    MOV SI, OFFSET Paleta
    MOV CX, 81
PruebaFundidoOriginal:
    IN AL, DX
    CMP AL, [SI]
    JNE PruebaFundidoFallo
    INC SI
    LOOP PruebaFundidoOriginal
    MOV BrilloActual, 8
    CALL AplicarBrillo

    MOV DX, 03C7H
    MOV AL, 3
    OUT DX, AL
    ADD DX, 2
    MOV CX, 3
PruebaFundidoMedio:
    IN AL, DX
    CMP AL, 31
    JNE PruebaFundidoFallo
    LOOP PruebaFundidoMedio
    JMP PruebaFundidoFin
PruebaFundidoFallo:
    INC PruebasFallidas
PruebaFundidoFin:
    MOV BrilloActual, 16
    CALL AplicarBrillo
    MOV ModoPruebas, 1
    POP SI
    POP DX
    POP CX
    POP AX
    RET
ProbarTransiciones ENDP

EncolarVictoriaMulti PROC
    PUSH AX
    PUSH BX
    PUSH CX
    CALL VaciarTeclasMulti
    MOV BX, 3
PruebaMultiEncolarCasilla:
    MOV CX, 0231H
    MOV AH, 05H
    INT 16H
    CMP AL, 0
    JNE PruebaMultiTecladoFallo
    MOV CX, 4D00H
    MOV AH, 05H
    INT 16H
    CMP AL, 0
    JNE PruebaMultiTecladoFallo
    DEC BX
    JNZ PruebaMultiEncolarCasilla
    MOV CX, 0231H
    MOV AH, 05H
    INT 16H
    CMP AL, 0
    JNE PruebaMultiTecladoFallo
    MOV CX, 1C0DH
    MOV AH, 05H
    INT 16H
    CMP AL, 0
    JNE PruebaMultiTecladoFallo
    POP CX
    POP BX
    POP AX
    CLC
    RET
PruebaMultiTecladoFallo:
    POP CX
    POP BX
    POP AX
    STC
    RET
EncolarVictoriaMulti ENDP

ProbarDuelo PROC
    PUSH AX
    PUSH CX
    PUSH SI
    MOV NivelActual, 0
PruebaMultiOtroNivel:
    MOV ModoMulti, 0
    CALL IniciarPartida
    CALL GuardarClaveMulti
    MOV ModoMulti, 1
    MOV CodigoSecreto[0], 0
    CALL IniciarPartida
    XOR SI, SI
PruebaMultiMismaClave:
    MOV AL, CodigoSecreto[SI]
    CMP AL, ClaveMulti[SI]
    JNE PruebaMultiFallo
    CMP IntentoActual[SI], 0
    JNE PruebaMultiFallo
    INC SI
    CMP SI, 5
    JB PruebaMultiMismaClave
    INC NivelActual
    CMP NivelActual, 3
    JB PruebaMultiOtroNivel

    MOV NivelActual, 1
    MOV ClaveMulti[0], 1
    MOV ClaveMulti[1], 1
    MOV ClaveMulti[2], 1
    MOV ClaveMulti[3], 1
    MOV JugadorMulti, 0
PruebaMultiTurno:
    CALL IniciarPartida
    CALL PantallaJuego
    CALL EncolarVictoriaMulti
    JC PruebaMultiFallo
    CALL ControlarTurnoMulti
    JC PruebaMultiFallo
    CMP PartidaTerminada, 1
    JNE PruebaMultiFallo
    CMP RelojActivo, 0
    JNE PruebaMultiFallo
    CMP NuevoRecord, 0
    JNE PruebaMultiFallo
    CMP JugadorMulti, 0
    JNE PruebaMultiDosTermino
    CMP ResultadoMulti[0], 1
    JNE PruebaMultiFallo
    CMP IntentosMulti[0], 1
    JNE PruebaMultiFallo
    CAPTURAR_VISTA 'DUELJ1.RAW'
    INC JugadorMulti
    CALL PantallaTurnoMulti
    CAPTURAR_VISTA 'TURNO2.RAW'
    JMP PruebaMultiTurno
PruebaMultiDosTermino:
    CMP ResultadoMulti[1], 1
    JNE PruebaMultiFallo
    CMP IntentosMulti[1], 1
    JNE PruebaMultiFallo
    MOV TiempoMulti[0], 90
    MOV TiempoMulti[2], 90
    CALL CompararDuelo
    CMP GanadorMulti, 0
    JNE PruebaMultiFallo
    CALL PantallaDuelo
    CAPTURAR_VISTA 'DUELFIN.RAW'

    CALL IniciarPartida
    CALL PantallaJuego
    CALL VaciarTeclasMulti
    MOV CX, 011BH
    MOV AH, 05H
    INT 16H
    CMP AL, 0
    JNE PruebaMultiFallo
    CALL ControlarTurnoMulti
    JNC PruebaMultiFallo
    CMP RelojActivo, 0
    JNE PruebaMultiFallo
    CMP PartidaTerminada, 0
    JNE PruebaMultiFallo
    JMP PruebaMultiFin
PruebaMultiFallo:
    INC PruebasFallidas
PruebaMultiFin:
    MOV ModoMulti, 0
    MOV JugadorMulti, 0
    MOV NivelActual, 0
    MOV RelojActivo, 0
    CALL VaciarTeclasMulti
    POP SI
    POP CX
    POP AX
    RET
ProbarDuelo ENDP

ComprobarGanadorMulti PROC
    CALL CompararDuelo
    CMP AL, GanadorMulti
    JE PruebaGanadorCorrecto
    INC PruebasFallidas
PruebaGanadorCorrecto:
    RET
ComprobarGanadorMulti ENDP

ProbarComparacionDuelo PROC
    PUSH AX
    MOV ResultadoMulti[0], 1
    MOV ResultadoMulti[1], 1
    MOV IntentosMulti[0], 3
    MOV IntentosMulti[1], 4
    MOV TiempoMulti[0], 300
    MOV TiempoMulti[2], 1
    MOV AL, 1
    CALL ComprobarGanadorMulti
    MOV IntentosMulti[0], 4
    MOV IntentosMulti[1], 3
    MOV AL, 2
    CALL ComprobarGanadorMulti
    MOV IntentosMulti[0], 3
    MOV TiempoMulti[0], 100
    MOV TiempoMulti[2], 90
    MOV AL, 2
    CALL ComprobarGanadorMulti
    MOV TiempoMulti[0], 80
    MOV AL, 1
    CALL ComprobarGanadorMulti
    MOV TiempoMulti[0], 90
    XOR AL, AL
    CALL ComprobarGanadorMulti
    MOV ResultadoMulti[1], 2
    MOV AL, 1
    CALL ComprobarGanadorMulti
    MOV ResultadoMulti[0], 2
    MOV ResultadoMulti[1], 1
    MOV AL, 2
    CALL ComprobarGanadorMulti
    MOV ResultadoMulti[1], 2
    XOR AL, AL
    CALL ComprobarGanadorMulti
    POP AX
    RET
ProbarComparacionDuelo ENDP

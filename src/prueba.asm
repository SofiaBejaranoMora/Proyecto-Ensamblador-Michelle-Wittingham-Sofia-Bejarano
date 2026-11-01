.MODEL SMALL
JUMPS ; TASM amplía los saltos condicionales que exceden el alcance 8086.
.STACK 100H

INCLUDE macros.inc

.DATA
ModoAnterior DB ?
OpcionMenu DB 0
InicioSonido DW 0
SonidoActivo DB 0
NivelActual DB 0
OpcionDificultad DB 0


; Reglas de la partida.
CantidadPosiciones DB 4
CantidadFichas DB 6
IntentosMaximos DB 12
PermitirRepetidos DB 0

; Estado de la partida.
TurnoActual DB 0
PosicionActual DB 0
FichaSeleccionada DB 1
CoordenadaFicha DW 0
FilaJuego DW 24
PartidaTerminada DB 0
AciertosExactos DB 0
AciertosOtraPosicion DB 0
Semilla DW 1
UsadoSecreto DB 5 DUP(0)
UsadoIntento DB 5 DUP(0)

; Combinaciones.
CodigoSecreto DB 5 DUP(0)
IntentoActual DB 5 DUP(0)
Historial DB 60 DUP(0) ; 12 intentos, cinco espacios por intento.
HistorialToros DB 12 DUP(0)
HistorialVacas DB 12 DUP(0)
CodigoAnterior DB 5 DUP(0)
HayCodigoAnterior DB 0
ClaveManual DB 0
TickAnimacion DW 0
CuadroAnimacion DB 0
; Cero conserva transparencia; otro indice rellena el fondo al animar.
FondoSprite DB 0
PruebasFallidas DB 0

INCLUDE paleta.inc
INCLUDE figuras.inc
INCLUDE cuadros.inc
Digitos DB 7,5,5,5,7, 2,6,2,2,7, 7,1,7,4,7, 7,1,7,1,7
        DB 5,5,7,1,1, 7,4,7,1,7, 7,4,7,5,7, 7,1,1,1,1
        DB 7,5,7,5,7, 7,5,7,1,7
CasosPrueba DB 1,2,3,4, 1,3,2,4, 2,2
            DB 1,1,2,3, 1,2,1,1, 1,2
            DB 1,2,3,4, 5,6,5,6, 0,0
            DB 3,3,3,3, 3,3,1,1, 2,0
; Una tabla permite dibujar cualquier ficha con la misma rutina.
Fichas DW OFFSET SpriteGato, OFFSET SpriteCalabaza, OFFSET SpriteCaldero
       DW OFFSET SpriteDulce, OFFSET SpriteSombrero, OFFSET SpriteCuervo
       DW OFFSET SpriteBuho, OFFSET SpriteMolly
FichasAnimadas DW OFFSET SpriteGato2, OFFSET SpriteCalabaza2, OFFSET SpriteCaldero2
       DW OFFSET SpriteDulce2, OFFSET SpriteSombrero2, OFFSET SpriteCuervo2
       DW OFFSET SpriteBuho2, OFFSET SpriteMolly2
INCLUDE inicio.inc
INCLUDE menuDato.inc

.CODE
INCLUDE video.inc
INCLUDE pantalla.inc
INCLUDE menu.inc
INCLUDE dific.inc
INCLUDE audio.inc
INCLUDE tablero.inc
INCLUDE instruc.inc
INCLUDE credit.inc
INCLUDE juego.inc
INCLUDE interfaz.inc
INCLUDE animar.inc
INCLUDE pruebas.inc

MAIN PROC
    MOV AX, @DATA
    MOV DS, AX
    CLD
    ; Guardar el modo original antes de activar graficos.
    MOV AH, 0FH
    INT 10H
    MOV ModoAnterior, AL
    MOV AX, 0013H
    INT 10H
    CALL CargarPaleta
    MOV AX, 0A000H
    MOV ES, AX
    ; CALL LimpiarPantalla
    ; /T ejecuta pruebas automaticas del algoritmo y vuelve a DOS.
    CALL RevisarPruebas
    CALL PantallaInicio
    EsperarInicio:
        CALL AnimarPortada
        MOV AH, 01H
        INT 16H
        JZ EsperarInicio
        MOV AH, 00H
        INT 16H
        CMP AL, 13
        JE AbrirMenu
        CMP AL, 27
        JNE EsperarInicio
        JMP Terminar

    AbrirMenu:
        ;CALL LimpiarPantalla
        CALL PantallaMenu
    EsperarMenu:
        CALL ActualizarSonido
        MOV AH, 01H
        INT 16H
        JZ EsperarMenu
        MOV AH, 10H
        INT 16H
        CMP AL, 27
        JNE RevisarTeclaMenu
        JMP Terminar
    RevisarTeclaMenu:
        CMP AH, 3CH
        JNE RevisarAbajoMenu
        CALL IniciarPartida
        CALL IntroducirCodigo
        JC ActualizarMenuManual
        CALL PantallaJuego
        CALL ControlarJuego
    ActualizarMenuManual:
        JMP ActualizarMenu
    RevisarAbajoMenu:
        CMP AH, 50H
        JNE RevisarArriba
        JMP MenuAbajo
    RevisarArriba:
        CMP AH, 48H
        JNE RevisarEnterMenu
        JMP MenuArriba
    RevisarEnterMenu:
        CMP AL, 13
        JE ElegirOpcion
        JMP EsperarMenu
    ElegirOpcion:
        CMP OpcionMenu, 0
        JE AbrirJuego
        CMP OpcionMenu, 1
        JE AbrirDificultad
        CMP OpcionMenu, 2
        JE AbrirInstrucciones
        CMP OpcionMenu, 3
        JE AbrirCreditos
        JMP Terminar
    AbrirJuego:
        CALL IniciarPartida
        CALL PantallaJuego
        CALL ControlarJuego
        JMP ActualizarMenu
    AbrirDificultad:
        CALL SeleccionarDificultad
        JMP ActualizarMenu
    AbrirInstrucciones:
        CALL PantallaInstrucciones
        JMP VistaAbierta
    AbrirCreditos:
        CALL PantallaCreditos
        JMP VistaAbierta
    VistaAbierta:
        CALL SonidoSeleccion
    EsperarVista:
        CALL ActualizarSonido
        MOV AH, 01H
        INT 16H
        JZ EsperarVista
        MOV AH, 00H
        INT 16H
        CMP AL, 27
        JE SalirVista
        JMP EsperarVista
    SalirVista:
        CALL SonidoSeleccion
        JMP ActualizarMenu
    MenuArriba:
        CALL SonidoSeleccion
        CALL BorrarSelector
        CMP OpcionMenu, 0
        JE VolverASalir
        DEC OpcionMenu
        JMP ActualizarSelector
    VolverASalir:
        MOV OpcionMenu, 4
        JMP ActualizarSelector
    MenuAbajo:
        CALL SonidoSeleccion
        ; Borrar antes de cambiar OpcionMenu.
        CALL BorrarSelector
        CMP OpcionMenu, 4
        JE VolverAJugar
        INC OpcionMenu
        JMP ActualizarSelector
    VolverAJugar:
        MOV OpcionMenu, 0
    ActualizarSelector:
        CALL DibujarSelector
        JMP EsperarMenu
        ; Redibujar todo solo al regresar de otra vista.
    ActualizarMenu:
        CALL PantallaMenu
        JMP EsperarMenu
    Terminar:
        CALL ApagarSonido
        MOV AL, ModoAnterior
        MOV AH, 00H
        INT 10H
        MOV AX, 4C00H
        INT 21H
MAIN ENDP
END MAIN

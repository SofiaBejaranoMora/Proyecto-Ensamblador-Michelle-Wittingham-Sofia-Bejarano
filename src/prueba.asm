.MODEL SMALL
.STACK 100H

INCLUDE macros.inc

.DATA
ModoAnterior DB ?
OpcionMenu DB 0

INCLUDE paleta.inc
INCLUDE figuras.inc
INCLUDE inicio.inc
INCLUDE menuDato.inc

.CODE
INCLUDE video.inc
INCLUDE pantalla.inc
INCLUDE menu.inc

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
    CALL LimpiarPantalla
    CALL PantallaInicio

    EsperarInicio:
        MOV AH, 00H
        INT 16H

        CMP AL, 13
        JE AbrirMenu

        CMP AL, 27
        JE Terminar

        JMP EsperarInicio

    AbrirMenu:
        CALL LimpiarPantalla
        CALL PantallaMenu

    EsperarMenu:
        MOV AH, 00H
        INT 16H

        CMP AL, 27
        JE Terminar

        CMP AH, 50H
        JE MenuAbajo

        ; Detectar flecha arriba.
        CMP AH, 48H
        JE MenuArriba

        JMP EsperarMenu

    MenuArriba:
        ; Borrar el selector de la posicion actual.
        CALL BorrarSelector

        ; Comprobar si estamos en JUGAR.
        CMP OpcionMenu, 0
        JE VolverASalir

        ; Pasar a la opcion anterior.
        DEC OpcionMenu
        JMP ActualizarMenu

    VolverASalir:
        MOV OpcionMenu, 4
        JMP ActualizarMenu

    MenuAbajo:
        CALL BorrarSelector

        CMP OpcionMenu, 4
        JE VolverAJugar

        INC OpcionMenu
        JMP ActualizarMenu

    VolverAJugar:
        MOV OpcionMenu, 0

    ActualizarMenu:
        CALL PantallaMenu
        JMP EsperarMenu

    Terminar:
        MOV AL, ModoAnterior
        MOV AH, 00H
        INT 10H
        MOV AX, 4C00H
        INT 21H
MAIN ENDP

END MAIN

# Cronometro y mejores puntajes

## Controles

El tablero muestra TIEMPO MM:SS. F3 abre la tabla desde el menu y el resultado;
Esc vuelve. Enter en el resultado reinicia tambien el reloj.

## Modulos

- `tiempo.inc`: lee CX:DX del BIOS, acumula diferencias de ticks de 32 bits,
  corrige medianoche y convierte a segundos. Solo actualiza el texto al cambiar
  el segundo. Se usa 182 ticks por diez segundos y se limita a 99:59.
- `records.inc`: tres puestos por dificultad, con intentos como primer criterio
  y segundos como desempate. Usa INT 21h 3Ch, 3Dh, 3Eh, 3Fh y 40h.

## Persistencia

`RECORDS.DAT` se guarda en el directorio de trabajo de DOS (bin si se usa la
configuracion incluida). Formato: CE01 (4 bytes) + 9 entradas de 3 bytes.
Cada entrada contiene intentos (1 byte) y segundos (2 bytes little-endian).
Las entradas vacias contienen FF FF FF. No se guardan derrotas ni claves manuales.
No existe todavia un campo de nombre de jugador.

Si falta el archivo se inicia una tabla vacia. El lector comprueba longitud,
firma y limites de intentos/segundos. Un fallo de escritura se informa y se
conserva la marca en memoria durante esa ejecucion.

## Pruebas

El modo /T comprueba conversion, limite, medianoche, acarreo de ticks,
actualizacion y detencion del reloj, insercion/desempate, aislamiento de niveles,
lectura/escritura real, rechazo de archivo truncado y escritura denegada en Z:.
Tambien comprueba que RegistrarVictoria excluya claves manuales, pruebas y derrotas.
El archivo utilizado es PRUEBREC.TMP en la copia temporal de compilacion;
las pruebas no abren ni modifican RECORDS.DAT.

Ver bin/BUILD.LOG y bin/TESTS.LOG para los resultados de la ultima compilacion.

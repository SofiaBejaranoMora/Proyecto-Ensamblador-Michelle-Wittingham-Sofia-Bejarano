param(
    [string]$DosBox = "C:\Users\michw\OneDrive\Desktop\Ensamblador IDE\resources\tools\dosbox\DOSBox.exe",
    [string]$Tasm = 'C:\tasm',
    [switch]$Pruebas
)
$ErrorActionPreference = 'Stop'
$repo = Split-Path $PSScriptRoot -Parent
if (!(Test-Path -LiteralPath $DosBox)) { throw 'Indica -DosBox con la ruta de DOSBox.exe.' }
if (!(Test-Path -LiteralPath (Join-Path $Tasm 'TASM.EXE'))) { throw 'Indica -Tasm con la carpeta de TASM y TLINK.' }
$trabajo = Join-Path ([IO.Path]::GetTempPath()) ('clave-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $trabajo | Out-Null
Copy-Item (Join-Path $repo 'src\*.inc'),(Join-Path $repo 'src\prueba.asm') -Destination $trabajo
Copy-Item (Join-Path $Tasm 'TASM.EXE'),(Join-Path $Tasm 'TLINK.EXE') -Destination $trabajo
$test = if ($Pruebas) { 'prueba.exe /T > tests.log' } else { '' }
$config = @"
[sdl]
output=surface
[cpu]
cycles=20000
[autoexec]
mount c "$trabajo"
c:
tasm /m5 /w2 prueba.asm > build.log
tlink prueba.obj > link.log
$test
exit
"@
$conf = Join-Path $trabajo 'build.conf'
Set-Content -LiteralPath $conf -Value $config -Encoding ascii
Start-Process -FilePath $DosBox -ArgumentList @('-conf',('"'+$conf+'"'),'-noconsole') -WindowStyle Hidden -Wait
$ensamblado = Get-Content (Join-Path $trabajo 'BUILD.LOG') -Raw
$enlace = Get-Content (Join-Path $trabajo 'LINK.LOG') -Raw
Write-Output $ensamblado
Write-Output $enlace
if ($ensamblado -match '\*\*(Error|Fatal)\*\*' -or !(Test-Path (Join-Path $trabajo 'PRUEBA.EXE'))) { throw "Fallo al compilar. Registros: $trabajo" }
$destino = Join-Path $repo 'bin'
New-Item -ItemType Directory -Path $destino -Force | Out-Null
Copy-Item (Join-Path $trabajo 'PRUEBA.EXE') (Join-Path $destino 'CLAVE.EXE') -Force
Copy-Item (Join-Path $trabajo 'BUILD.LOG'),(Join-Path $trabajo 'LINK.LOG') -Destination $destino -Force
if ($Pruebas) {
    $resultado = Get-Content (Join-Path $trabajo 'TESTS.LOG') -Raw
    Write-Output $resultado
    Copy-Item (Join-Path $trabajo 'TESTS.LOG') -Destination $destino -Force
    if ($resultado -notmatch 'OK:') { throw "Pruebas fallidas: $trabajo" }
    Copy-Item (Join-Path $trabajo '*.RAW') -Destination $destino -Force
}
Write-Output "Ejecutable: $destino\CLAVE.EXE"

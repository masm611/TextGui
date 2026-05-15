param(
  [string[]]$Envs = @('py38','py39','py310','py311','py312','py313','py314'),
  [string]$CMakeConfig = 'Release',
  [string]$BuildDir = 'build',
  [string]$DistDir = 'pybindDist',
  [string]$CondaPath = ''
)

$ErrorActionPreference = 'Stop'

$conda = "conda"

foreach ($envName in $Envs) {
  if (Test-Path -LiteralPath $BuildDir) {
    Remove-Item -LiteralPath $BuildDir -Recurse -Force
  }
  New-Item -ItemType Directory -Path $BuildDir | Out-Null

  & $conda run -n $envName --no-capture-output cmake -S . -B $BuildDir -DTEXTGUI_BUILD_PYTHON_BINDINGS=ON
  & $conda run -n $envName --no-capture-output cmake --build $BuildDir --config $CMakeConfig

  $src = Join-Path -Path $BuildDir -ChildPath "pybind\\$CMakeConfig\\*.pyd"
  if (-not (Test-Path -LiteralPath $DistDir)) {
    New-Item -ItemType Directory -Path $DistDir | Out-Null
  }
  Copy-Item -Path $src -Destination $DistDir -Force
}

set Config=%1

IF NOT EXIST ".\build" (
mkdir .\build
)

IF NOT EXIST ".\build\%Config%" (
mkdir .\build\%Config%
)

@ECHO OFF

PUSHD %~dp0

IF NOT DEFINED VSCMD_VER (
  ECHO Opening "Developer Command Prompt for VS"...

  CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
  IF %ERRORLEVEL% NEQ 0 (
    ECHO Visual Studio is not configured correctly.
    GOTO :ERROR
  )
)

ECHO Compiling solution...

MSBUILD /t:Build /p:Configuration=Release /p:Platform=x64 main.sln

IF %ERRORLEVEL% NEQ 0 (
  ECHO Compilation failed.
  GOTO :ERROR
)

ECHO Solution compiled.

FOR %%f IN (tests\input*.txt) DO (CALL :TEST %%f || GOTO :ERROR)

ECHO All tests passed.
GOTO :OK

:TEST
  SET ifile=%1
  SET eofile=%ifile:input=output%
  SET aofile=%TMP%\output.txt

  ECHO Testing against %ifile% and %eofile%...

  x64\Release\main.exe < %ifile% > %aofile% && FC %aofile% %eofile%

  IF %ERRORLEVEL% EQU 0 (
      ECHO PASSED
      EXIT /B 0
  ) ELSE (
      ECHO FAILED
      EXIT /B 1
  )

:ERROR
POPD
PAUSE
EXIT /B 1

:OK
POPD
PAUSE
EXIT /B 0

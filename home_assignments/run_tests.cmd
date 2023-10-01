@ECHO OFF

PUSHD %1

IF NOT DEFINED VSCMD_VER (
  ECHO Opening "Developer Command Prompt for VS"...

  CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
  IF %ERRORLEVEL% NEQ 0 (
    ECHO Visual Studio is not configured correctly.
    GOTO :ERROR
  )
)

CD ..
ECHO Compiling solution...

MSBUILD /t:Build /p:Configuration=Release %2.sln

IF %ERRORLEVEL% NEQ 0 (
  ECHO Compilation failed.
  GOTO :ERROR
)

ECHO Solution compiled.

CD tests

FOR /D %%t IN (test-*) DO (CALL :test %%t %2 || GOTO :ERROR)

ECHO All tests passed.
GOTO :OK

:test
  CD %1

  ECHO Running test %1...

  ..\..\x64\Release\%2.exe < input.txt > output.txt
  > exitcode.txt ECHO %ERRORLEVEL%

  FC /N /L exitcode.txt expected_exitcode.txt
  IF %ERRORLEVEL% NEQ 0 (
    ECHO Expected exitcode:
    TYPE expected_output.txt
    ECHO Actual exitcode:
    TYPE output.txt
    GOTO :failed
  )

  FC /B output.txt expected_output.txt
  IF %ERRORLEVEL% NEQ 0 (
    ECHO Expected output:
    TYPE expected_output.txt
    ECHO Actual output:
    TYPE output.txt
    GOTO :failed
  )

  ECHO Test passed.

  :cleanup
  CD ..
  GOTO :EOF

  :failed
  CALL :cleanup
  ECHO Test failed.
  EXIT /B 1
  GOTO :EOF

:ERROR
POPD
PAUSE
EXIT /B 1

:OK
POPD
PAUSE
EXIT /B 0
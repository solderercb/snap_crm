@rem Автоматическое обновление порядкового номера ревизии ветки master. Предназначено для вызова из хуков post-checkout и post-commit, а также перед компиляцией
@rem При работе в другой ветке в строку версии будет добавлено название этой ветки
@rem При наличии незафиксированных изменений в встроку версии будет добавлена метка dirty
@echo off
setlocal EnableExtensions DisableDelayedExpansion
chcp 65001 >nul 2>&1
@set LC_CTYPE=

for /f "TOKENS=*" %%v in ('git rev-list master --first-parent --count') do (
  set /A version=%%v
)

for /f "TOKENS=*" %%b in ('git branch --show-current') do (
  set branch=%%b
)

set dirty=dirty
if "%branch%" NEQ "master" (
  call :check_diff version || set "dirty=" && goto :skip_update
) else (
  call :check_diff version || set "dirty="
  set branch=
)

C:\Windows\System32\sed -i "s/^#define APP_COMMIT.*$/#define APP_COMMIT %version%/g" appver.h
:skip_update
set build_remark=%branch%%dirty%
if "%branch%" NEQ "" (
  if "%dirty%" NEQ "" (
    set build_remark=%branch%-%dirty%
  )
)
if "%build_remark%" NEQ "" (
  set build_remark=[%build_remark%]
)
echo #define BUILD_REMARK %build_remark% > appver_build_remark.h
exit

:check_diff
setlocal DisableDelayedExpansion
set /A val=%1
for /f "TOKENS=*" %%d in ('git diff -- . ":(exclude)appver.h"') do (
  if "%%d" NEQ "" (
    set /A val=%1+1
    goto :end_check_diff
  )
)
exit /b 1
:end_check_diff
endlocal & set "%~1=%val%"
exit /b


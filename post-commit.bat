@echo off
chcp 65001
for /f "TOKENS=*" %%v in ('hg identify --num -r .') do (
  set /A version=%%v + 1
  )
::echo %version%
C:\Windows\System32\sed -i "s/^#define APP_COMMIT.*$/#define APP_COMMIT %version%/g" appver.h

@echo off
set "metadata=--unite-metadata"
::set "metadata="
set sourcedir=rare-packages-update-src-files
set repodir=rare-packages-update-mt-repo
rmdir /S /Q %repodir%
set "command=C:\Qt\Tools\QtInstallerFramework-mod\4.6_msvc2019_64\bin\repogen.exe %metadata% -p %sourcedir% --update %repodir%"
%command%

for /f %%x in ('wmic path Win32_LocalTime get /format:list ^| findstr "="') do (
    set %%x
)
set Second=0%Second%
set Second=%Second:~-2%
set Minute=0%Minute%
set Minute=%Minute:~-2%
set Hour=0%Hour%
set Hour=%Hour:~-2%
set Day=0%Day%
set Day=%Day:~-2%
set Month=0%Month%
set Month=%Month:~-2%
set TIMESTAMP=%Year%-%Month%-%Day%_%Hour%-%Minute%-%Second%

echo %command% > results_dump\%TIMESTAMP%_command.txt
for /f "usebackq" %%A in  (`dir "%%sourcedir%%\*" /a:d /o:n /b`) do (
  copy %sourcedir%\%%A\meta\package.xml results_dump\%TIMESTAMP%_%%A_package.xml
)
copy %repodir%\Updates.xml results_dump\%TIMESTAMP%_Updates.xml
"C:\Program Files\7-Zip\7z.exe" l %repodir%\*.7z > results_dump\%TIMESTAMP%_repo_content.txt

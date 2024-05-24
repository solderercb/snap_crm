set executable=SnapSetup.exe
del %executable%
C:\Qt\Tools\QtInstallerFramework-mod\4.6_msvc2019_64\bin\binarycreator.exe -p installer-packages-src-files -c installer-config\config.xml %executable%
:: Файл следует скопировать в папку сборки (build-*, debug, release).
:: Предпочитается создавать символические ссылки на библиотеки, но можно создать жёсткие ссылки (целевая папка должна быть расположена на том же диске, что и папка Qt (C:))
:: для этого mklink доложен запускаться с параметром /H; также можно банально скопировать файлы
:: В папке проекта необходимо поместить файл библиотеки mysql (для того, чтобы не удалить его вместе с целевой папкой)
mklink libgcc_s_seh-1.dll C:\Qt\6.2.1\mingw81_64\bin\libgcc_s_seh-1.dll
mklink libmysql.dll ..\libmysql.dll
mklink down-arrow.png ..\down-arrow.png
mklink libstdc++-6.dll C:\Qt\6.2.1\mingw81_64\bin\libstdc++-6.dll
mklink libwinpthread-1.dll C:\Qt\6.2.1\mingw81_64\bin\libwinpthread-1.dll
mklink Qt6Core.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6Core.dll
mklink Qt6Core5Compat.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6Core5Compat.dll
mklink Qt6Gui.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6Gui.dll
mklink Qt6QuickLayouts.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6QuickLayouts.dll
mklink Qt6Sql.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6Sql.dll
mklink Qt6Widgets.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6Widgets.dll
mkdir plugins\sqldrivers
mklink plugins\sqldrivers\qsqlmysql.dll C:\Qt\6.2.1\mingw81_64\plugins\sqldrivers\qsqlmysql.dll
mkdir plugins\platforms
mklink plugins\platforms\qwindows.dll C:\Qt\6.2.1\mingw81_64\plugins\platforms\qwindows.dll
mkdir plugins\styles
mklink plugins\styles\qwindowsvistastyle.dll C:\Qt\6.2.1\mingw81_64\plugins\styles\qwindowsvistastyle.dll

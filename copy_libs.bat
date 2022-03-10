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
mklink Qt6PrintSupport.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6PrintSupport.dll
mklink Qt6Qml.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6Qml.dll
mklink limereportd.dll ..\lib6\limereportd.dll 
mklink limereport.dll ..\lib6\limereport.dll
mklink Qt6Designer.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6Designer.dll
mklink Qt6DesignerComponents.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6DesignerComponents.dll
mklink Qt6Network.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6Network.dll
mklink QtZintd.dll ..\lib6\QtZintd.dll
mklink QtZint.dll ..\lib6\QtZint.dll
mklink Qt6OpenGL.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6OpenGL.dll 
mklink Qt6OpenGLWidgets.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6OpenGLWidgets.dll
mklink Qt6Svg.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6Svg.dll 
mklink Qt6UiTools.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6UiTools.dll 
mklink Qt6Xml.dll C:\Qt\6.2.1\mingw81_64\bin\Qt6Xml.dll 
mkdir plugins\sqldrivers
mklink plugins\sqldrivers\qsqlmysql.dll C:\Qt\6.2.1\mingw81_64\plugins\sqldrivers\qsqlmysql.dll
mkdir plugins\platforms
mklink plugins\platforms\qwindows.dll C:\Qt\6.2.1\mingw81_64\plugins\platforms\qwindows.dll
mkdir plugins\styles
mklink plugins\styles\qwindowsvistastyle.dll C:\Qt\6.2.1\mingw81_64\plugins\styles\qwindowsvistastyle.dll
mkdir plugins\imageformats
mklink plugins\imageformats\qgif.dll C:\Qt\6.2.1\mingw81_64\plugins\imageformats\qgif.dll
mklink plugins\imageformats\qico.dll C:\Qt\6.2.1\mingw81_64\plugins\imageformats\qico.dll
mklink plugins\imageformats\qjpeg.dll C:\Qt\6.2.1\mingw81_64\plugins\imageformats\qjpeg.dll
mklink plugins\imageformats\qsvg.dll C:\Qt\6.2.1\mingw81_64\plugins\imageformats\qsvg.dll
mklink /D reports ..\reports

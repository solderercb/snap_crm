3RDPARTY_ROOT       = $$PWD

!exists($$3RDPARTY_ROOT/outpwd.tmp){
    error("Something went wrong, can't find" $$3RDPARTY_ROOT/outpwd.tmp)
}
include($$3RDPARTY_ROOT/outpwd.tmp)

# Т. к. разработка может производиться на разных машинах (рабочий какталог синхронизируется через облако), то
# может возникнуть проблема с совместимостью библиотек (программа падает). Чтобы уменьшить путаницу было
# принято решение хранить библиотеки в директории, описывающей окружение сборки (имя машины и название комплекта сборки).
# А поскольку в QMAKE не предусмотрена переменная, содержащая название комплекта сборки (например, "Desktop (Qt 5.15.10 mingw81_64)")
# его нужно сгенерировать.
SYS_ROOT = $$system("$${QMAKE_CXX} -print-sysroot")         # e.g. C:/mingw810/x86_64-810-posix-seh-rt_v6-rev0/mingw64
COMPILER_VERSION = $$system($$QMAKE_CXX " -dumpversion")    # e.g. 8.1.0
COMPILER_MAJOR_VERSION = $$section(COMPILER_VERSION, ".", 0, 0)
COMPILER_MINOR_VERSION = $$section(COMPILER_VERSION, ".", 1, 1)
KIT_NAME = $$lower($$QMAKE_HOST.name)_"Qt_"$$replace(QT_VERSION, "\.", "_")"_"$$basename(SYS_ROOT)_$${COMPILER_MAJOR_VERSION}$${COMPILER_MINOR_VERSION} # e.g. solderer-home_Qt_5_15_4_mingw64_81

# путь к директории, куда будут копироваться скомпилированные библиотеки сторонних проектов
EXPORT_LIBS = $${3RDPARTY_ROOT}/../lib/$${KIT_NAME}

# утилита copy в windows не создаёт пути назначения
!exists($${EXPORT_LIBS}\\include) {
   $$system("mkdir \"$${EXPORT_LIBS}\\include\"")
}

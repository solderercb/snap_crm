@echo off
set "tmp=scomsettings_processed.tmp"
set "output=scomsettings_processed.h"

g++ -DSKIP_INCLUDES_IN_PREPROCESSING_EVALUATION -I%~dp0include -E "../../models/scomsettings.h" -o %tmp%

echo #include ^<QString^>>%output%
echo #include ^<QSqlDatabase^>>>%output%
echo #include ^<QSqlQuery^>>>%output%
echo #include ^<QSqlField^>>>%output%
echo #include ^<QSqlRecord^>>>%output%
echo #include ^<memory^>>>%output%
echo #include ^<type_traits^>>>%output%
echo #include ^<QLineEdit^>>>%output%
echo #include ^<QComboBox^>>>%output%
echo #include ^<QCheckBox^>>>%output%
echo #include ^<QSpinBox^>>>%output%
echo #include ^<QDoubleSpinBox^>>>%output%
echo #include ^<QRadioButton^>>>%output%
echo #include ^<QDateTimeEdit^>>>%output%
echo #include ^<QLabel^>>>%output%
echo #include ^<SComSettingsTypes^>>>%output%
echo #include ^<LimeReport^>>>%output%
echo #include ^<SCheckComboBox^>>>%output%
echo.>>%output%
::echo class SStandardItemModel;>>%output%
echo.>>%output%

sed -r "/^# [0-9]+/d" %tmp% >> %output%
::type %tmp% >> %output%


set "output=scomsettings_processed.cpp"
echo #include "scomsettings_processed.h">%output%
echo #include ^<QTimeZone^>>>%output%
echo #include ^<SStandardItemModel^>>>%output%
echo #include ^<ProjectGlobals^>>>%output%
echo #include ^<ProjectQueries^>>>%output%
type "..\..\models\ssinglerowmodelbase.cpp">%tmp%
type "..\..\models\ssettingsbase.cpp">>%tmp%
type "..\..\models\scomsettings.cpp">>%tmp%
sed -r "/^#include .+/d" %tmp% >> %output%

del %tmp%

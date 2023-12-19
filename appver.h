//#pragma once
#include "appver_build_remark.h"

// подсмотрено: https://stackoverflow.com/a/5459929
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)   // препроцессор заключит конструкцию x в кавычки

#define APP_MAJOR 0
#define APP_MINOR 0
#define APP_PATCH 0
#define APP_COMMIT 281

#define FILE_VER                    APP_MAJOR,APP_MINOR,APP_PATCH,APP_COMMIT    // обязательно запятые!
#define PRODUCT_VER                 FILE_VER

#define APP_VER                     APP_MAJOR.APP_MINOR.APP_PATCH.APP_COMMIT

#define PRODUCTNAME_STR             "SNAP CRM"
#define APP_VER_STR                 STR(APP_VER)
#define PRODUCT_VER_STR             STR(APP_VER BUILD_REMARK)
#define COMPANYDOMAIN_STR           "fixed.ks.ua"
#define COMPANYNAME_STR             COMPANYDOMAIN_STR
#define FILEDESCRIPTION_STR         "CRM for service centers"
#define INTERNALNAME_STR            "snap_crm"
#define LEGALCOPYRIGHT_STR          ""
#define LEGALTRADEMARKS1_STR        ""
#define LEGALTRADEMARKS2_STR        LEGALTRADEMARKS1_STR
#define ORIGINALFILENAME_STR        "snap.exe"

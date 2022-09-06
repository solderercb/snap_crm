//#pragma once

// подсмотрено: https://stackoverflow.com/a/5459929
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define APP_MAJOR 0
#define APP_MINOR 0
#define APP_PATCH 0
#define APP_COMMIT 115

#define APP_VER                     STR(APP_MAJOR.APP_MINOR.APP_PATCH.APP_COMMIT)   // препроцессор создаст из этой конструкции строку
//#define PROJECT_VERSION             APP_MAJOR.APP_MINOR.APP_PATCH.APP_COMMIT  // это не нужно, но оставлю для истории

#define VER_FILEVERSION             APP_MAJOR,APP_MINOR,APP_PATCH,APP_COMMIT    // обязательно запятые!
#define VER_FILEVERSION_STR         APP_VER "\0"

#define VER_PRODUCTVERSION          VER_FILEVERSION
#define VER_PRODUCTVERSION_STR      APP_VER "\0"

#define VER_COMPANYNAME_STR         "fixed.ks.ua"
#define VER_FILEDESCRIPTION_STR     "SNAP CRM"
#define VER_INTERNALNAME_STR        "SNAP CRM"
#define VER_LEGALCOPYRIGHT_STR      ""
#define VER_LEGALTRADEMARKS1_STR    ""
#define VER_LEGALTRADEMARKS2_STR    VER_LEGALTRADEMARKS1_STR
#define VER_ORIGINALFILENAME_STR    "snap.exe"
#define VER_PRODUCTNAME_STR         "SNAP CRM"

#define VER_COMPANYDOMAIN_STR       "fixed.ks.ua"

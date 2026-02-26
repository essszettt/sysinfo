
#if !defined(__VERSION_H__)
  #define __VERSION_H__

#define VER_STR_(s) #s
#define VER_STR(s) VER_STR_(s)

/* --- Produktversion --- */
#define APP_VERSION_MAJOR     0
#define APP_VERSION_MINOR     0
#define APP_VERSION_PATCH     22

#define APP_COMPANYNAME_STR   "STZ Engineering"
#define APP_DESCRIPTION_STR   "System Info ZX Spectrum Next"
#define APP_VERSION           APP_VERSION_MAJOR,APP_VERSION_MINOR,APP_VERSION_PATCH
#define APP_VERSION_STR       VER_STR(APP_VERSION_MAJOR) "." VER_STR(APP_VERSION_MINOR) "." VER_STR(APP_VERSION_PATCH)

#define APP_INTERNALNAME_STR    "sysinfo"
#define APP_LEGALCOPYRIGHT_STR  "\x7F 2025 " APP_COMPANYNAME_STR

#endif /* __VERSION_H__ */

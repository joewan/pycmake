// Patch for VS2015 and OVR_ALIGNAS macro
// This doesn't work properly right now, so just go back to previous behavior
#if defined(_MSC_VER) && _MSC_VER >= 1900
#include "OVR_Version.h"
#if OVR_PRODUCT_VERSION == 0 && OVR_MAJOR_VERSION == 8 && OVR_MINOR_VERSION == 0 && OVR_PATCH_VERSION == 0 && OVR_BUILD_NUMBER == 0
#undef OVR_ALIGNAS
#define OVR_ALIGNAS(n) __declspec(align(n))
#else
#error The Oculus SDK version has changed, please check that this work-around is still necessary. If it is, please update the version numbers above. If not, please delete this file (also update wscript + waf_files).
#endif
#endif

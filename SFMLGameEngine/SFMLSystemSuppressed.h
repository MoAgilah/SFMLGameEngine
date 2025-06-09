#pragma once

#ifdef _MSC_VER
#define DISABLE_MSVC_WARNING_4275 __pragma(warning(push)) \
                                    __pragma(warning(disable: 4275)) \
                                    __pragma(warning(disable: 4251))
#define ENABLE_MSVC_WARNINGS      __pragma(warning(pop))
#else
#define DISABLE_MSVC_WARNING_4275
#define ENABLE_MSVC_WARNINGS
#endif
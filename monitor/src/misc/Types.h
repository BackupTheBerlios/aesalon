#ifndef AESALON_MISC_TYPES_H
#define AESALON_MISC_TYPES_H

#ifndef QT_VERSION
#include <sys/types.h>
#endif

namespace Aesalon {
namespace Misc {

#if defined(QT_VERSION)
    typedef quint64 uint_64;
    typedef qint32 int_32;
    typedef qint16 int_16;
#else
    typedef u_int64_t uint_64;
    typedef int32_t int_32;
    typedef int16_t int_16;
#endif

} // namespace Misc
} // namespace Aesalon

#endif


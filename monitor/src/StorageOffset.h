#ifndef AESALON_MONITOR_STORAGE_OFFSET_H
#define AESALON_MONITOR_STORAGE_OFFSET_H

#include "Types.h"

/** Defines a storage offset, used by StorageManager. This type is
    signed, since -1 is used as an error code by some functions. */
typedef SWord StorageOffset;

#endif

#ifndef AESALON_ANALYZER_STORAGE_ATTRIBUTE_H
#define AESALON_ANALYZER_STORAGE_ATTRIBUTE_H

#include "StorageOffset.h"

namespace Analyzer {

struct StorageAttribute {
    StorageOffset offset;
    StorageOffset name;
    Word value;
    
    StorageOffset next;
    StorageOffset child;
};

} // namespce Analyzer

#endif

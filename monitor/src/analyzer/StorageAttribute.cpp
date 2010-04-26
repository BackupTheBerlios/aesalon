#include "StorageAttribute.h"

namespace Analyzer {

StorageAttribute::StorageAttribute() : name(NULL), value(0), next(NULL), child(NULL) {
    
}

StorageAttribute::~StorageAttribute() {
    
}

} // namespace Analyzer

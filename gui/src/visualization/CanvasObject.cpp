#include "CanvasObject.h"

CanvasObject::CanvasObject(const DataRange &bounding_rect) : bounding_rect(bounding_rect) {
    references = 0;
}

CanvasObject::~CanvasObject() {

}

void CanvasObject::inc_references() {
    references ++;
}

void CanvasObject::dec_references() {
    references --;
    if(references == 0) {
        delete this;
    }
}

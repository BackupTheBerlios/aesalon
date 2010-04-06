#include "CanvasObject.h"

CanvasObject::CanvasObject(const DataRange &bounding_rect) : bounding_rect(bounding_rect), next(NULL) {
    references = 0;
}

CanvasObject::~CanvasObject() {

}

void CanvasObject::inc_references() {
    references ++;
    if(references > 1) {
        qDebug("Refcount incremented to > 1 . . .");
    }   
}

void CanvasObject::dec_references() {
    references --;
    if(references == 0) {
        qDebug("Deleting CanvasObject with address of %p . . .", (void *)this);
        delete this;
    }
}

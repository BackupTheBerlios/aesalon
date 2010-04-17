#include "CanvasObject.h"

CanvasObject::CanvasObject(const DataRange &bounding_rect) : bounding_rect(bounding_rect), next(NULL), prev(NULL) {
}

CanvasObject::~CanvasObject() {

}

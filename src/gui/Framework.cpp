#include "Framework.h"
#include "Framework.moc"

namespace Aesalon {
namespace GUI {

Framework::Framework(QWidget *parent) {
    this->setGeometry(0, 0, 800, 600);
    main_layout = new QGridLayout();
    
    
}

Framework::~Framework() {

}



} // namespace GUI
} // namespace Aesalon

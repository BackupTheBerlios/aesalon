#include <iostream>

#include "ActiveSessionOverview.h"
#include "ActiveSessionOverview.moc"

namespace Aesalon {
namespace GUI {

ActiveSessionOverview::ActiveSessionOverview(Session *session, QWidget *parent) : session(session), run_time() {
    info_form = new QFormLayout();
    
    info_form->addRow("Session name: ", new QLabel(session->get_session_name()));
    info_form->addRow("Executable path: ", new QLabel(session->get_executable_path()));
    running_time = new QLabel();
    info_form->addRow("Running time: ", running_time);
    this->setLayout(info_form);
    run_time.start();
    running_time_timer = new QTimer(this);
    connect(running_time_timer, SIGNAL(timeout()), this, SLOT(update_running_time()));
    running_time_timer->start(1000);
}

void ActiveSessionOverview::update_running_time() {
    /*qDebug("Asked up update running time; text is \"%i\"", );*/
    QTime elapsed_time = QTime(0, 0, 0).addSecs(run_time.elapsed() / 1000);
    running_time->setText(elapsed_time.toString());
    this->update();
}

} // namespace GUI
} // namespace Aesalon

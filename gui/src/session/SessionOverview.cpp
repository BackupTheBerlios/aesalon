#include "SessionOverview.h"
#include "SessionOverview.moc"
#include "Session.h"

SessionOverview::SessionOverview(DataThread *data_thread) : QWidget(NULL), status(WAITING), data_thread(data_thread) {
    main_layout = new QFormLayout();
    time_running_label = new QLabel("00:00");
    main_layout->addRow(tr("Running time:"), time_running_label);
    
    QTimer *update_timer = new QTimer(this);
    connect(update_timer, SIGNAL(timeout()), SLOT(update_time_running()));
    update_timer->start(1000);
    
    v_list = new VisualizationList();
    
    main_layout->addRow(v_list);
    
    new_visualization_button = new QPushButton(tr("&New visualization"));
    connect(new_visualization_button, SIGNAL(clicked()), SLOT(new_visualization()));
    main_layout->addRow(new_visualization_button);
    
    setLayout(main_layout);
}

SessionOverview::~SessionOverview() {
    
}

void SessionOverview::update_time_running() {
    Timestamp now = Timestamp();
    quint64 seconds = 0;
    if(status == RUNNING) seconds = start_timestamp.seconds_until(now);
    else seconds = start_timestamp.seconds_until(finish_timestamp);
    QString string;
    string.sprintf("%02lli:%02lli", seconds / 60, seconds % 60);
    time_running_label->setText(string);
}

void SessionOverview::new_visualization() {
    Visualization *visualization = v_list->create_from_selected(data_thread);
    if(visualization) {
        visualization->initialize();
        emit new_visualization(visualization);
    }
}

void SessionOverview::started() {
    start_timestamp = Timestamp();
    status = RUNNING;
}

void SessionOverview::finished() {
    finish_timestamp = Timestamp();
    status = FINISHED;
}

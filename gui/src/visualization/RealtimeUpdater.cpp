#include "RealtimeUpdater.h"
#include "RealtimeUpdater.moc"

RealtimeUpdater::RealtimeUpdater(int history_length, int cycle_length)
    : VisualizationUpdater(), history_length(history_length), cycle_length(cycle_length) {
    cycle_timer = new QTimer(this);
    connect(cycle_timer, SIGNAL(timeout()), SLOT(render_cycle()));
    cycle_timer->start(cycle_length);
    range.set_lower_data(0);
    range.set_upper_data(0);
}

RealtimeUpdater::~RealtimeUpdater() {

}

void RealtimeUpdater::render_cycle() {
    Timestamp current_time;
    if(current_time > range.get_upper_time()) {
        range.set_lower_time(Timestamp());
        current_time.add_ms(-cycle_length);
        range.set_upper_time(current_time);
        emit move_display(range.get_lower_time(), range.get_upper_time());
        current_time.add_ms(cycle_length);
    }
    VisualizationDataRange render_range;
    render_range.set_upper_time(current_time);
    current_time.add_ms(-cycle_length);
    render_range.set_lower_time(current_time);
    emit render_region(render_range.get_lower_time(), render_range.get_upper_time());
}

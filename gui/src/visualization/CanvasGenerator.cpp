#include "CanvasGenerator.h"
#include "CanvasGenerator.moc"

CanvasGenerator::CanvasGenerator(DataThread *data_thread, Renderer *renderer)
    : QDialog(NULL), data_thread(data_thread), renderer(renderer) {
    
    setMinimumSize(300, 50);
    
    main_layout = new QVBoxLayout();
    
    setWindowTitle(tr("Processing . . ."));
    
    progress_bar = new QProgressBar();
    progress_bar->setOrientation(Qt::Horizontal);
    progress_bar->setTextVisible(true);
    main_layout->addWidget(progress_bar);
    
    setLayout(main_layout);
    
    update_timer = new QTimer();
    connect(update_timer, SIGNAL(timeout()), SLOT(update_progress_bar()));
    update_timer->start(500);
    
    gen_thread = new CanvasGeneratorThread(data_thread, renderer);
    connect(gen_thread, SIGNAL(finished()), SLOT(accept()));
    gen_thread->start(QThread::HighestPriority);
    
    progress_bar->setRange(0, data_thread->get_snapshot_list()->get_last_id());
    
    setWindowModality(Qt::WindowModal);
    show();
}

CanvasGenerator::~CanvasGenerator() {

}

void CanvasGenerator::update_progress_bar() {
    progress_bar->setValue(renderer->get_last_snapshot());
}

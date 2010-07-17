#include "RenderingThread.h"

RenderingThread::RenderingThread() : QThread(module) {

}

RenderingThread::~RenderingThread() {

}

void RenderingThread::addRequest(RenderingThreadRequest request) {

}

void RenderingThread::run() {
	QThread::run();
}

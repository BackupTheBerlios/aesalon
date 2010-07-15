#include <iostream>

#include "Visualizer.h"

extern "C" {

VisualizerInterface *AesalonVisualizerCreateInstance() {
	return new CpuTimeVisualizer();
}

}

CpuTimeVisualizer::CpuTimeVisualizer() {

}

CpuTimeVisualizer::~CpuTimeVisualizer() {

}

void CpuTimeVisualizer::processIncoming(DataPacket *packet) {
	std::cout << "Received packet!" << std::endl;
}

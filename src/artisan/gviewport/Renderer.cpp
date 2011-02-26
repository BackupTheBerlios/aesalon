/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/Renderer.cpp
*/

#include "artisan/gviewport/Renderer.h"
#include "artisan/gviewport/Object.h"

namespace Artisan {
namespace GViewport {

Renderer::Renderer(Data &data) : m_data(data) {
	
}

Renderer::~Renderer() {
	
}

void Renderer::render(RenderRequest request) {
	m_data.lock();
	
	RenderedImage image(request.width(), request.height(),
		request.bound().range(1).start(),
		request.bound().range(2).start(),
		request.bound().range(1).end(),
		request.bound().range(2).end());
	
	class Processor : public TreeType::SearchProcessor {
	private:
		Renderer *m_renderer;
		RenderedImage &m_image;
	public:
		Processor(Renderer *renderer, RenderedImage &image) : m_renderer(renderer), m_image(image) {}
		
		virtual bool process(const TreeType::Bound &bound, Object *value) {
			if(m_renderer->shouldRender(value)) value->render(m_image);
			
			return true;
		}
	};
	
	Processor p(this, image);
	
	image.startPainting();
	
	m_data.tree().search(request.bound(), &p);
	
	image.endPainting();
	
	m_data.unlock();
	
	emit renderingFinished(image);
}

} // namespace GViewport
} // namespace Artisan

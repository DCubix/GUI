#ifndef SYG_IMAGEVIEW_H
#define SYG_IMAGEVIEW_H

#include "widget.h"
#include "../image.h"

class ImageView : public Widget {
public:
	virtual void onDraw(Renderer& renderer) override;

	Image* image() { return m_image; }
	void image(Image* img) { m_image = img; }

	virtual Size preferredSize() override;

private:
	Image* m_image{ nullptr };
};

#endif // SYG_IMAGEVIEW_H

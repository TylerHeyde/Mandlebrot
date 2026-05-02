#include "ComplexPlane.h"
//---public functions---//
//constructor

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
	m_pixel_size = { pixelWidth, pixelHeight };
	m_aspectRatio = (float)sf::VideoMode::getDesktopMode().height / (float)sf::VideoMode::getDesktopMode().width;
	m_plane_center = { 0, 0 };
	m_zoomCount = 0;
	m_State = State::CALCULATING;
	m_vArray.setPrimitiveType(sf::Points);
	m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_vArray, states);
}

// stopped on the mapPixelToCoords step
void ComplexPlane::updateRender() {
	for (int i = 0; i < m_pixel_size.y; ++i) {
		for (int j = 0; j < m_pixel_size.x; ++j) {
			m_vArray[i * m_pixel_size.x + j].position = { (float)j, (float)i };
			//turns the pixel coordinates into coordinates on the complex plane then counts the iterations for that coordinate
			sf::Vector2f coord = ComplexPlane::mapPixelToCoords({ j, i });
			int iterations = ComplexPlane::countIterations(coord);

			//local rgb variables to store current pixel's color
			uint8_t r, g, b;

			//uses pointers and the iterations to set rgb values
			ComplexPlane::iterationsToRGB(iterations, r, g, b);
			m_vArray[i * m_pixel_size.x + j].color = sf::Color(r, g, b);

			//sets state to displaying after all pixels have been calculated and colored
			m_State = State::DISPLAYING;
		}
	}
}

void ComplexPlane::zoomIn() {
	m_zoomCount++;
	float x_size = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y_size = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x_size, y_size };
	m_State = CALCULATING;
}

void ComplexPlane::zoomOut() {
	m_zoomCount--;
	float x_size = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y_size = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x_size, y_size };
	m_State = CALCULATING;
}

void ComplexPlane::setCenter(sf::Vector2i mousePixel) {
	m_plane_center = ComplexPlane::mapPixelToCoords(mousePixel);
}

void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {
	//sf::Mouse::getPosition(window);	// relative to the window
	// sf::Mouse::getPosition()	// global screen position
	// window.mapPixelToCoords(sf::Mouse::getPosition(window))	// as it floats
	m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(sf::Text& text) {
	stringstream ss;

	ss << fixed << setprecision(2);

	ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";

	ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";

	ss << "Left-click to Zoom in" << "\n";
	ss << "Right-click to Zoom out" << "\n";

	text.setString(ss.str());
}

//---private functions---//
int ComplexPlane::countIterations(sf::Vector2f coord) {
	int iterations = 0;
	//---this is the code that was written based on the assignment description---//

	complex<double> c = { coord.x, coord.y };
	complex<double> z = (0, 0);

	//was doing some research and found that using norm() is faster than abs() for checking the magnitude of a complex number, so I switched to that
	while (norm(z) <= 4.0 && iterations < MAX_ITER) {
		z = z * z + c;
		++iterations;
	}


	//went gpt ways to speed up exicution and it said doing it manually is faster than using the complex library, so this is just trying that

	//double zx = 0.0, zy = 0.0;
	//double cx = coord.x, cy = coord.y;

	//while (zx * zx + zy * zy <= 4.0 && iterations < MAX_ITER) {
	//	double xtemp = zx * zx - zy * zy + cx;
	//	zy = 2.0 * zx * zy + cy;
	//	zx = xtemp;
	//	++iterations;
	//}

	return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, uint8_t& r, uint8_t& g, uint8_t& b) {
	if (count >= MAX_ITER) {
		r = 0;
		g = 0;
		b = 0;
		return;
	}
	//  Map your iteration count to a value t between[0:1]
	//	red(t) = startRed * (1 - t) + endRed * t
	//	green(t) = startGreen * (1 - t) + endGreen * t
	//	blue(t) = startBlue * (1 - t) + endBlue * t
	float t = (float)count / (float)MAX_ITER;
	//r = 0 * (1 - t) + 255 * t;
	//g = 0 * (1 - t) + 255 * t;
	//b = 0 * (1 - t) + 255 * t;
	//r = sin(3.0 * t + 0.0) * 127 + 128;
	//g = sin(3.0 * t + 2.0) * 127 + 128;
	//b = sin(3.0 * t + 4.0) * 127 + 128;
	//r = (uint8_t)(255 * t);
	//g = (uint8_t)(255 * t);
	//b = (uint8_t)(255 * t);

	//this is the clasic mandelbrot coloring
	r = (uint8_t)(9 * (1 - t) * t * t * t * 255);
	g = (uint8_t)(15 * (1 - t) * (1 - t) * t * t * 255);
	b = (uint8_t)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
}

//maps the pixel coordinates [a,b] which is the [x,y] position of the pixel to [c,d] which is the [x,y] position on the complex plane
sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {
	sf::Vector2f coord;
	coord.x = (((float)mousePixel.x - 0.0) / ((float)m_pixel_size.x - 0.0)) * (m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);
	coord.y = (((float)mousePixel.y - (float)m_pixel_size.y) / (0.0 - (float)m_pixel_size.y)) * (m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);

	return coord;
}
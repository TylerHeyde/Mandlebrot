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
	double x_size = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	double y_size = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x_size, y_size };
	m_State = CALCULATING;
}

void ComplexPlane::zoomOut() {
	m_zoomCount--;
	double x_size = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	double y_size = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
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
	m_mouseLocation = mapPixelToCoords(sf::Mouse::getPosition());
}
// TODO:
void ComplexPlane::loadText(sf::Text& text) {
	;	// idk how to draw string on the canvase
}

//---private functions---//
int ComplexPlane::countIterations(sf::Vector2f coord) {
	int iterations = 0;
	complex<double> c = { coord.x, coord.y };
	complex<double> z = (0, 0);

	//was doing some research and found that using norm() is faster than abs() for checking the magnitude of a complex number, so I switched to that
	while (norm(z) <= 4.0 && iterations < MAX_ITER) {
		z = z * z + c;
		++iterations;
	}

	return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, uint8_t& r, uint8_t& g, uint8_t& b) {
	//  Map your iteration count to a value t between[0:1]
	//	red(t) = startRed * (1 - t) + endRed * t
	//	green(t) = startGreen * (1 - t) + endGreen * t
	//	blue(t) = startBlue * (1 - t) + endBlue * t
	float t = (float)count / (float)MAX_ITER;
	r = 255 * (1 - t) + 0 * t;
	g = 255 * (1 - t) + 0 * t;
	b = 255 * (1 - t) + 0 * t;
}

//maps the pixel coordinates [a,b] which is the [x,y] position of the pixel to [c,d] which is the [x,y] position on the complex plane
sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {
	sf::Vector2f coord;
	coord.x = (((float)mousePixel.x - 0.0) / ((float)m_pixel_size.x - 0.0)) * (m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);
	coord.y = (((float)mousePixel.y - (float)m_pixel_size.y) / (0.0 - (float)m_pixel_size.y)) * (m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);

	return coord;
}
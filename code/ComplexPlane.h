#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <complex>
#include <sstream>
#include <iomanip>

using namespace std;

// base global variables for the set
const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0f;
const float BASE_HEIGHT = 4.0f;
const float BASE_ZOOM = 0.5f;

// enum for the state of the set
enum State { CALCULATING, DISPLAYING };

// Class for the complex plane, child of parent "Drawable" so it can be drawn to the window
class ComplexPlane : public sf::Drawable
{
public:
	ComplexPlane(int pixelWidth, int pixelHeight);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void zoomIn();
	void zoomOut();
	void setCenter(sf::Vector2i mousePixel);
	void setMouseLocation(sf::Vector2i mousePixel);
	void loadText(sf::Text& text);
	void updateRender();
private:
	//private functions
	int countIterations(sf::Vector2f coord);
	void iterationsToRGB(size_t count, uint8_t& r, uint8_t& g, uint8_t& b);
	sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel);

	//data members
	sf::VertexArray m_vArray;
	State m_State;
	sf::Vector2f m_mouseLocation;
	sf::Vector2i m_pixel_size;
	sf::Vector2f m_plane_center;
	sf::Vector2f m_plane_size;
	int m_zoomCount;
	float m_aspectRatio;
};


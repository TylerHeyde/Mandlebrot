#include <SFML/Graphics.hpp>

#include <iostream>




using namespace sf;

using namespace std;




int main()

{

	int pixelWidth = VideoMode::getDesktopMode().width;

	int pixelHeight = VideoMode::getDesktopMode().height;

	VideoMode vm(pixelWidth, pixelHeight);
	// Create the main redering window 
	RenderWindow window(vm, "Mandlebrot", Style::Default);

	/**/ // debug code for testing rendering
	 
	 
	 
	VertexArray particle1(TriangleFan);
	particle1.resize(5);

	Vector2f center = { (float)pixelWidth / 2.0f, (float)pixelHeight / 2.0f };

	particle1[0].color = Color::Blue;
	particle1[0].position = center;
	particle1[1].color = Color::Green;
	particle1[1].position = { center.x - 100.0f, center.y - 100.0f };
	particle1[2].color = Color::Red;
	particle1[2].position = { center.x + 100.0f, center.y - 100.0f };
	particle1[3].color = Color::Red;
	particle1[3].position = { center.x + 100.0f, center.y + 100.0f };
	particle1[4].color = Color::Red;
	particle1[4].position = { center.x - 100.0f, center.y + 100.0f };
	//*/

	while (window.isOpen())

	{

		///Input

		if (Keyboard::isKeyPressed(Keyboard::Escape))

		{

			window.close();

		}




		///Update




		///Draw

		window.clear();

		/*Debug*/  window.draw(particle1);

		window.display();

	}




}
#include <SFML/Graphics.hpp>

#include <iostream>
#include "ComplexPlane.h"



using namespace sf;

using namespace std;




int main()

{

    int pixelWidth = VideoMode::getDesktopMode().width;

    int pixelHeight = VideoMode::getDesktopMode().height;

    VideoMode vm(pixelWidth, pixelHeight);
    // Create the main redering window 
    RenderWindow window(vm, "Mandlebrot", Style::Default);
    // ComplexPlane cPlane(pixelWidth, pixelHeight);
    // debug code for testing rendering 
    // VertexArray particle1(TriangleFan);
    // particle1.resize(5); 
    // Vector2f center = { (float)pixelWidth / 2.0f, (float)pixelHeight / 2.0f };
    // particle1[0].color = Color::Blue;
    // particle1[0].position = center;
    // particle1[1].color = Color::Green;
    // particle1[1].position = { center.x - 100.0f, center.y - 100.0f };
    // particle1[2].color = Color::Red;
    // particle1[2].position = { center.x + 100.0f, center.y - 100.0f };
    // particle1[3].color = Color::Red;
    // particle1[3].position = { center.x + 100.0f, center.y + 100.0f };
    // particle1[4].color = Color::Red;
    // particle1[4].position = { center.x - 100.0f, center.y + 100.0f };


    while (window.isOpen())
    {
        ///Input
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    cout << "zoomIn()\n";
                    // zoomIn();
                }
                else if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // zoomOut();
                    cout << "zoomOut()\n";
                }
            }
            if (event.type == sf::Event::MouseMoved)
            {
                // setMouseLocation();
                cout << "setMouseLocation\n";
            }
        }
        ///Update
        // cPlane.updateRender();
        // cPlane.loadText();

        ///Draw
        window.clear();
        // cPlane.draw();
        // window.display();
    }





}
#include <SFML/Graphics.hpp>
#include <iostream>
#include <eigen3/Eigen/Dense>

const std::string title = "16 rotational puzzle";

sf::RenderWindow window(sf::VideoMode(500, 400), title, sf::Style::Resize | sf::Style::Close);
sf::Font font;
sf::Vector2i mousePos {0, 0};

void on_resize(sf::Event e) {    
    window.setView(sf::View(
        sf::FloatRect(0, 0,
            static_cast<float>(e.size.width),
            static_cast<float>(e.size.height)
        )
    ));
}

class MatrixView
{
private:
    Eigen::Matrix4i m;
    sf::Vector2f position;
    const sf::Vector2f cell {50, 50};
    const sf::Vector2f padding {13, 5};
    sf::Text text;
    
    float fragmentAngle = 0;
    sf::IntRect rotatingFragment {0,0,0,0};

    void drawRotatingFragment() 
    {
        sf::Vector2f initial = position;
        sf::Vector2f curr = position + padding + sf::Vector2f(rotatingFragment.left * cell.x, rotatingFragment.top * cell.y) ;
        
        const sf::Vector2f size { cell.x * m.cols(), cell.y * m.rows() };
        curr += padding;

        // center of rotating fragment
        sf::Vector2f center {
            initial.x + cell.x * (rotatingFragment.left + float(rotatingFragment.width) / 2),
            initial.y + cell.y * (rotatingFragment.top + float(rotatingFragment.height) / 2)
        };
        
        for (int i = rotatingFragment.left; i < rotatingFragment.left + rotatingFragment.width; i++) {
            for (int j = rotatingFragment.top; j < rotatingFragment.top + rotatingFragment.height; j++) {
                text.setPosition(curr);
                text.setString(std::to_string(m(i,j)));
                sf::Transform t;
                t.rotate(fragmentAngle, center);
                window.draw(text, t);
                curr.x += cell.x; 
            }
            curr.x = initial.x + padding.x + cell.x;
            curr.y += cell.y;
        }
    }

public:
    MatrixView(Eigen::Matrix4i mx, sf::Vector2i pos): m(mx), position(pos) 
    {
        text.setCharacterSize(24);
        text.setFont(font);
        text.setFillColor(sf::Color::Black);
    }
    void setPosition(sf::Vector2f pos) { position = pos; }
    void setMatrix(Eigen::Matrix4i mx) { m = mx; }
    // sf::IntRect skip = {0, 0, 0, 0}
    void drawMatrix(bool interactive)
    {
        sf::Vector2f initial = position;
        sf::Vector2f curr = position;
        
        const sf::Vector2f size { cell.x * m.cols(), cell.y * m.rows() };
        curr += padding;
        
        sf::Text text;
        text.setCharacterSize(24);
        text.setFont(font);
        text.setFillColor(sf::Color::Black);
        
        sf::Vertex horiz_line[] = {
            sf::Vertex(sf::Vector2f(initial.x, initial.y)),
            sf::Vertex(sf::Vector2f(initial.x, initial.y + size.y))
        };
        horiz_line[0].color = horiz_line[1].color = sf::Color::Black;
        sf::Vertex vert_line[] = {
            sf::Vertex(sf::Vector2f(initial.x, initial.y)),
            sf::Vertex(sf::Vector2f(initial.x + size.x, initial.y))
        };
        vert_line[0].color = vert_line[1].color = sf::Color::Black;

        for (int i = 0; i < m.rows(); i++) {
            for (int j = 0; j < m.cols(); j++) {
                sf::FloatRect cellShape {curr - padding, cell};
                if (cellShape.contains(mousePos.x, mousePos.y)) {
                    sf::RectangleShape r {cell};
                    r.setPosition(cellShape.left, cellShape.top);
                    r.setFillColor(sf::Color::Cyan);
                    window.draw(r);
                }
                if (!rotatingFragment.contains(i, j)) {
                    text.setPosition(curr);
                    text.setString(std::to_string(m(i,j)));
                    window.draw(text);
                }
                curr.x += cell.x; 
            }
            curr.x = initial.x + padding.x;
            curr.y += cell.y;

            window.draw(horiz_line, 2, sf::Lines);
            window.draw(vert_line, 2, sf::Lines);
            horiz_line[0].position.x += cell.x;
            horiz_line[1].position.x += cell.x;
            vert_line[0].position.y += cell.y;
            vert_line[1].position.y += cell.y;
        }
        window.draw(horiz_line, 2, sf::Lines);
        window.draw(vert_line, 2, sf::Lines);


        if (fragmentAngle > 0) {
            drawRotatingFragment();
            fragmentAngle += 90.0 / 120.0;
        }
        // end rotation if needed
        if (90.0 - fragmentAngle < (90.0 / 120.0)) {
            fragmentAngle = 0.0;
            rotatingFragment = sf::IntRect {0, 0, 0, 0};
        }
    }
    
    void rotateFragment(sf::IntRect fragment) {
        rotatingFragment = fragment;
        fragmentAngle += 90.0 / 120.0;
    }
};




int main()
{
    Eigen::Matrix4i m;
    m << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;
    
    font.loadFromFile("assets/fonts/calibri-reg.ttf");

    MatrixView mview(m, sf::Vector2i(20, 30));

    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized) {
                on_resize(event);
            }
            else if (event.type == sf::Event::MouseMoved) {
                mousePos = sf::Mouse::getPosition(window);
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                mview.rotateFragment(sf::IntRect(1, 1, 2, 2));
            }
        }

        window.clear(sf::Color::White);
        mview.drawMatrix(true);
        window.display();
    }

    return 0;
}


#include "ttt.h"
#include <string>
#include <iostream>
#include <vector>

#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Main.hpp>


//
//	CONSTANTS
//
/// WINDOW
const unsigned WIDTH = 720;
const unsigned HEIGHT = 480;
const std::string TITLE = "Hyper Tic-Tac-Toe";
const sf::Uint32 STYLE = sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize;
const sf::Int32 FRAME_RATE = 30;

extern size_t n;
extern size_t a;
extern size_t r;

//
//	GLOBALS
//
/// ENGINE
sf::RenderWindow window_;
sf::Vector2f mouse_pos_;
bool running_ = true;
bool redraw_ = true;
sf::Clock clock_;
sf::Time next_tick_ = clock_.getElapsedTime();
bool keys_[sf::Keyboard::KeyCount];
/// GAMEPLAY
Field turn_;

const float TileSize = 18;
const float TileNOffset = 6;
sf::Vector2f tiles_offset_ = sf::Vector2f(20,10);
float tiles_scale_ = 0;
struct Tile
{
	size_t i;
	sf::RectangleShape rect;
	VMapPos dim;
};
std::vector<Tile> tiles_;
Map map_;

//
//	HEADERS
//
void init_sfml();
void init_game();
void handle_input();
void update();
void redraw();


int main(int argc, char ** argv)
{
	init_sfml();
	init_game();

	while (running_)
	{
		handle_input();

		if (clock_.getElapsedTime() >= next_tick_)
		{
			update();
			redraw();

			next_tick_ += sf::milliseconds(1000 / FRAME_RATE);
		}

	}

	return 0;
}

void init_sfml()
{
	window_.create(sf::VideoMode(WIDTH, HEIGHT), TITLE, STYLE);
	window_.setMouseCursorVisible(true);
}

int f(int N)
{
	return ++N % 2 ? int(N / 2) : N / 2 - 1;
}

int dimoffset(int N)
{
	if (N == 0 || N == 1)
		return TileSize + 1;
	else if (N == 2)
		return a * dimoffset(0) + TileNOffset;
	else
		return a * dimoffset(N - 2) + (2) * TileNOffset;
}

void init_game()
{
	std::cout << "Enter number of dimensions: ";
	std::cin >> n;
	std::cout << "Enter size of the edges: ";
	std::cin >> a;
	r = a;

	map_.resize(pow(a, n), EMPTY);
	turn_ = Field(std::rand() % 2);

	for (size_t i = 0; i < map_.size(); i++)
	{
		VMapPos v = pos_to_vector(i);
		int x = 0, y = 0;
		sf::Color color = sf::Color::White;
		for (size_t N = 0; N < n; N++)
		{
			if (N % 2 == 0)
			{
				x += v[N] * dimoffset(N);
			}
			else
			{
				y += v[N] * dimoffset(N);
			}


		}
		sf::RectangleShape rect(sf::Vector2f(TileSize, TileSize));
		rect.setFillColor(color);
		rect.setPosition(x, y);

		tiles_.push_back({ i, rect, pos_to_vector(i) });

	}
}

bool is_in(sf::Vector2f pos, const Tile & tile)
{
	float x0 = tile.rect.getPosition().x;
	float y0 = tile.rect.getPosition().y;
	float x1 = x0 + TileSize;
	float y1 = y0 + TileSize;

	if (pos.x >= x0 && pos.x < x1 && pos.y >= y0 && pos.y < y1)
		return true;
	else
		return false;
}

void handle_input()
{
	sf::Event event;

	while (window_.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			running_ = false;
			break;
		case sf::Event::KeyPressed:
			keys_[event.key.code] = true;
			break;
		case sf::Event::KeyReleased:
			if (event.key.code != sf::Keyboard::Unknown)
			{
				keys_[event.key.code] = false;
			}
			break;

		case sf::Event::MouseButtonPressed:
			for (auto & t : tiles_)
			{
				if (is_in(mouse_pos_ - tiles_offset_, t))
				{
					t.rect.setFillColor(sf::Color(100, 100, 100));
					if (map_[t.i] == EMPTY)
					{
						map_[t.i] = turn_;
						turn_ = Field(!turn_);
						auto win = check_win(map_, t.i);
						switch (win)
						{
						case O:
							std::cout << "Player Blue created a line!" << std::endl;
							break;
						case X:
							std::cout << "Player Red created a line!" << std::endl;
							break;
						}
					}
				}
				else
				{
					t.rect.setFillColor(sf::Color::White);
				}

			}
			redraw_ = true;

			break;

		case sf::Event::MouseButtonReleased:
		case sf::Event::MouseMoved:
			mouse_pos_ = sf::Vector2f(sf::Mouse::getPosition(window_));
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				for (auto & t : tiles_)
				{
					if (is_in(mouse_pos_ - tiles_offset_, t))
					{
						t.rect.setFillColor(sf::Color(200, 200, 200));
					}
					else
					{
						t.rect.setFillColor(sf::Color::White);
					}

				}
				redraw_ = true;
			}
			break;
		case sf::Event::Resized:
			sf::View v(sf::FloatRect(0, 0, event.size.width, event.size.height));
			window_.setView(v);
			redraw_ = true;
		}
	}
}

void update()
{
	if (window_.hasFocus())
	{
		if (mouse_pos_.x < 10)
		{
			tiles_offset_.x += 5;
			redraw_ = true;
		}
		if (mouse_pos_.x > window_.getSize().x - 10)
		{
			tiles_offset_.x -= 5;
			redraw_ = true;
		}

		if (mouse_pos_.y < 10)
		{
			tiles_offset_.y += 5;
			redraw_ = true;
		}
		if (mouse_pos_.y > window_.getSize().y - 10)
		{
			tiles_offset_.y -= 5;
			redraw_ = true;
		}

	}
}


void redraw()
{
	if (redraw_)
	{
		window_.clear(sf::Color::Black);
		for (int i = 0; i < tiles_.size(); i++)
		{
			auto rect = tiles_[i].rect;
			rect.move(tiles_offset_);
			window_.draw(rect);

			switch (map_[tiles_[i].i])
			{
			case X:
			{
				auto circle = sf::CircleShape(TileSize / 2 - 1);
				circle.setPosition(rect.getPosition() + sf::Vector2f(1, 1));
				circle.setFillColor(sf::Color::Red);
				window_.draw(circle);
				break;
			}
			case O:
			{
				auto circle = sf::CircleShape(TileSize / 2 - 1);
				circle.setPosition(rect.getPosition() + sf::Vector2f(1, 1));
				circle.setFillColor(sf::Color::Blue);
				window_.draw(circle);
				break;
			}
			}
		}

		/*auto a = sf::RectangleShape(sf::Vector2f(40, 40));
		a.setFillColor(sf::Color(0, 0, 0, 200));
		*/
		auto circle = sf::CircleShape(15);
		circle.setPosition(sf::Vector2f(5, 5));
		if (turn_ == O)
			circle.setFillColor(sf::Color(0,0, 255, 200));
		else if (turn_ == X)
			circle.setFillColor(sf::Color(255, 0, 0, 200));
		//window_.draw(a);
		window_.draw(circle);


		window_.display();
		redraw_ = false;
	}
}
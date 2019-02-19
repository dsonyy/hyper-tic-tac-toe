#pragma once
#include <functional>
#include <string>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Main.hpp>
#include "ttt.h"

const unsigned WIDTH = 720;
const unsigned HEIGHT = 480;
const std::string TITLE = "Hyper Tic-Tac-Toe";
const sf::Uint32 STYLE = sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize;
const sf::Int32 FRAME_RATE = 60;

const float TILE_SIZE = 18;
const float TileNOffset = 6;

const float MOVE_SPEED = 4;

const sf::Color BG_COLOR = sf::Color(0, 0, 20);
const sf::Color FG_COLOR = sf::Color(0, 0, 20);
const sf::Color TEXT_COLOR = sf::Color(255, 255, 255);
const sf::Color TEXT2_COLOR = sf::Color(100, 100, 100);
const sf::Color O_COLOR = sf::Color(255, 0, 0);
const sf::Color X_COLOR = sf::Color(0, 0, 255);
const sf::Color Y_COLOR = sf::Color(50, 50, 50);
const sf::Color Z_COLOR = sf::Color(0, 180, 0);

const sf::Vector2f MENU_WINDOW_SIZE = sf::Vector2f(300, 400);
const sf::Vector2f MENU_BUTTON_SIZE = sf::Vector2f(30, 30);

const size_t MAX_N = 8;

struct Tile
{
	size_t i;
	sf::RectangleShape rect;
	VMapPos dim;
};

struct State
{
	typedef std::string ID;

	ID id;
	std::function<void()> init;
	std::function<void()> update;
	std::function<void()> redraw;
	std::function<void()> handle_input;
};

struct Button
{
	typedef std::string ID;

	ID id;
	sf::RectangleShape shape;
	sf::Text text;
	std::function<void()> action;

	Button(sf::Font & font)
	{
		shape.setSize(MENU_BUTTON_SIZE);
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color::Black);
		text.setFont(font);
		text.setCharacterSize(18);
		text.setFillColor(sf::Color::Black);
		text.setStyle(sf::Text::Bold);
	}
};



void draw_map(sf::RenderWindow & window, const Map & map, const std::vector<Tile> & tiles);
void draw_turn(sf::RenderWindow & window, Field turn);
void draw_coords(sf::RenderWindow & window, VMapPos vpos);
void draw_dialog(sf::RenderWindow & window, std::string str, sf::Color color = TEXT_COLOR);
void draw_legend(sf::RenderWindow & window);

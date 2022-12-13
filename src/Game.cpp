#include "Game.h"
#include <iostream>
using namespace luabridge;

Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{ false },
	_player()
{
	auto shape = new sf::RectangleShape(sf::Vector2f(10, 10));
	shape->setFillColor(sf::Color::Black);
	_player.addComponents(position{ sf::Vector2f(40, 30) }, renderable{ shape }, player{ "player" });
	_player.addComponent(velocity{ sf::Vector2f{40,40} });
	L = luaL_newstate();
	if (!m_font.loadFromFile("assets/font/ariblk.ttf"))
	{
		std::cout << "Could not load font";
	}
	m_text.setFont(m_font);
	m_text.setCharacterSize(15);
	m_text.setFillColor(sf::Color::Red);
	m_text.setPosition(sf::Vector2f(10, 350));

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	getGlobalNamespace(L)
		.beginClass<Entity>("Entity")
		.addConstructor<void(*)()>()
		.addFunction("update", &Entity::update)
		.endClass();
	luaL_dofile(L, "assets/scripts/script.lua");
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);

	LuaRef window = getGlobal(L, "window");
	LuaRef size = window["size"];
	std::string titleString = window["title"].cast<std::string>();
	int width = size["width"].cast<int>();
	int height = size["height"].cast<int>();
	m_window.setTitle(titleString);
	m_window.setSize(sf::Vector2u{ (unsigned int)width, (unsigned int)height });
}

Game::~Game()
{
	lua_close(L);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}

		processKeys(newEvent);
	}
}


void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (t_event.type == sf::Event::KeyPressed)
	{
		directions.moveUp = sf::Keyboard::W == t_event.key.code ? true : directions.moveUp;
		directions.moveDown = sf::Keyboard::S == t_event.key.code ? true : directions.moveDown;
		directions.moveLeft = sf::Keyboard::A == t_event.key.code ? true : directions.moveLeft;
		directions.moveRight = sf::Keyboard::D == t_event.key.code ? true : directions.moveRight;
	}
	else if(t_event.type == sf::Event::KeyReleased)
	{
		if(sf::Keyboard::W == t_event.key.code)
			directions.moveUp = false;
		else if(sf::Keyboard::S == t_event.key.code)
			directions.moveDown = false;
		else if(sf::Keyboard::A == t_event.key.code)
			directions.moveLeft = false;
		else if(sf::Keyboard::D == t_event.key.code)
			directions.moveRight = false;
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	auto view = registry.view<position, player, velocity>();
	view.each([&](position& pos, player& player, velocity& vel)
	{
		auto movement = sf::Vector2f{0,0};
		if(directions.moveUp)
		{
			movement.y -= vel.vel.y * t_deltaTime.asSeconds();
		}
		else if(directions.moveDown)
		{
			movement.y += vel.vel.y * t_deltaTime.asSeconds();
		}

		if (directions.moveRight)
		{
			movement.x += vel.vel.x * t_deltaTime.asSeconds();
		}
		else if (directions.moveLeft)
		{
			movement.x -= vel.vel.x * t_deltaTime.asSeconds();
		}
		pos.pos += movement;
	});
}

void Game::render()
{
	m_window.clear(sf::Color::White);
	auto view = registry.view<position, renderable>();
	view.each([&](const position& pos, const renderable& render)
	{
		render.shape->setPosition(pos.pos);
		m_window.draw(*render.shape);
	});
	m_window.display();
}

void Game::printMessage(const std::string& s)
{
	std::cout << s << std::endl;
}

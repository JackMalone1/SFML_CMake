#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "entt.hpp"
#include "Components.h"
#include "Lua.hpp"
#include "Entity.h"
#include "Globals.h"
#include <LuaBridge.h>

struct MovementDirections
{
	bool moveUp = false;
	bool moveDown = false;
	bool moveLeft = false;
	bool moveRight = false;
};

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	void printMessage(const std::string& s);
private:
	sf::RenderWindow m_window; // main SFML window
	bool m_exitGame; // control exiting game
	MovementDirections directions;
	lua_State* L;
	sf::Text m_text;
	sf::Font m_font;
	Entity _player;
};

#endif // !GAME_HPP
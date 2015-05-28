#ifndef __MENU__
#define __MENU__

#include <vector>

#include "Utils.h"
#include "Stream.h"

class MenuItem
{
	private:
		std::string Text;

	public:
		MenuItem();
		MenuItem(std::string val);

		virtual void Draw(sf::RenderWindow* win, std::string prefix = "", sf::Vector2f pos = sf::Vector2f(0, 0), bool highlight = false);
		
		std::string GetText();
		void SetText(std::string val);
};

class Menu
{
	private:
		std::vector<MenuItem> Items;
		int currentItem, firstItem;

	public:
		Menu();

		virtual void Reset();
		virtual void Clear();

		virtual void AddItem(std::string val);
		virtual void AddItem(MenuItem val);

		virtual void Draw(sf::RenderWindow* win);

		virtual MenuItem GetItem(unsigned int index) const;
		int GetItemNum() const;

		virtual int Update(sf::Event evt);
};

class MenuManager
{
	private:
		std::vector<Menu> Items;
		int currentMenu;

	public:
		MenuManager();

		void AddMenu(Menu val);

		void Clear();

		void DropMenu(unsigned int index);

		Menu GetMenu(unsigned int index) const;
		int GetCurrentMenuIndex() const;

		void ChangeMenu(int index);

		Menu GetCurrentMenu() const;

		int Update(sf::Event evt);
};

#endif
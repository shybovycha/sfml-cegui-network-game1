#include "Menu.h"

MenuItem::MenuItem()
{
	Text = "";
}

MenuItem::MenuItem(std::string val)
{
	Text = val;
}

void MenuItem::Draw(sf::RenderWindow* win, std::string prefix, sf::Vector2f pos, bool highlight)
{
	sf::String val(prefix + Text);
	val.SetSize(18);
	val.SetPosition(pos.x, pos.y);

	if (highlight)
		val.SetColor(sf::Color(255, 0, 0));

	win->Draw(val);
}

std::string MenuItem::GetText()
{
	return Text;
}

void MenuItem::SetText(std::string val)
{
	Text = val;
}

// ==================================================================

Menu::Menu()
{
	currentItem = -1;
	firstItem = 0;
}

void Menu::Reset()
{
	currentItem = -1;
	firstItem = 0;
}

void Menu::Clear()
{
	Items.clear();
}

void Menu::AddItem(std::string val)
{
	Items.push_back(MenuItem(val));
}

void Menu::AddItem(MenuItem val)
{
	Items.push_back(val);
}

void Menu::Draw(sf::RenderWindow* win)
{
	if (firstItem > -1)
	{
		for (unsigned int i = (unsigned int) firstItem; i < (((unsigned int) firstItem + 9 < Items.size()) ?  (unsigned int) firstItem + 9 : Items.size()); i++)
		{
			if (i == firstItem)
				Items[i].Draw(win, "[" + IntToStr(i - firstItem + 1) + "]. ", sf::Vector2f(win->GetWidth() / 2.0f, (i - firstItem) * 20.0f), true); else
					Items[i].Draw(win, "[" + IntToStr(i - firstItem + 1) + "]. ", sf::Vector2f(win->GetWidth() / 2.0f, (i - firstItem) * 20.0f));
		}
	}
}

MenuItem Menu::GetItem(unsigned int index) const
{
	if (index < Items.size())
		return Items[index]; else
			return MenuItem();
}

int Menu::GetItemNum() const
{
	return Items.size();
}

int Menu::Update(sf::Event evt)
{
	if (evt.Type == sf::Event::KeyPressed)
	{
		if (currentItem < 0)
		{
			if (evt.Key.Code >= sf::Key::Num1 && evt.Key.Code <= sf::Key::Num9)
				currentItem = firstItem + int(evt.Key.Code) - int(sf::Key::Num1);

			if (evt.Key.Code == sf::Key::Up)
				firstItem--; else
			if (evt.Key.Code == sf::Key::Down)
				firstItem++;

			if (evt.Key.Code == sf::Key::Return)
				currentItem = firstItem;

			if (firstItem < 0)
				firstItem = 0; else
			if (firstItem > (int) Items.size() - 1)
				firstItem = (int) Items.size() - 1;

			if (currentItem < -1 || currentItem > (int) Items.size() - 1)
				currentItem = -1;
		} else
		{
			if (evt.Key.Code == sf::Key::Return)
				currentItem = -1;
		}
	}

	return currentItem;
}

// ==================================================================

MenuManager::MenuManager()
{
	currentMenu = 0;
}

void MenuManager::AddMenu(Menu val)
{
	Items.push_back(val);
}

void MenuManager::Clear()
{
	Items.clear();
}

void MenuManager::DropMenu(unsigned int index)
{
	if (index < Items.size())
		Items.erase(Items.begin() + index);
}

Menu MenuManager::GetMenu(unsigned int index) const
{
	if (index < Items.size())
		return Items[index]; else
			return Menu();
}

int MenuManager::GetCurrentMenuIndex() const
{
	return currentMenu;
}

void MenuManager::ChangeMenu(int index)
{
	if (currentMenu > -1 && currentMenu < (int) Items.size() && Items.size() > 0)
		Items[currentMenu].Reset();

	currentMenu = index;
}

Menu MenuManager::GetCurrentMenu() const
{
	if (currentMenu > -1 && currentMenu < (int) Items.size() && Items.size() > 0)
		return Items[currentMenu]; else
			return Menu();
}

int MenuManager::Update(sf::Event evt)
{
	if (currentMenu > -1 && currentMenu < (int) Items.size() && Items.size() > 0)
		return Items[currentMenu].Update(evt); else
			return -1;
}
#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <CEGUI.h>
#include <RendererModules/OpenGL/CEGUIOpenGLRenderer.h>
#include <CEGUIDefaultResourceProvider.h>

#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#define DEBUG_MODE

#ifdef DEBUG_MODE
	#pragma comment(lib, "sfml-main-d.lib")
	#pragma comment(lib, "sfml-system-d.lib")
	#pragma comment(lib, "sfml-graphics-d.lib")
	#pragma comment(lib, "sfml-window-s-d.lib")

	#pragma comment(lib, "CEGUIBase_d.lib")
	#pragma comment(lib, "CEGUIOpenGLRenderer_d.lib")
#else
	#pragma comment(lib, "sfml-main.lib")
	#pragma comment(lib, "sfml-system.lib")
	#pragma comment(lib, "sfml-graphics.lib")
	#pragma comment(lib, "sfml-window-s.lib")

	#pragma comment(lib, "CEGUIBase.lib")
	#pragma comment(lib, "CEGUIOpenGLRenderer.lib")
#endif

#pragma comment(lib, "glu32.lib")

#include "Utils.h"
#include "Map.h"

Map map;

bool RaiseHP(const CEGUI::EventArgs& e)
{
	if (map.Players[0].GetSkillPoints() > 0)
	{
		map.Players[0].SetMaxHP(map.Players[0].GetMaxHP() + (map.Players[0].GetLevel() * 10));
		map.Players[0].SetSkillPoints(map.Players[0].GetSkillPoints() - 1);
	}

	return true;
}

bool RaiseMP(const CEGUI::EventArgs& e)
{
	if (map.Players[0].GetSkillPoints() > 0)
	{
		map.Players[0].SetMaxMP(map.Players[0].GetMaxMP() + (map.Players[0].GetLevel() * 10));
		map.Players[0].SetSkillPoints(map.Players[0].GetSkillPoints() - 1);
	}

	return true;
}

bool RaiseAP(const CEGUI::EventArgs& e)
{
	if (map.Players[0].GetSkillPoints() > 0)
	{
		map.Players[0].SetAttack(map.Players[0].GetAttack() + map.Players[0].GetLevel());
		map.Players[0].SetSkillPoints(map.Players[0].GetSkillPoints() - 1);
	}

	return true;
}

bool RaiseAR(const CEGUI::EventArgs& e)
{
	if (map.Players[0].GetSkillPoints() > 0)
	{
		map.Players[0].SetARadius(map.Players[0].GetARadius() + 1);
		map.Players[0].SetSkillPoints(map.Players[0].GetSkillPoints() - 1);
	}

	return true;
}

bool RaiseWR(const CEGUI::EventArgs& e)
{
	if (map.Players[0].GetSkillPoints() > 0)
	{
		map.Players[0].SetWRadius(map.Players[0].GetWRadius() + 1);
		map.Players[0].SetSkillPoints(map.Players[0].GetSkillPoints() - 1);
	}

	return true;
}

bool SPToXP(const CEGUI::EventArgs& e)
{
	if (map.Players[0].GetSkillPoints() > 0)
	{
		map.Players[0].SetXP(map.Players[0].GetXP() + (map.Players[0].GetSkillPoints() * map.Players[0].GetLevel()));
		map.Players[0].SetSkillPoints(0);
	}

	return true;
}

int main()
{
	sf::RenderWindow App(sf::VideoMode(800, 600, 32), "MyDemo");
    App.PreserveOpenGLStates(true);

	// ========== Set-up CEGUI ===========
	CEGUI::OpenGLRenderer& myRenderer = CEGUI::OpenGLRenderer::create();

	try
	{
		CEGUI::System::create( myRenderer );
	}
	catch (CEGUI::Exception& exc)
    {
		std::cout << exc.getMessage().c_str() << std::endl;
    }
	catch (std::exception& exc)
    {
		std::cout << exc.what() << std::endl;
    }
    catch (const char* exc)
    {
        std::cout << exc << std::endl;
    }
    catch(...)
    {
        std::cout << "Unknown exception was caught!" << std::endl;
    }

	// initialise the required dirs for the DefaultResourceProvider
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());

	rp->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
	rp->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
	rp->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
	rp->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");

	// This is only needed if you are using Xerces and need to
	// specify the schemas location
	// setup default group for validation schemas
	rp->setResourceGroupDirectory("schemas", "../datafiles/xml_schemas/");

	CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();

	if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
		parser->setProperty("SchemaDefaultResourceGroup", "schemas");

	// set the default resource groups to be used
	CEGUI::Imageset::setDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::DefaultWindow* root = (CEGUI::DefaultWindow*) winMgr.loadWindowLayout("Character.layout");
    CEGUI::System::getSingleton().setGUISheet(root);

	CEGUI::System::getSingleton().setDefaultTooltip("TaharezLook/Tooltip");

	winMgr.getWindow("RootWin")->setAlpha(0.9f);

	for (int i = 1; i < 8; i++)
		winMgr.getWindow("Root/PB" + IntToStr(i))->setEnabled(false);

	winMgr.getWindow("Root/PB1")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(RaiseHP));
	winMgr.getWindow("Root/PB2")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(RaiseMP));
	winMgr.getWindow("Root/PB3")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(RaiseAP));
	winMgr.getWindow("Root/PB4")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(RaiseAR));
	winMgr.getWindow("Root/PB5")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(RaiseWR));
	winMgr.getWindow("Root/XPBar")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(SPToXP));

	// ============ Some variables ===============
	sf::Clock Clock;
	sf::Vector3f CamPos;

	// ======= Create some players =======
	unsigned int p1, p2;

	p1 = map.AddPlayer('b');
	p2 = map.AddPlayer('a');

	map.Prepare();

	// ======= OpenGL settings =======
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, 1.f, 1.f, 500.f);

	glClearColor(0.0f, 0.5f, 1.0f, 0.5f);

	// ======= Set camera position =======
	CamPos = sf::Vector3f(0, 100, 75);

	App.ShowMouseCursor(false);

	while (App.IsOpened())
    {
        sf::Event Event;

		// ============ Inject CEGUI inputs ================
		CEGUI::System::getSingleton().injectMousePosition(App.GetInput().GetMouseX(), App.GetInput().GetMouseY());
		CEGUI::System::getSingleton().injectTimePulse(App.GetFrameTime());

		// ========= Handle events =============
		while (App.GetEvent(Event))
        {
	        if (Event.Type == sf::Event::Resized)
                glViewport(0, 0, Event.Size.Width, Event.Size.Height);

			if (Event.Type == sf::Event::Closed || App.GetInput().IsKeyDown(sf::Key::Escape))
				App.Close();

			if (App.GetInput().IsKeyDown(sf::Key::W))
				CamPos.z -= 3;

			if (App.GetInput().IsKeyDown(sf::Key::S))
				CamPos.z += 3;

			if (App.GetInput().IsKeyDown(sf::Key::A))
				CamPos.x -= 3;

			if (App.GetInput().IsKeyDown(sf::Key::D))
				CamPos.x += 3;

			// ================== Update CEGUI ====================
			if (Event.Type == sf::Event::MouseButtonPressed)
			{
				if (Event.MouseButton.Button == sf::Mouse::Left)
					CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::LeftButton); else
				if (Event.MouseButton.Button == sf::Mouse::Right)
					CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::RightButton); else
				if (Event.MouseButton.Button == sf::Mouse::Middle)
					CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton::MiddleButton);
			} else
			if (Event.Type == sf::Event::MouseButtonReleased)
			{
				if (Event.MouseButton.Button == sf::Mouse::Left)
					CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::LeftButton); else
				if (Event.MouseButton.Button == sf::Mouse::Right)
					CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::RightButton); else
				if (Event.MouseButton.Button == sf::Mouse::Middle)
					CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton::MiddleButton);
			}

			if (Event.Type == sf::Event::MouseButtonReleased)
			{
				bool overWindow = (CEGUI::System::getSingleton().getWindowContainingMouse()->getName().find("Root/") == std::string::npos) && 
					(CEGUI::System::getSingleton().getWindowContainingMouse()->getName().find("RootWin") == std::string::npos);
				
				if (Event.MouseButton.Button == sf::Mouse::Left && overWindow)
				{
					sf::Vector3f v = ScreenToSpace(App.GetInput().GetMouseX(), App.GetInput().GetMouseY());

					int x1, z1; // cell indexes
					float x2, z2; // cell normal basis

					map.SpaceToCell(v.x, v.z, x1, z1, x2, z2);

					map.PlayerMove(x1, z1, 0);
				}
			}
		}
		
		//  ======= Clear OpenGL state ======= 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (true)
		{
			//  ======= Update camera view ======= 
			/*gluLookAt(Players[0].Model.GetPosition().GetX(), Players[0].Model.GetPosition().GetY() + 100, Players[0].Model.GetPosition().GetZ() + 75, 
					  Players[0].Model.GetPosition().GetX(), Players[0].Model.GetPosition().GetY(), Players[0].Model.GetPosition().GetZ(), 
					  0, 1, 0);*/

			gluLookAt(CamPos.x, CamPos.y, CamPos.z,
					  CamPos.x, CamPos.y - 100, CamPos.z - 75, 
					  0, 1, 0);

			map.Draw();

			//  ======= Pick terrain point at mouse cursor position ======= 
			{
				sf::Vector3f v = ScreenToSpace(App.GetInput().GetMouseX(), App.GetInput().GetMouseY());

				int x1, z1; // cell indexes
				float x2, z2; // cell normal basis

				map.SpaceToCell(v.x, v.z, x1, z1);
				map.CellToSpace(x1, z1, x2, z2);

				//  ======= Draw normal ======= 
				glBegin(GL_LINES);
					glVertex3f(x2, 0, z2);
					glVertex3f(x2, 150, z2);
				glEnd();
			}

			// ======= Render models ======= 
			float time = Clock.GetElapsedTime();

			map.DrawPlayers(time);

			// ============== Set the progress for player's XP ==============
			int lvl = map.Players[0].GetLevel();
			int currXP = map.Players[0].GetXP();
			int sp = map.Players[0].GetSkillPoints();

			static int oldXP = -1;
			std::string s;

			if (sp <= 0)
			{
				if (!winMgr.getWindow("Root/PB1")->isDisabled())
					winMgr.getWindow("Root/PB1")->setEnabled(false);

				if (!winMgr.getWindow("Root/PB2")->isDisabled())
					winMgr.getWindow("Root/PB2")->setEnabled(false);

				if (!winMgr.getWindow("Root/PB3")->isDisabled())
					winMgr.getWindow("Root/PB3")->setEnabled(false);

				if (!winMgr.getWindow("Root/PB4")->isDisabled())
					winMgr.getWindow("Root/PB4")->setEnabled(false);

				if (!winMgr.getWindow("Root/PB5")->isDisabled())
					winMgr.getWindow("Root/PB5")->setEnabled(false);

				if (!winMgr.getWindow("Root/PB6")->isDisabled())
					winMgr.getWindow("Root/PB6")->setEnabled(false);

				if (!winMgr.getWindow("Root/PB7")->isDisabled())
					winMgr.getWindow("Root/PB7")->setEnabled(false);
			}

			if (oldXP != currXP)
			{
				if (sp <= 0)
				{
					winMgr.getWindow("Root/Descr")->setText("Description: --");
				}

				int maxXP = (pow(2.f, (float) lvl) * 20); //100);  + (50 * (lvl + 1));
				float val = (float) maxXP / (float) ((currXP == 0) ? maxXP : currXP) / 100.f;

				if (currXP >= maxXP)
				{
					map.Players[0].SetLevel(++lvl);
					map.Players[0].SetSkillPoints(map.Players[0].GetSkillPoints() + 5 + (lvl * lvl / 25));

					winMgr.getWindow("Root/Descr")->setText("Description: you've \n got level up! Click on XP bar to \n convert SP => XP (+bonuses!)");

					winMgr.getWindow("Root/PB1")->setEnabled(true);
					winMgr.getWindow("Root/PB2")->setEnabled(true);
					winMgr.getWindow("Root/PB3")->setEnabled(true);
					winMgr.getWindow("Root/PB4")->setEnabled(true);
					winMgr.getWindow("Root/PB5")->setEnabled(true);
					winMgr.getWindow("Root/PB6")->setEnabled(true);
					winMgr.getWindow("Root/PB7")->setEnabled(true);

					maxXP = (pow(2.f, (float) lvl) * 20); //100);  + (50 * (lvl + 1));
					val = (float) maxXP / (float) ((currXP == 0) ? maxXP : currXP) / 100.f;
				}

				s = "XP: " + IntToStr(currXP) + " / " + IntToStr(maxXP);

				static_cast<CEGUI::ProgressBar*>(CEGUI::WindowManager::getSingleton().getWindow("Root/XPBar"))->setProgress(val);
				static_cast<CEGUI::ProgressBar*>(CEGUI::WindowManager::getSingleton().getWindow("Root/XPBar"))->setTooltipText(s);

				oldXP = currXP;
			}

			s = "HP: " + IntToStr(map.Players[0].GetHP());
			winMgr.getWindow("Root/HP")->setText(s);

			if (map.Players[0].GetMaxMP() > 0)
			{
				s = "MP: " + IntToStr(map.Players[0].GetMP());
				winMgr.getWindow("Root/MP")->setText(s);
			}

			s = "Lvl: " + IntToStr(map.Players[0].GetLevel());
			winMgr.getWindow("Root/Lvl")->setText(s);

			s = "AP: " + IntToStr(map.Players[0].GetAttack());
			winMgr.getWindow("Root/AP")->setText(s);

			s = "AR: " + IntToStr(map.Players[0].GetARadius());
			winMgr.getWindow("Root/AR")->setText(s);

			s = "WR: " + IntToStr(map.Players[0].GetWRadius());
			winMgr.getWindow("Root/WR")->setText(s);
		}

		// ============== Render GUI ==============
		glDisable(GL_DEPTH_TEST);
		CEGUI::System::getSingleton().renderGUI();
		glEnable(GL_DEPTH_TEST);

		App.Display();
    }

	return EXIT_SUCCESS;
}
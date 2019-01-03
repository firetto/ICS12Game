// the main source file where the main function is

#include <SFML/Graphics.hpp>
#include "GameSetup.h"
#include "GameUpdate.h"
#include "GameDraw.h"
#include "MainMenuAnimation.h"

int main() { // main method
	// window setup
	windowSetup();

	// load font
	fontLoad();

	// game setup
	gameLoader.gameSetup();

	// creates a player object
	Player player;
	globalPlayer = &player;

	// set up buttons and GUIs
	guiSetup();

	// while the displayed window is open
	while (WINDOW.isOpen()) { 
		// adds the time elapsed since last time the clock was restarted to the sf::time
		WINDOW_TIME_SINCE_LAST_UPDATE += WINDOW_FRAME_CLOCK.restart();

		while (WINDOW.pollEvent(event)) {
			if (event.type == sf::Event::Closed) WINDOW.close();
			if (GAME_RUNNING) {
				if (event.type == sf::Event::KeyPressed) {
					switch (event.key.code) {
					case sf::Keyboard::M:
						GAME_MAP_OPEN = !GAME_MAP_OPEN; break;
					case sf::Keyboard::Escape:
						if (GAME_MAP_OPEN) GAME_MAP_OPEN = false;
						else pauseGame(); 
						break;
					}
				}
				if (!GAME_MAP_OPEN) {
					player.items.eventUpdateBar();
					craftHUD.update(player.items);
					player.eventUpdate();
				}
			}
			if (isFullscreen != wasFullscreen) {
				wasFullscreen = isFullscreen;
				if (!isFullscreen) WINDOW_DIMENSIONS = defaultWindowDimension;
				updateResolution();
			}
		}

		//Lets the physics & etc. run if the time elapsed since last time physics ran was more than the time per frame
		while (WINDOW_TIME_SINCE_LAST_UPDATE > WINDOW_TIME_PER_FRAME) {  
			drawableVector.clear();
			
			if (GAME_RUNNING) {
				// update the game physics
				gameUpdate(player);
			}

			// once everything is done, subtracts the time per frame from the time since last update
			WINDOW_TIME_SINCE_LAST_UPDATE -= WINDOW_TIME_PER_FRAME; 
		}
		// updates game click delay
		if (!GAME_CLICK_DELAY_PASSED && GAME_CLICK_DELAY_CLOCK.getElapsedTime().asSeconds() >= GAME_CLICK_DELAY_TIME) {
			GAME_CLICK_DELAY_PASSED = true;
			GAME_CLICK_DELAY_CLOCK.restart();
		}
		else if (GAME_CLICK_DELAY_PASSED) GAME_CLICK_DELAY_CLOCK.restart();




		//
		// draws the game
		if (GAME_RUNNING) gameDraw(player);
		else {
			WINDOW.clear();
			mainMenuAnimation.update();
			currentGUIScreen->update();
			WINDOW.display();
		}

		
	}
}
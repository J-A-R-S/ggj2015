/*
 * Bank Robbery for Dummies
 * Copyright (C) 2015 JARS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <game/base/Random.h>
#include <game/graphics/Group.h>
#include <game/graphics/Action.h>
#include <game/base/Log.h>

#include "local/Car.h"

#include "local/Map.h"

#include "config.h"

int main() {
  // initialize

  static constexpr unsigned INITIAL_WIDTH = 1024;
  static constexpr unsigned INITIAL_HEIGHT = 768;

  sf::RenderWindow window(sf::VideoMode(INITIAL_WIDTH, INITIAL_HEIGHT), "Bank Robbery for Dummies (version " GAME_VERSION ")");
  window.setKeyRepeatEnabled(false);

  game::Log::setLevel(game::Log::DEBUG);

  // load resources

  // add actions
  game::ActionSet actions;

  auto closeWindow = std::make_shared<game::Action>("Close window");
  closeWindow->addCloseControl();
  closeWindow->addKeyControl(sf::Keyboard::Escape);
  actions.addAction(closeWindow);

  auto carAcceleration = std::make_shared<game::Action>("Car accelerate");
  carAcceleration->addKeyControl(sf::Keyboard::Up);
  carAcceleration->setContinuous();
  actions.addAction(carAcceleration);

  auto carBrake = std::make_shared<game::Action>("Car brake");
  carBrake->addKeyControl(sf::Keyboard::Down);
  carBrake->setContinuous();
  actions.addAction(carBrake);

  auto carTurnLeft = std::make_shared<game::Action>("Car turn left");
  carTurnLeft->addKeyControl(sf::Keyboard::Left);
  carTurnLeft->setContinuous();
  actions.addAction(carTurnLeft);

  auto carTurnRight = std::make_shared<game::Action>("Car turn right");
  carTurnRight->addKeyControl(sf::Keyboard::Right);
  carTurnRight->setContinuous();
  actions.addAction(carTurnRight);

  game::Random random(0);

  // add entities
  game::Group group;

  // add map
  Map map;
  map.generate(random);
  group.addEntity(map);

  // add cars
  Car car;
  group.addEntity(car);

  // main loop
  sf::Clock clock;

  sf::View view({ 128.0f * Map::SIZE, 128.0f * Map::SIZE }, { 15 * INITIAL_WIDTH, 15 * INITIAL_HEIGHT });

  while (window.isOpen()) {
    // input
    sf::Event event;

    while (window.pollEvent(event))	{
      actions.update(event);
    }

    // actions
    if (closeWindow->isActive()) {
		window.close();
	}

	if (carAcceleration->isActive()) {
		car.accelerate();
	}

	if (carBrake->isActive()) {
        car.brake();
    }

	if (carTurnLeft->isActive()) {
        car.turnLeft();
    }

    if (carTurnRight->isActive()) {
        car.turnRight();
    }

	actions.reset();

    // update
    sf::Time elapsed = clock.restart();
    group.update(elapsed.asSeconds());

    // render
    window.setView(view);
    window.clear(sf::Color::White);
    group.render(window);
    window.display();
  }

  return 0;
}

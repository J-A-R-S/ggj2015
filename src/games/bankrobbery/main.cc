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
#include <game/base/Event.h>
#include <game/base/Log.h>
#include <game/base/Random.h>
#include <game/graphics/Action.h>
#include <game/graphics/Group.h>
#include <game/graphics/Resource.h>

#include "local/Car.h"
#include "local/Events.h"
#include "local/Map.h"
#include "local/Scenario.h"

#include "config.h"

int main() {
  // initialize

  static constexpr unsigned INITIAL_WIDTH = 1024;
  static constexpr unsigned INITIAL_HEIGHT = 768;

  sf::RenderWindow window(sf::VideoMode(INITIAL_WIDTH, INITIAL_HEIGHT), "Bank Robbery for Dummies (version " GAME_VERSION ")");
  window.setKeyRepeatEnabled(false);

  game::Log::setLevel(game::Log::DEBUG);
  game::Log::debug(game::Log::GENERAL, "Path install : %s\n", GAME_DATADIR);

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

  std::random_device dev;
  unsigned seed = dev();
  game::Log::info(game::Log::GENERAL, "Seed: %u\n", seed);
  game::Random random(seed);


  // create resource manager
  game::ResourceManager resources;
  resources.addSearchDir(GAME_DATADIR);

  // add entities
  b2Vec2 gravity(0.0f, 0.0f);
  b2World world(gravity);

  game::EventManager events;
  game::Group group;

  // add map
  Map map(resources);
  map.generate(random, world);
  group.addEntity(map);

  // add cars
  Car car(events, resources, world);
  group.addEntity(car);

  // add Scenario
  Scenario scenario(events, resources);
  group.addEntity(scenario);

  // main loop
  sf::Clock clock;

  sf::View view({ 128.0f * Map::SIZE, 128.0f * Map::SIZE }, { 2.0f * INITIAL_WIDTH, 2.0f * INITIAL_HEIGHT });

  events.registerHandler<HeroPositionEvent>([&view](game::EventType type, game::Event *event) {
    auto e = static_cast<HeroPositionEvent*>(event);
    view.setCenter({ e->pos.x, e->pos.y });
    return game::EventStatus::KEEP;
  });

  while (window.isOpen()) {
    // input
    sf::Event event;

    while (window.pollEvent(event))	{
      actions.update(event);

      if (event.type == sf::Event::Resized) {
        view.setSize(2.0f * event.size.width, 2.0f * event.size.height);
      }
    }

    // actions
    if (closeWindow->isActive()) {
      window.close();
    }

    if (carAcceleration->isActive()) {
      car.accelerate();
    } else if (carBrake->isActive()) {
      car.brake();
    } else {
      car.cruise();
    }

    if (carTurnLeft->isActive()) {
      car.turnLeft();
    } else if (carTurnRight->isActive()) {
      car.turnRight();
    } else {
      car.turnNone();
    }

    actions.reset();

    // update
    sf::Time elapsed = clock.restart();
    float dt = elapsed.asSeconds();

    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    world.Step(dt, velocityIterations, positionIterations);

    group.update(dt);

    // render
    window.setView(view);
    window.clear(sf::Color::White);
    group.render(window);
    window.display();
  }

  return 0;
}

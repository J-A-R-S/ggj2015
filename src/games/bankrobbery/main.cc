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

#include "local/Map.h"

#include "config.h"

int main() {
  // initialize

  static constexpr unsigned INITIAL_WIDTH = 1024;
  static constexpr unsigned INITIAL_HEIGHT = 768;

  sf::RenderWindow window(sf::VideoMode(INITIAL_WIDTH, INITIAL_HEIGHT), "Bank Robbery for Dummies (version " GAME_VERSION ")");
  window.setKeyRepeatEnabled(false);

  // load resources
  game::Random random(0);

  // add entities
  game::Group group;

  Map map;
  map.generate(random);
  group.addEntity(map);


  // main loop
  sf::Clock clock;

  sf::View view({ 128.0f * Map::SIZE, 128.0f * Map::SIZE }, { 15 * INITIAL_WIDTH, 15 * INITIAL_HEIGHT });

  while (window.isOpen()) {
    // input
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::KeyPressed) {

        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window.close();
            break;

          default:
            break;
        }

      }
    }

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
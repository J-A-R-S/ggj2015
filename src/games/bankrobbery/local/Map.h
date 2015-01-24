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
#ifndef LOCAL_MAP_H
#define LOCAL_MAP_H

#include <array>

#include <SFML/Graphics.hpp>

#include <game/base/Random.h>
#include <game/graphics/Entity.h>
#include <game/model/Physics.h>

class Map : public game::Entity {
public:
  static constexpr std::size_t NUMBER_OF_STREETS = 15;
  static constexpr std::size_t SIZE = NUMBER_OF_STREETS * 3;

  enum Kind {
    STREET,
    ROADBLOCK,
    BUILDING,
    GRASS,
  };

  struct Block {
    Kind kind;
    int number;
    game::Body body;
  };

  Map();

  void generate(game::Random& random, game::Physics& physics);

  virtual void update(float dt) override;

  virtual void render(sf::RenderWindow& window) override;

private:
  std::array<std::array<Block, SIZE>, SIZE> m_map;
  sf::VertexArray m_array;
};

#endif // LOCAL_MAP_H

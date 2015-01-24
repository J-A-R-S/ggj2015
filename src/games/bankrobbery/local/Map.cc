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
#include "Map.h"

#include <iostream>

Map::Map()
: m_array(sf::Quads) {
}

static constexpr float TILESIZE = 256.0f;

void Map::generate(game::Random& random) {
  for (std::size_t i = 0; i < SIZE; ++i) {
    for (std::size_t j = 0; j < SIZE; ++j) {
      auto& block = m_map[i][j];

      if (i == 0 || j == 0 || i == SIZE - 1 || j == SIZE - 1) {
        block.kind = GRASS;
        block.number = 0;
      } else if (i % 3 == 1 || j % 3 == 1) {
        block.kind = STREET;
        block.number = 0;
      } else {
        block.kind = BUILDING;
        block.number = 0;
      }
    }
  }

  static constexpr float RATIO = 0.25f;

  for (std::size_t i = 1; i < NUMBER_OF_STREETS - 1; ++i) {
    for (std::size_t j = 0; j < NUMBER_OF_STREETS - 1; ++j) {
      if (random.computeBernoulli(RATIO)) {
        std::size_t x = 3 * i + 1;
        std::size_t y1 = 3 * j + 2;
        std::size_t y2 = 3 * j + 3;

        m_map[x][y1].kind = ROADBLOCK;
        m_map[x][y2].kind = ROADBLOCK;
      }
    }
  }

  for (std::size_t i = 1; i < NUMBER_OF_STREETS - 1; ++i) {
    for (std::size_t j = 0; j < NUMBER_OF_STREETS - 1; ++j) {
      if (random.computeBernoulli(RATIO)) {
        std::size_t y = 3 * i + 1;
        std::size_t x1 = 3 * j + 2;
        std::size_t x2 = 3 * j + 3;

        m_map[x1][y].kind = ROADBLOCK;
        m_map[x2][y].kind = ROADBLOCK;
      }
    }
  }


  for (std::size_t i = 0; i < SIZE; ++i) {
    for (std::size_t j = 0; j < SIZE; ++j) {
      const auto& block = m_map[i][j];

      float x1 = i * TILESIZE;
      float x2 = x1 + TILESIZE;
      float y1 = j * TILESIZE;
      float y2 = y1 + TILESIZE;

      switch (block.kind) {
        case GRASS:
          m_array.append(sf::Vertex({ x1, y1 }, sf::Color::Green));
          m_array.append(sf::Vertex({ x1, y2 }, sf::Color::Green));
          m_array.append(sf::Vertex({ x2, y2 }, sf::Color::Green));
          m_array.append(sf::Vertex({ x2, y1 }, sf::Color::Green));
          break;

        case ROADBLOCK:
          m_array.append(sf::Vertex({ x1, y1 }, sf::Color::Yellow));
          m_array.append(sf::Vertex({ x1, y2 }, sf::Color::Yellow));
          m_array.append(sf::Vertex({ x2, y2 }, sf::Color::Yellow));
          m_array.append(sf::Vertex({ x2, y1 }, sf::Color::Yellow));
          break;

        case STREET:
          m_array.append(sf::Vertex({ x1, y1 }, sf::Color::Black));
          m_array.append(sf::Vertex({ x1, y2 }, sf::Color::Black));
          m_array.append(sf::Vertex({ x2, y2 }, sf::Color::Black));
          m_array.append(sf::Vertex({ x2, y1 }, sf::Color::Black));
          break;

        case BUILDING:
          m_array.append(sf::Vertex({ x1, y1 }, sf::Color::Red));
          m_array.append(sf::Vertex({ x1, y2 }, sf::Color::Red));
          m_array.append(sf::Vertex({ x2, y2 }, sf::Color::Red));
          m_array.append(sf::Vertex({ x2, y1 }, sf::Color::Red));
          break;
      }
    }
  }
}

void Map::update(float dt) {

}

void Map::render(sf::RenderWindow& window) {
  window.draw(m_array);
}

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
#include <cassert>
#include <game/base/Log.h>

#ifdef _WIN32
#define M_PI (3.14159265359)
#endif

#include "Constants.h"

Map::Map(game::ResourceManager &resources)
: m_resources(resources)
, m_buildingTexture (resources.getTexture("sheet_building.png"))
, m_roadTexture(resources.getTexture("sheet_road.png"))
, m_arrayBuilding(sf::Quads)
, m_arrayStreet(sf::Quads) {

}

static constexpr float TILESIZE = 256.0f;
static constexpr float BUILDINGSIZE = 512.0f;

void Map::generate(game::Random& random, b2World& world) {
  for (std::size_t i = 0; i < SIZE; ++i) {
    for (std::size_t j = 0; j < SIZE; ++j) {
      auto& block = m_map[i][j];

      if (i == 0 || j == 0 || i == SIZE - 1 || j == SIZE - 1) {
        block.kind = GRASS;
        block.number = random.computeUniformInteger(15, 18);
      } else if (i % 3 == 1 || j % 3 == 1) {
        block.kind = STREET;
        block.number = 0;
      } else {
        block.kind = BUILDING;
        block.number = random.computeUniformInteger(0, 2);
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

        int typeBlock = random.computeUniformInteger(0, 99);

        if (typeBlock >= 0 && typeBlock < 10) {
          typeBlock = random.computeUniformInteger(7, 9);
          m_map[x][y1].number = typeBlock;

          switch (typeBlock) {
            case 7:
              typeBlock = random.computeUniformInteger(8, 9);
              m_map[x][y2].number = typeBlock;
              break;

            case 8:
              typeBlock = random.computeUniformInteger(0, 1);

              if (typeBlock == 0) {
                m_map[x][y2].number = 7;
              }
              else {
                m_map[x][y2].number = 9;
              }

              break;

            case 9:
              typeBlock = random.computeUniformInteger(7, 8);
              m_map[x][y2].number = typeBlock;
              break;

            default:
              assert(false);
          }
        }
        else {
          m_map[x][y1].number = 13;
          m_map[x][y2].number = 14;
        }
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

        int typeBlock = random.computeUniformInteger(0, 99);

        if (typeBlock >= 0 && typeBlock < 10) {
          typeBlock = random.computeUniformInteger(10, 12);
          m_map[x1][y].number = typeBlock;

          switch (typeBlock) {
            case 10:
              typeBlock = random.computeUniformInteger(11, 12);
              m_map[x2][y].number = typeBlock;
              break;

            case 11:
              typeBlock = random.computeUniformInteger(10, 12);

              if (typeBlock == 0) {
                m_map[x2][y].number = 10;
              }
              else {
                m_map[x2][y].number = 12;
              }

              break;

            case 12:
              typeBlock = random.computeUniformInteger(10, 11);
              m_map[x2][y].number = typeBlock;
              break;

            default:
              assert(false);
          }
        }
        else {
          m_map[x1][y].number = 13;
          m_map[x2][y].number = 14;
        }
      }
    }
  }

  // Set right number of road
  for (std::size_t i = 1; i < SIZE - 1; ++i) {
    for (std::size_t j = 1; j < SIZE - 1; j = j + 3) {
      auto& block = m_map[i][j];

      if (block.kind == STREET) {
        if (i % 3 != 1) {
          block.number = 1;
        }
      }
    }
  }

  // Set right number of road
  for (std::size_t i = 1; i < SIZE - 1; i = i + 3) {
    for (std::size_t j = 2; j < SIZE - 1; ++j) {
      auto& block = m_map[i][j];

      if (block.kind == STREET) {
        if (j % 3 != 1) {
          block.number = 6;
        }
      }
    }
  }

  // Set corner
  m_map[1][1].number = 2;
  m_map[1][SIZE-2].number = 5;
  m_map[SIZE-2][SIZE-2].number = 4;
  m_map[SIZE-2][1].number = 3;

  // set special building
  int x, y;
  x = random.computeUniformInteger(0, NUMBER_OF_STREETS-2);
  y = random.computeUniformInteger(0, NUMBER_OF_STREETS-2);

  std::vector<sf::Vector2i> positions;
  positions.push_back({ x, y });

  // set Bank
  x = 2 + 3 * x;
  y = 2 + 3 * y;
  m_map[x][y].number = 3;
  // Check if the street is clearly
  if (m_map[x - 1][y].kind == STREET) {
    m_bankGoal.x = (x - 1 + 0.5f) * TILESIZE;
    m_bankGoal.y = (y + 0.5f) * TILESIZE;
  }
  else if (m_map[x][y - 1].kind == STREET) {
    m_bankGoal.x = (x + 0.5f) * TILESIZE;
    m_bankGoal.y = (y - 1 + 0.5f) * TILESIZE;
  }
  else if (m_map[x][y + 2].kind == STREET) {
    m_bankGoal.x = (x + 0.5f) * TILESIZE;
    m_bankGoal.y = (y + 2 + 0.5f) * TILESIZE;
  }
  else if (m_map[x + 2][y + 1].kind == STREET) {
    m_bankGoal.x = (x + 2 + 0.5f) * TILESIZE;
    m_bankGoal.y = (y + 1 + 0.5f) * TILESIZE;
  }
  else {
    assert(false);
  }

  // set gun store
  do {
    x = random.computeUniformInteger(0, NUMBER_OF_STREETS-3);
    y = random.computeUniformInteger(0, NUMBER_OF_STREETS-3);
  } while (checkPosition(positions, x, y));
  positions.push_back({ x, y });

  x = 2 + 3 * x;
  y = 2 + 3 * y;
  m_map[x][y].number = 4;
  // Check if the street is clearly
  if (m_map[x - 1][y].kind == STREET) {
    m_gunStoreGoal.x = (x - 1 + 0.5f) * TILESIZE;
    m_gunStoreGoal.y = (y + 0.5f) * TILESIZE;
  }
  else if (m_map[x][y - 1].kind == STREET) {
    m_gunStoreGoal.x = (x + 0.5f) * TILESIZE;
    m_gunStoreGoal.y = (y - 1 + 0.5f) * TILESIZE;
  }
  else if (m_map[x][y + 2].kind == STREET) {
    m_gunStoreGoal.x = (x + 0.5f) * TILESIZE;
    m_gunStoreGoal.y = (y + 2 + 0.5f) * TILESIZE;
  }
  else if (m_map[x + 2][y + 1].kind == STREET) {
    m_gunStoreGoal.x = (x + 2 + 0.5f) * TILESIZE;
    m_gunStoreGoal.y = (y + 1 + 0.5f) * TILESIZE;
  }
  else {
    assert(false);
  }

  // set home
  do {
    x = random.computeUniformInteger(0, NUMBER_OF_STREETS-3);
    y = random.computeUniformInteger(0, NUMBER_OF_STREETS-3);
  } while (checkPosition(positions, x, y));
  positions.push_back({ x, y });

  x = 2 + 3 * x;
  y = 2 + 3 * y;
  m_map[x][y].number = 6;
  // Check if the street is clearly
  if (m_map[x - 1][y].kind == STREET) {
    m_homeGoal.x = (x - 1 + 0.5f) * TILESIZE;
    m_homeGoal.y = (y + 0.5f) * TILESIZE;
    m_homeAngle = M_PI / 2;
  }
  else if (m_map[x + 1][y - 1].kind == STREET) {
    m_homeGoal.x = (x + 1 + 0.5f) * TILESIZE;
    m_homeGoal.y = (y - 1 + 0.5f) * TILESIZE;
    m_homeAngle = M_PI;
  }
  else if (m_map[x][y + 2].kind == STREET) {
    m_homeGoal.x = (x + 0.5f) * TILESIZE;
    m_homeGoal.y = (y + 2 + 0.5f) * TILESIZE;
    m_homeAngle = 0;
  }
  else if (m_map[x + 2][y + 1].kind == STREET) {
    m_homeGoal.x = (x + 2 + 0.5f) * TILESIZE;
    m_homeGoal.y = (y + 1 + 0.5f) * TILESIZE;
    m_homeAngle = 3 * M_PI / 2;
  }
  else {
    assert(false);
  }

  // set clothing store
  do {
    x = random.computeUniformInteger(0, NUMBER_OF_STREETS-3);
    y = random.computeUniformInteger(0, NUMBER_OF_STREETS-3);
  } while (checkPosition(positions, x, y));
  positions.push_back({ x, y });

  x = 2 + 3 * x;
  y = 2 + 3 * y;
  m_map[x][y].number = 0;
  // Check if the street is clearly
  if (m_map[x - 1][y].kind == STREET) {
    m_clothingStore.x = (x - 1 + 0.5f) * TILESIZE;
    m_clothingStore.y = (y + 0.5f) * TILESIZE;
  }
  else if (m_map[x][y - 1].kind == STREET) {
    m_clothingStore.x = (x + 0.5f) * TILESIZE;
    m_clothingStore.y = (y - 1 + 0.5f) * TILESIZE;
  }
  else if (m_map[x][y + 2].kind == STREET) {
    m_clothingStore.x = (x + 0.5f) * TILESIZE;
    m_clothingStore.y = (y + 2 + 0.5f) * TILESIZE;
  }
  else if (m_map[x + 2][y + 1].kind == STREET) {
    m_clothingStore.x = (x + 2 + 0.5f) * TILESIZE;
    m_clothingStore.y = (y + 1 + 0.5f) * TILESIZE;
  }
  else {
    assert(false);
  }

  // set rocket store
  do {
    x = random.computeUniformInteger(0, NUMBER_OF_STREETS-3);
    y = random.computeUniformInteger(0, NUMBER_OF_STREETS-3);
  } while (checkPosition(positions, x, y));
  positions.push_back({ x, y });

  x = 2 + 3 * x;
  y = 2 + 3 * y;
  m_map[x][y].number = 4;
  // Check if the street is clearly
  if (m_map[x - 1][y].kind == STREET) {
    m_rocketStoreGoal.x = (x - 1 + 0.5f) * TILESIZE;
    m_rocketStoreGoal.y = (y + 0.5f) * TILESIZE;
  }
  else if (m_map[x][y - 1].kind == STREET) {
    m_rocketStoreGoal.x = (x + 0.5f) * TILESIZE;
    m_rocketStoreGoal.y = (y - 1 + 0.5f) * TILESIZE;
  }
  else if (m_map[x][y + 2].kind == STREET) {
    m_rocketStoreGoal.x = (x + 0.5f) * TILESIZE;
    m_rocketStoreGoal.y = (y + 2 + 0.5f) * TILESIZE;
  }
  else if (m_map[x + 2][y + 1].kind == STREET) {
    m_rocketStoreGoal.x = (x + 2 + 0.5f) * TILESIZE;
    m_rocketStoreGoal.y = (y + 1 + 0.5f) * TILESIZE;
  }
  else {
    assert(false);
  }

  // set accomplice
  do {
    x = random.computeUniformInteger(0, NUMBER_OF_STREETS-3);
    y = random.computeUniformInteger(0, NUMBER_OF_STREETS-3);
  } while (checkPosition(positions, x, y));
  positions.push_back({ x, y });

  x = 2 + 3 * x;
  y = 2 + 3 * y;
  m_map[x][y].number = 5;
  // Check if the street is clearly
  if (m_map[x - 1][y].kind == STREET) {
    m_accompliceGoal.x = (x - 1 + 0.5f) * TILESIZE;
    m_accompliceGoal.y = (y + 0.5f) * TILESIZE;
  }
  else if (m_map[x][y - 1].kind == STREET) {
    m_accompliceGoal.x = (x + 0.5f) * TILESIZE;
    m_accompliceGoal.y = (y - 1 + 0.5f) * TILESIZE;
  }
  else if (m_map[x][y + 2].kind == STREET) {
    m_accompliceGoal.x = (x + 0.5f) * TILESIZE;
    m_accompliceGoal.y = (y + 2 + 0.5f) * TILESIZE;
  }
  else if (m_map[x + 2][y + 1].kind == STREET) {
    m_accompliceGoal.x = (x + 2 + 0.5f) * TILESIZE;
    m_accompliceGoal.y = (y + 1 + 0.5f) * TILESIZE;
  }
  else {
    assert(false);
  }


  for (std::size_t i = 0; i < SIZE; ++i) {
    for (std::size_t j = 0; j < SIZE; ++j) {
      const auto& block = m_map[i][j];

      float x1 = i * TILESIZE;
      float x2 = x1 + TILESIZE;
      float y1 = j * TILESIZE;
      float y2 = y1 + TILESIZE;

      float u = (block.number % 6) * TILESIZE;
      float v = (block.number / 6) * TILESIZE;

      switch (block.kind) {
        case STREET:
        case ROADBLOCK:
        case GRASS:
          m_arrayStreet.append(sf::Vertex({ x1, y1 }, { u, v }));
          m_arrayStreet.append(sf::Vertex({ x1, y2 }, { u, v + TILESIZE }));
          m_arrayStreet.append(sf::Vertex({ x2, y2 }, { u + TILESIZE, v + TILESIZE }));
          m_arrayStreet.append(sf::Vertex({ x2, y1 }, { u + TILESIZE, v }));
          break;

        case BUILDING:
          break;
      }
    }

    for (std::size_t i = 2; i < SIZE - 2; i += 3) {
      for (std::size_t j = 2; j < SIZE - 2; j += 3) {
        const auto& block = m_map[i][j];

        float x1 = i * TILESIZE;
        float x2 = x1 + TILESIZE * 2;
        float y1 = j * TILESIZE;
        float y2 = y1 + TILESIZE * 2;

        float u = (block.number % 3) * BUILDINGSIZE;
        float v = (block.number / 3) * BUILDINGSIZE;

        switch (block.kind) {
          case BUILDING:
            // on récupère un pointeur vers le quad à définir dans le tableau de vertex
            m_arrayBuilding.append(sf::Vertex({ x1, y1 }, { u, v }));
            m_arrayBuilding.append(sf::Vertex({ x1, y2 }, { u, v + BUILDINGSIZE }));
            m_arrayBuilding.append(sf::Vertex({ x2, y2 }, { u + BUILDINGSIZE, v + BUILDINGSIZE }));
            m_arrayBuilding.append(sf::Vertex({ x2, y1 }, { u + BUILDINGSIZE, v }));
            break;

          default:
            assert(false);
        }
      }
    }
  }

  for (std::size_t i = 0; i < SIZE; ++i) {
    for (std::size_t j = 0; j < SIZE; ++j) {
      auto& block = m_map[i][j];

      if (block.kind != STREET) {
        b2BodyDef def;
        def.type = b2_staticBody;
        def.position.x = (i * TILESIZE + TILESIZE * 0.5f) * BOX2D_SCALE;
        def.position.y = (j * TILESIZE + TILESIZE * 0.5f) * BOX2D_SCALE;
        auto body = world.CreateBody(&def);

        b2PolygonShape shape;
        shape.SetAsBox(TILESIZE * BOX2D_SCALE * 0.5f, TILESIZE * BOX2D_SCALE * 0.5f);

        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.density = 1.0f;
        fixture.friction = 0.3f;

        body->CreateFixture(&fixture);
      }
    }
  }

}

void Map::update(float dt) {

}

void Map::render(sf::RenderWindow& window) {
  window.draw(m_arrayStreet, m_roadTexture);
  window.draw(m_arrayBuilding, m_buildingTexture);
}

sf::Vector2f Map::getBankGoal() {
  return m_bankGoal;
}

sf::Vector2f Map::getGunStoreGoal() {
  return m_gunStoreGoal;
}

sf::Vector2f Map::getHomeGoal() {
  return m_homeGoal;
}

sf::Vector2f Map::getClothingStoreGoal() {
  return m_clothingStore;
}

sf::Vector2f Map::getAccompliceGoal() {
  return m_accompliceGoal;
}

sf::Vector2f Map::getRocketStoreGoal() {
  return m_rocketStoreGoal;
}

bool Map::checkPosition(std::vector<sf::Vector2i> vector, int x, int y) const {
  bool found = false;
  for (std::size_t i = 0; i < vector.size() && !found; ++i) {
    found = vector[i].x == x && vector[i].y == y;
  }

  return found;
}

std::vector<std::tuple<sf::Vector2f, float>> Map::getCarsPositions(unsigned n, game::Random& random) const {
  std::vector<std::tuple<sf::Vector2f, float>> cars;

  std::vector<sf::Vector2i> check;

  for (unsigned k = 0; k < n; ++k) {

    int i, j;
    do {
      i = random.computeUniformInteger(1, SIZE - 1);
      j = random.computeUniformInteger(1, SIZE - 1);
    } while (m_map[i][j].kind != STREET || m_map[i][j].number == 0 || checkPosition(check, i, j));

    check.push_back({ i, j });

    float x, y, angle;

    if (i % 3 == 1) {
      if (random.computeBernoulli(0.5)) {
        x = (i + 0.875) * TILESIZE;
        y = (j + 0.5) * TILESIZE;
        angle = 3 * M_PI / 2;
      } else {
        x = (i + 0.125) * TILESIZE;
        y = (j + 0.5) * TILESIZE;
        angle = M_PI / 2;
      }
    } else {
      assert(j % 3 == 1);

      if (random.computeBernoulli(0.5)) {
        x = (i + 0.5) * TILESIZE;
        y = (j + 0.875) * TILESIZE;
        angle = 0;
      } else {
        x = (i + 0.5) * TILESIZE;
        y = (j + 0.125) * TILESIZE;
        angle = M_PI;
      }
    }

    cars.push_back(std::make_tuple(sf::Vector2f(x, y), angle));
  }

  return cars;
}

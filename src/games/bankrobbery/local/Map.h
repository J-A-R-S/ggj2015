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

#include <Box2D/Box2D.h>

#include <game/base/Random.h>
#include <game/graphics/Entity.h>
#include <game/graphics/Resource.h>

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
  };

  Map(game::ResourceManager &resources);

  void generate(game::Random& random, b2World& world);

  virtual void update(float dt) override;

  virtual void render(sf::RenderWindow& window) override;

  sf::Vector2f getBankGoal();
  sf::Vector2f getGunStoreGoal();
  sf::Vector2f getHomeGoal();
  float getHomeAngle() { return m_homeAngle; }
  sf::Vector2f getClothingStoreGoal();
  sf::Vector2f getAccompliceGoal();
  sf::Vector2f getRocketStoreGoal();

  std::vector<std::tuple<sf::Vector2f, float>> getCarsPositions(unsigned n, game::Random& random) const;

private:
  game::ResourceManager& m_resources;
  sf::Texture *m_buildingTexture;
  sf::Texture *m_roadTexture;

  std::array<std::array<Block, SIZE>, SIZE> m_map;
  sf::VertexArray m_arrayBuilding;
  sf::VertexArray m_arrayStreet;

  sf::Vector2f m_bankGoal;
  sf::Vector2f m_gunStoreGoal;
  sf::Vector2f m_homeGoal;
  float m_homeAngle;
  sf::Vector2f m_clothingStore;
  sf::Vector2f m_rocketStoreGoal;
  sf::Vector2f m_accompliceGoal;

  bool checkPosition(std::vector<sf::Vector2i> vector, int x, int y) const;
};

#endif // LOCAL_MAP_H

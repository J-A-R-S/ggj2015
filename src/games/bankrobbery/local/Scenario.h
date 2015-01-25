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
#ifndef LOCAL_SCENARIO_H
#define LOCAL_SCENARIO_H

#include <game/base/Event.h>
#include <game/graphics/Entity.h>
#include <game/graphics/Resource.h>

struct Step;

class Scenario : public game::Entity {
public:
  enum Character {
    Flo,
    Roxy,
  };

  Scenario(game::EventManager& events, game::ResourceManager& resources);

  void addStep(Character character, float message_timer, const std::string& message);
  void addStep(Character character, float message_timer, const std::string& message, float target_timer, const sf::Vector2f& target);
  void start();

  virtual int priority() const override;
  virtual void update(float dt) override;
  virtual void render(sf::RenderWindow& window) override;

private:
  game::EventStatus onHeroPosition(game::EventType type, game::Event* event);
  void initStep();

private:
  std::vector<Step> m_steps;
  unsigned m_current_step;

  float m_message_timer;
  std::string m_message;

  bool m_active;
  float m_target_timer;
  sf::Vector2f m_target;
  sf::Vector2f m_hero;
  Character m_character;

  sf::Font *m_font;
  sf::Texture *m_floTexture;
  sf::Texture *m_roxyTexture;
};

struct Step {
  float message_timer;
  std::string message;
  bool active;
  float target_timer;
  sf::Vector2f target;
  Scenario::Character character;
};



#endif // LOCAL_SCENARIO_H

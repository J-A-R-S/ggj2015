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

struct Step {
  std::string message;
  float timer;
  sf::Vector2f target;
};

class Scenario : public game::Entity {
public:
  Scenario(game::EventManager& events, game::ResourceManager& resources);

  void addStep(const Step& step);
  void start();

  virtual void  update  (float dt) override;
  virtual void  render  (sf::RenderWindow& window) override;

private:
  game::EventStatus onHeroPosition(game::EventType type, game::Event* event);
  void initStep();

private:
  std::vector<Step> m_steps;
  unsigned m_current_step;

  float m_message_timer;
  std::string m_message;

  float m_timer;
  sf::Vector2f m_target;
  sf::Vector2f m_hero;

  sf::Font *m_font;
};



#endif // LOCAL_SCENARIO_H
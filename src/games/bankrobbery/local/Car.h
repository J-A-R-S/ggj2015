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
#ifndef LOCAL_CAR_H
#define LOCAL_CAR_H

#include <SFML/Graphics.hpp>

#include <game/base/Event.h>
#include <game/graphics/Entity.h>
#include <game/graphics/Resource.h>

#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>

class Car : public game::Entity {
public:
  constexpr static float SPEED_MAX = 200;
  constexpr static float SPEED_MIN = -60;
  constexpr static float SPEED_FACTOR = 1.25;
  constexpr static float TURN_FACTOR = 0.02;
  constexpr static float FRICTION_FACTOR = 20;

  constexpr static float WIDTH = 128;
  constexpr static float HEIGHT = 64;

  explicit Car(int car, game::ResourceManager& resources, b2World& world, sf::Vector2f position, float angle);


  virtual int priority() const override;
  virtual void render(sf::RenderWindow& window) override;

protected:
  float getVelocity() const {
    return m_velocity;
  }

  float getAngle() const {
    return m_body->GetAngle();
  }

  void setVelocityAndAngle(float velocity, float angle);
  void fadeVelocity(float dt);

  float getAbsoluteVelocity() const;

  sf::Vector2f getPosition() const;

private:
  const int m_car;
  sf::Texture *m_texture;
  b2Body *m_body;

  float m_velocity;
};

class HeroCar : public Car {
public:
  HeroCar(game::EventManager& events, game::ResourceManager& resources, b2World& world, sf::Vector2f position, float angle);

  virtual void update(float dt) override;

  enum Movement {
    ACCELERATE,
    BRAKE,
    CRUISE,
  };

  void accelerate() {
    m_movement = ACCELERATE;
  }

  void brake() {
    m_movement = BRAKE;
  }

  void cruise() {
    m_movement = CRUISE;
  }

  enum Turn {
    LEFT,
    RIGHT,
    NONE,
  };

  void turnLeft() {
    m_turn = LEFT;
  }

  void turnRight() {
    m_turn = RIGHT;
  }

  void turnNone() {
    m_turn = NONE;
  }

private:
  game::EventManager& m_events;

  Movement m_movement;
  Turn m_turn;

  sf::Sound m_sound;
};

class OtherCar : public Car {
public:
  OtherCar(int car, game::ResourceManager& resources, b2World& world, const sf::Vector2f& position, float angle);

  virtual void update(float dt) override;

};

#endif // LOCAL_CAR_H

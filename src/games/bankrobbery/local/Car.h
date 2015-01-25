/*
 * Copyright (c) 2014, Julien Bernard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
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

  explicit Car(int car, game::ResourceManager& resources, b2World& world, sf::Vector2f position);


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
  HeroCar(game::EventManager& events, game::ResourceManager& resources, b2World& world, sf::Vector2f position);

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

#endif // LOCAL_CAR_H

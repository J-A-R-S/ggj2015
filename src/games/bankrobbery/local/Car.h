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

#include <game/graphics/Entity.h>

class Car : public game::Entity {
public:
  constexpr static float SPEED_MAX = 130; // km/h
  constexpr static float SPEED_MIN = -60; // km/h
  constexpr static float SPEED_FACTOR = 1.25;
  constexpr static float TURN_FACTOR = 0.025;
  constexpr static float FRICTION_FACTOR = 20;

  explicit Car();

  virtual void update(float dt) override;
  virtual void render(sf::RenderWindow& window) override;

  void accelerate();
  void brake();
  void turnLeft();
  void turnRight();

private:
  sf::Vector2f m_speedVector;
  sf::Vector2f m_position;
  float m_speed;
  float m_angle;

  void updateSpeedVector();
};

#endif // LOCAL_CAR_H

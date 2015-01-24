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

#include <cmath>

#include "Car.h"
#include <game/base/Log.h>

/* explicit */ Car::Car()
  : m_speedVector(0.0, 0.0)
  , m_position(500.0, 500.0)
  , m_speed(0)
  , m_angle(0)
{
  //ctor
}

/* virtual */ void Car::update(float dt) {
  m_position.y = m_position.y + m_speedVector.y * dt;
  m_position.x = m_position.x + m_speedVector.x * dt;

  // Friction
  if (m_speed < 0) {
    if (m_speed + FRICTION_FACTOR * dt > 0) {
      m_speed = 0;
    }
    else {
      m_speed = m_speed + FRICTION_FACTOR * dt;
    }
  }
  else if (m_speed > 0) {
    if (m_speed - FRICTION_FACTOR * dt < 0) {
      m_speed = 0;
    }
    else {
      m_speed = m_speed - FRICTION_FACTOR * dt;
    }
  }

  updateSpeedVector();
}

/* virtual */ void Car::render(sf::RenderWindow& window) {
  // Draw the car
  sf::RectangleShape rectangle(sf::Vector2f(128, 64));
  rectangle.setOrigin(rectangle.getLocalBounds().width / 2, rectangle.getLocalBounds().height);
  rectangle.setFillColor(sf::Color::White);
  rectangle.setPosition(m_position);
  rectangle.rotate(m_angle * (180 / M_PI));
  window.draw(rectangle);
}

void Car::accelerate() {

  if (m_speed < Car::SPEED_MAX) {
    m_speed += Car::SPEED_FACTOR;
  }
}

void Car::brake() {

  if (m_speed > Car::SPEED_MIN) {
    m_speed -= Car::SPEED_FACTOR*1.20;
  }
}

void Car::turnLeft() {
  if (m_speed == 0) {
    return;
  }

  m_angle = m_angle - TURN_FACTOR;

  if (m_angle < 0) {
    m_angle = M_PI * 2;
  }
}

void Car::turnRight() {
  if (m_speed == 0) {
    return;
  }

  m_angle = m_angle + TURN_FACTOR;

  if (m_angle > M_PI * 2) {
    m_angle = 0;
  }
}

void Car::updateSpeedVector() {
  m_speedVector.x = std::cos(m_angle) * m_speed;
  m_speedVector.y = std::sin(m_angle) * m_speed;
}

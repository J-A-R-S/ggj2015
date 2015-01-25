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
#include "Car.h"

#include <cmath>
#include <iostream>

#include <game/base/Log.h>

#include "Constants.h"
#include "Events.h"

/* explicit */ Car::Car(game::EventManager& events, game::ResourceManager& resources, b2World& world)
  : m_events(events)
  , m_texture(nullptr)
  , m_body(nullptr)
  , m_movement(CRUISE)
  , m_turn(NONE)
  , m_velocity(0)
  , m_angle(0)
{
  m_texture = resources.getTexture("sheet_car.png");
  m_texture->setSmooth(true);

  b2BodyDef def;
  def.type = b2_dynamicBody;
  def.linearDamping = 0.2f;
  def.position.Set(384.0f * BOX2D_SCALE, 384.0f * BOX2D_SCALE);
  m_body = world.CreateBody(&def);

  float half_w = WIDTH * BOX2D_SCALE * 0.5f;
  float half_h = HEIGHT * BOX2D_SCALE * 0.5f;

  static constexpr float X = 0.8f;

  b2Vec2 vertices[8];
  vertices[0].Set(- half_w, X * half_h);
  vertices[1].Set(- X * half_w, half_h);
  vertices[2].Set(  X * half_w, half_h);
  vertices[3].Set(  half_w, X * half_h);
  vertices[4].Set(  half_w, - X * half_h);
  vertices[5].Set(  X * half_w, - half_h);
  vertices[6].Set(- X * half_w, - half_h);
  vertices[7].Set(- half_w, - X * half_h);

  b2PolygonShape shape;
  shape.Set(vertices, 8);
//   shape.SetAsBox(WIDTH * BOX2D_SCALE * 0.5f, HEIGHT * BOX2D_SCALE * 0.5f);

  b2FixtureDef fixture;
  fixture.shape = &shape;
  fixture.density = 1.0f;
  fixture.friction = 0.0f;

  m_body->CreateFixture(&fixture);

  sf::SoundBuffer *buffer = resources.getSoundBuffer("car_engine.ogg");
  m_sound.setBuffer(*buffer);
  m_sound.setVolume(0);
  m_sound.setLoop(true);
  m_sound.play();
}


int Car::priority() const {
  return 20;
}

static constexpr float HOP = 8.0f;
static constexpr float TURN = 2.0f;

/* virtual */ void Car::update(float dt) {
  auto pos = m_body->GetPosition();

  HeroPositionEvent event;
  event.pos.x = pos.x / BOX2D_SCALE;
  event.pos.y = pos.y / BOX2D_SCALE;
  m_events.triggerEvent(&event);

  float angle = m_body->GetAngle();
  float abs_velocity = m_body->GetLinearVelocity().Length();
  float velocity = m_velocity > 0 ? abs_velocity : - abs_velocity;

  switch (m_movement) {
    case ACCELERATE:
      velocity += HOP * dt;
      break;

    case BRAKE:
      velocity -= HOP * dt;
      break;

    case CRUISE:
      break;
  }

  velocity = b2Clamp(velocity, -5.0f, 15.0f);

  float factor = 0.6f * abs_velocity * std::exp(- 0.2 * abs_velocity);

  switch (m_turn) {
    case LEFT:
      angle -= dt * TURN * factor;
      break;

    case RIGHT:
      angle += dt * TURN * factor;
      break;

    case NONE:
      break;
  }

  m_body->SetTransform(m_body->GetPosition(), angle);
  m_body->SetAngularVelocity(0.0f);

  b2Rot rot(angle);
  b2Vec2 vel(velocity, 0.0f);
  vel = b2Mul(rot, vel);

  m_body->SetLinearVelocity(vel);

  m_velocity = velocity;

  // Set volume audio
  float volume = m_body->GetLinearVelocity().Length() / 15.0f * 100;
  m_sound.setVolume(volume);
}

/* virtual */ void Car::render(sf::RenderWindow& window) {
  auto pos = m_body->GetPosition();

  sf::IntRect textureRect;
  textureRect.left = 1 * WIDTH;
  textureRect.top = 1 * HEIGHT;
  textureRect.width = WIDTH;
  textureRect.height = HEIGHT;

  sf::Sprite sprite(*m_texture);
  sprite.setTextureRect(textureRect);
  sprite.setOrigin(WIDTH / 2.0f, HEIGHT / 2.0f);
  sprite.setPosition(pos.x / BOX2D_SCALE, pos.y / BOX2D_SCALE);
  sprite.rotate(m_body->GetAngle() * (180 / M_PI));

  window.draw(sprite);
}

void Car::accelerate() {
  m_movement = ACCELERATE;
}

void Car::brake() {
  m_movement = BRAKE;
}

void Car::cruise() {
  m_movement = CRUISE;
}

void Car::turnLeft() {
  m_turn = LEFT;
}

void Car::turnRight() {
  m_turn = RIGHT;
}

void Car::turnNone() {
  m_turn = NONE;
}

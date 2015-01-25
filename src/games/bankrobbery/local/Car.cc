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
#include "Car.h"

#include <cmath>
#include <iostream>

#include <game/base/Log.h>

#include "Constants.h"
#include "Events.h"

Car::Car(int car, game::ResourceManager& resources, b2World& world, sf::Vector2f position, float angle)
  : m_car(car)
  , m_texture(nullptr)
  , m_body(nullptr)
  , m_velocity(0)
{
  assert(0 <= car && car < 10);

  m_texture = resources.getTexture("sheet_car.png");
  m_texture->setSmooth(true);

  b2BodyDef def;
  def.type = b2_dynamicBody;
  def.linearDamping = 0.2f;
  def.position.Set(position.x * BOX2D_SCALE, position.y * BOX2D_SCALE);
  def.angle = angle;
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
  fixture.density = 100.0f;
  fixture.friction = 1.0f;

  m_body->CreateFixture(&fixture);
}


int Car::priority() const {
  return 20;
}

void Car::render(sf::RenderWindow& window) {
  auto pos = m_body->GetPosition();

  sf::IntRect textureRect;
  textureRect.left = (m_car % 4) * WIDTH;
  textureRect.top = (m_car / 4) * HEIGHT;
  textureRect.width = WIDTH;
  textureRect.height = HEIGHT;

  sf::Sprite sprite(*m_texture);
  sprite.setTextureRect(textureRect);
  sprite.setOrigin(WIDTH / 2.0f, HEIGHT / 2.0f);
  sprite.setPosition(pos.x / BOX2D_SCALE, pos.y / BOX2D_SCALE);
  sprite.rotate(m_body->GetAngle() * (180 / M_PI));

  window.draw(sprite);
}

void Car::setVelocityAndAngle(float velocity, float angle) {
  m_body->SetTransform(m_body->GetPosition(), angle);
  m_body->SetAngularVelocity(0.0f);

  b2Rot rot(angle);
  b2Vec2 vel(velocity, 0.0f);
  vel = b2Mul(rot, vel);

  m_body->SetLinearVelocity(vel);

  m_velocity = velocity;
}

void Car::fadeVelocity(float dt) {
  b2Vec2 velocity = m_body->GetLinearVelocity();
  velocity -= (3.0f * dt * velocity);
  m_body->SetAngularVelocity(0.0f);
  m_body->SetLinearVelocity(velocity);
}

float Car::getAbsoluteVelocity() const {
  return m_body->GetLinearVelocity().Length();
}

sf::Vector2f Car::getPosition() const {
  auto pos = m_body->GetPosition();
  return { pos.x / BOX2D_SCALE, pos.y / BOX2D_SCALE };
}

/*
 * HeroCar
 */

HeroCar::HeroCar(game::EventManager& events, game::ResourceManager& resources, b2World& world, sf::Vector2f position, float angle)
  : Car(5, resources, world, position, angle)
  , m_events(events)
  , m_movement(CRUISE)
  , m_turn(NONE)
{
  sf::SoundBuffer *buffer = resources.getSoundBuffer("car_engine.ogg");
  m_sound.setBuffer(*buffer);
  m_sound.setVolume(0);
  m_sound.setLoop(true);
  m_sound.play();
}

static constexpr float HOP = 8.0f;
static constexpr float TURN = 2.0f;

void HeroCar::update(float dt) {
  auto pos = getPosition();

  HeroPositionEvent event;
  event.pos = pos;
  m_events.triggerEvent(&event);

  float angle = getAngle();
  float abs_velocity = getAbsoluteVelocity();
  float velocity = getVelocity() > 0 ? abs_velocity : - abs_velocity;

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
      if (velocity >= 0)
        angle -= dt * TURN * factor;
      else
        angle += dt * TURN * factor;
      break;

    case RIGHT:
      if (velocity >= 0)
        angle += dt * TURN * factor;
      else
        angle -= dt * TURN * factor;
      break;

    case NONE:
      break;
  }

  setVelocityAndAngle(velocity, angle);

  // Set volume audio
  float volume = getAbsoluteVelocity() / 15.0f * 100;
  m_sound.setVolume(volume);
}


/*
 * OtherCar
 */

OtherCar::OtherCar(int car, game::ResourceManager& resources, b2World& world, const sf::Vector2f& position, float angle)
  : Car(car, resources, world, position, angle)
{
}

void OtherCar::update(float dt) {
  fadeVelocity(dt);
//   float angle = getAngle();
//   float abs_velocity = getAbsoluteVelocity();
//
//   abs_velocity -= (abs_velocity * 0.5 * dt);
//
//   setVelocityAndAngle(abs_velocity, angle);
}

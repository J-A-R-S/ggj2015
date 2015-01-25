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

#include "Scenario.h"

#include <cassert>
#include <cstdio>
#include <array>

#include "Events.h"
#include "Map.h"

Scenario::Scenario(game::EventManager& events, game::ResourceManager& resources) {
  events.registerHandler<HeroPositionEvent>(&Scenario::onHeroPosition, this);
  m_font = resources.getFont("Averia-Regular.ttf");
  assert(m_font);
}

static constexpr float DISTANCE = 100.0f;

game::EventStatus Scenario::onHeroPosition(game::EventType type, game::Event* event) {
  assert(event);
  auto e = static_cast<HeroPositionEvent*>(event);
  m_hero = e->pos;

  sf::Vector2f diff = m_target - m_hero;
  float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

  if (distance < DISTANCE) {
    m_current_step++;

    if (m_current_step < m_steps.size()) {
      initStep();
    }
  }

  return game::EventStatus::KEEP;
}

void Scenario::initStep() {
  m_message_timer = m_steps[m_current_step].message_timer;
  m_message = m_steps[m_current_step].message;

  m_active = m_steps[m_current_step].active;
  m_target = m_steps[m_current_step].target;
  m_target_timer = m_steps[m_current_step].target_timer;
}

void Scenario::addStep(float message_timer, const std::string& message) {
  Step step;
  step.message_timer = message_timer;
  step.message = message;
  step.active = false;
  m_steps.emplace_back(std::move(step));
}

void Scenario::addStep(float message_timer, const std::string& message, float target_timer, const sf::Vector2f& target) {
  Step step;
  step.message_timer = message_timer;
  step.message = message;
  step.active = true;
  step.target_timer = target_timer;
  step.target = target;
  m_steps.emplace_back(std::move(step));
}

void Scenario::start() {
  assert(!m_steps.empty());

  m_current_step = 0;
  initStep();
}

void Scenario::update(float dt) {
  m_target_timer -= dt;
  m_message_timer -= dt;

  if (m_message_timer < 0 && !m_active) {
    m_current_step++;
    initStep();
  }
}

void Scenario::render(sf::RenderWindow& window) {
  if (m_active) {
    sf::CircleShape shape(100);
    shape.setFillColor({0xff, 0x00, 0x00, 0x80});
    shape.setOrigin(100.0, 100.0);
    shape.setPosition(m_target);

    window.draw(shape);
    
    // direction
    sf::Vector2f diff = m_target - m_hero;
    float angle = std::atan2(diff.y, diff.x);

    static constexpr float TRIANGLE_SIZE = 30;
    sf::CircleShape triangle(TRIANGLE_SIZE, 3);
    float radius = 120.0f;
    triangle.setPosition(m_hero.x + radius * std::cos(angle), m_hero.y + radius * std::sin(angle));
    triangle.setFillColor(sf::Color(0xFF, 0x00, 0x00, 0x80));
    triangle.setOrigin(TRIANGLE_SIZE, TRIANGLE_SIZE);
    triangle.setRotation(angle / M_PI * 180.0 + 90.0);

    window.draw(triangle);
  }

  sf::View saved_view = window.getView();

  auto size = window.getSize();
  sf::View view;
  view.setCenter(size.x / 2.0f, size.y / 2.0f);
  view.setSize(size.x, size.y);
  window.setView(view);

  if (m_active) {
    // timer
    if (m_target_timer > 0) {
      unsigned timer = static_cast<unsigned>(m_target_timer);
      std::array<char, 64> buffer;
      std::snprintf(buffer.data(), buffer.size(), "%.2u", timer);

      sf::Text text;
      text.setFont(*m_font);
      text.setCharacterSize(48);
      text.setColor(sf::Color::Red);
      text.setString(buffer.data());
      text.setPosition(20.0f, 20.0f);
      window.draw(text);
    }

  }

  // message

  if (m_message_timer > 0) {
    sf::Text text;
    text.setFont(*m_font);
    text.setCharacterSize(32);
    text.setColor(sf::Color::Black);
    text.setString(m_message);

    auto bounds = text.getLocalBounds();

    text.setOrigin(bounds.width / 2.0f, 0.0f);
    text.setPosition(size.x / 2.0f, size.y * 0.8f);

    static constexpr float PADDING = 20.0f;

    sf::RectangleShape rectangle({ bounds.width + 2 * PADDING, bounds.height + 2 * PADDING });
    rectangle.setOrigin(bounds.width / 2.0f + PADDING, PADDING);
    rectangle.setPosition(text.getPosition());
    rectangle.setFillColor({ 0xA7, 0x67, 0x26, 0x95 });
    rectangle.setOutlineThickness(1.0f);
    rectangle.setOutlineColor(sf::Color::Black);
    window.draw(rectangle);

    window.draw(text);
  }

  window.setView(saved_view);
}

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
#ifndef LOCAL_EVENTS_H
#define LOCAL_EVENTS_H

#include <game/base/Event.h>
#include <SFML/System.hpp>

struct HeroPositionEvent : game::Event {
  static const game::EventType type = "HeroPosition"_type;

  sf::Vector2f pos;
};


#endif // LOCAL_EVENTS_H

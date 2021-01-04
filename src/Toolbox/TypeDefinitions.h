#pragma once

#ifndef TYPEDEFINTIONS_H
#define TYPEDEFINITONS_H

#include <bitset>
#include <cstdint>

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>; //determines what components are turned on for an entity.

#endif

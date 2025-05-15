#pragma once

// This is the main include file for the kotECS library
// Include this file to access all ECS functionality

// Core ECS components
#include "ecs/BaseComponentStorage.h"
#include "ecs/ComponentStorage.h"
#include "ecs/EntityId.h"
#include "ecs/World.h"

// Filter system for querying entities
#include "ecs/Filter.h"
#include "ecs/FilterBuilder.h"
#include "ecs/KotIterator.h"

// Systems management
#include "ecs/IInitializer.h"
#include "ecs/ISystem.h"
#include "ecs/SystemManager.h"

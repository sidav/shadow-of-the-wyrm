#pragma once

enum ClassIdentifier
{
  CLASS_ID_NULL = 0,
  CLASS_ID_GAME,
  CLASS_ID_CURSES_DISPLAY,
  CLASS_ID_COMMAND_FACTORY,
  CLASS_ID_EQUIPMENT_COMMAND_FACTORY,
  CLASS_ID_INVENTORY_COMMAND_FACTORY,
  CLASS_ID_TILE_SELECTION_COMMAND_FACTORY,
  CLASS_ID_ACTION_COORDINATOR,
  CLASS_ID_ACTION_COST,
  CLASS_ID_KEYBOARD_COMMAND_MAP,
  CLASS_ID_EQUIPMENT_KEYBOARD_COMMAND_MAP,
  CLASS_ID_INVENTORY_KEYBOARD_COMMAND_MAP,
  CLASS_ID_TILE_SELECTION_KEYBOARD_COMMAND_MAP,
  CLASS_ID_ACTION_MANAGER,
  CLASS_ID_TILE_SELECTION_MANAGER,
  CLASS_ID_WORLD,
  CLASS_ID_CALENDAR,
  CLASS_ID_SPRING,
  CLASS_ID_SUMMER,
  CLASS_ID_AUTUMN,
  CLASS_ID_WINTER,
  CLASS_ID_WORLD_TIME_KEEPER,
  CLASS_ID_TIME_OBSERVER,
  CLASS_ID_CREATURE_TIME_OBSERVER,
  CLASS_ID_SEASONS_TIME_OBSERVER,
  CLASS_ID_MAP_REGISTRY,
  CLASS_ID_MAP,
  CLASS_ID_DIMENSIONS,
  CLASS_ID_DEPTH,
  CLASS_ID_MAP_EXIT
};

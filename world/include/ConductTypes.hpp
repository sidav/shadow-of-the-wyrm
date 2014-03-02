#pragma once

enum ConductType
{
  CONDUCT_TYPE_FOODLESS = 0,
  CONDUCT_TYPE_VEGETARIAN = 1,
  CONDUCT_TYPE_AGNOSTIC = 2,
  CONDUCT_TYPE_ILLITERATE = 3,
  CONDUCT_TYPE_WEAPONLESS = 4, /* Never hits something with a weapon, rather than not wielding one. */
  CONDUCT_SIZE = 5
};

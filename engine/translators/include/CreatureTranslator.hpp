#pragma once
#include <string>
#include "IStatusAilmentTranslator.hpp"

class Creature;
class DisplayStatistics;

class CreatureTranslator
{
  public:
    static DisplayStatistics create_display_statistics(const CreaturePtr& creature, const MapPtr& map);

  protected:
    CreatureTranslator();

    static std::string get_display_name(const CreaturePtr& c);
    static std::string get_display_synopsis(const CreaturePtr& c);

    static std::pair<std::string, Colour> get_display_strength(const CreaturePtr& c);
    static std::pair<std::string, Colour> get_display_dexterity(const CreaturePtr& c);
    static std::pair<std::string, Colour> get_display_agility(const CreaturePtr& c);
    static std::pair<std::string, Colour> get_display_health(const CreaturePtr& c);
    static std::pair<std::string, Colour> get_display_intelligence(const CreaturePtr& c);
    static std::pair<std::string, Colour> get_display_willpower(const CreaturePtr& c);
    static std::pair<std::string, Colour> get_display_charisma(const CreaturePtr& c);

    static std::string get_display_speed(const CreaturePtr& c);
    static std::string get_display_level(const CreaturePtr& c);
    static std::string get_display_defence(const CreaturePtr& c);

    static std::string get_display_hit_points(const CreaturePtr& c);
    static std::string get_display_arcana_points(const CreaturePtr& c);

    static std::vector<std::pair<std::string, Colour>> get_display_status_ailments(const CreaturePtr& c);

    // Used to initialize/check all the various status ailments:
    static void initialize_status_ailment_checkers();
    static std::vector<IStatusAilmentTranslatorPtr> status_ailment_checkers;
};

#include "Date.hpp"

using namespace std;

const uint TimeOfDayConstants::TIME_OF_DAY_DAWN = 7;
const uint TimeOfDayConstants::TIME_OF_DAY_DUSK = 19;
std::map<TimeOfDayType, std::pair<Colour, Colour>> TimeOfDayConstants::time_of_day_colour_overrides;

pair<Colour, Colour> TimeOfDayConstants::get_time_of_day_colours(const TimeOfDayType tod, const bool on_overworld_map)
{
  if (time_of_day_colour_overrides.empty())
  {
    initialize_colour_overrides();
  }

  pair<Colour, Colour> overrides = make_pair(Colour::COLOUR_UNDEFINED, Colour::COLOUR_UNDEFINED);
  auto t_it = time_of_day_colour_overrides.find(tod);

  if (on_overworld_map && t_it != time_of_day_colour_overrides.end())
  {
    overrides = t_it->second;
  }

  return overrides;
}

void TimeOfDayConstants::initialize_colour_overrides()
{
  time_of_day_colour_overrides.clear();

  time_of_day_colour_overrides = { {TimeOfDayType::TIME_OF_DAY_NIGHT, make_pair(Colour::COLOUR_BLUE, Colour::COLOUR_BOLD_BLUE)} };
}

string Date::month_sids[DateValues::NUMBER_OF_MONTHS] = {"MONTH_1", "MONTH_2", "MONTH_3", "MONTH_4", "MONTH_5", "MONTH_6", "MONTH_7", "MONTH_8", "MONTH_9", "MONTH_10", "MONTH_11", "MONTH_12"};
string Date::day_of_week_sids[DateValues::NUMBER_OF_DAYS] = {"DAY_1", "DAY_2", "DAY_3", "DAY_4", "DAY_5", "DAY_6", "DAY_7"};

Date::Date(const uint sec, const uint min, const uint hrs, const uint day_week, const uint day_month, const uint day_year, const uint mnth, const uint yr)
: seconds(sec), minutes(min), hours(hrs), day_of_week(day_week), day_of_month(day_month), day_of_year(day_year), month(mnth), year(yr)
{
}

uint Date::get_seconds() const
{
  return seconds;
}

uint Date::get_minutes() const
{
  return minutes;
}

uint Date::get_hours() const
{
  return hours;
}

TimeOfDayType Date::get_time_of_day() const
{
  TimeOfDayType dtype = TimeOfDayType::TIME_OF_DAY_NIGHT;

  if (hours >= TimeOfDayConstants::TIME_OF_DAY_DAWN && hours < TimeOfDayConstants::TIME_OF_DAY_DUSK)
  {
    dtype = TimeOfDayType::TIME_OF_DAY_DAY;  
  }

  return dtype;
}

uint Date::get_day_of_week() const
{
  return day_of_week;
}

string Date::get_day_of_week_sid() const
{
  return day_of_week_sids[day_of_week];
}

uint Date::get_day_of_month() const
{
  return day_of_month;
}

uint Date::get_day_of_year() const
{
  return day_of_year;
}

uint Date::get_month() const
{
  return month;
}

string Date::get_month_sid() const
{
  return month_sids[month];
}

uint Date::get_year() const
{
  return year;
}

#ifdef UNIT_TESTS
#include "unit_tests/Date_test.cpp"
#endif

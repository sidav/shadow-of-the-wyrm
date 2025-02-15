#pragma once
#ifdef _MSC_VER
#include <curses.h>
#else
#define NCURSES_OPAQUE 0
#include <ncurses.h>
#endif
#include <deque>
#include "Display.hpp"
#include "CursesPromptProcessor.hpp"
#include "ISerializable.hpp"
#include "MenuWrapper.hpp"
#include "TextComponent.hpp"
#include "OptionsComponent.hpp"

class CursesDisplay : public Display
{
  public:
    CursesDisplay();
    bool operator==(const CursesDisplay& cd) const;

	  bool create() override;
	  void tear_down() override;

    unsigned int get_width() const override;
    unsigned int get_height() const override;

    void clear_messages() override;
    void clear_display() override;

    // Adds a message in the current window, clearing any existing messages and
    // requiring user input.
    void add_alert(const std::string& message, const bool require_input) override;

    // Functions to add messages to the message buffer.
    void add_message(const std::string& message, const bool reset_prompt) override;
    void add_message(const std::string& message, const Colour colour, const bool reset_prompt) override;
    std::string add_message_with_prompt(const std::string& message, const Colour colour, const bool reset_prompt) override;

    void halt_messages() override;

	  void draw(const DisplayMap& current_map, const CursorSettings cs) override;
    void redraw() override;

    virtual void draw_update_map(const DisplayMap& update_map, const CursorSettings cs) override;
    virtual void draw_tile(const uint y, const uint x, const DisplayTile& tile) override;

    // Create an animation factory for the display type.
    AnimationFactoryPtr create_animation_factory() const override;
    virtual void draw_animation(const Animation& animation) override;

    virtual void draw_coordinate(const DisplayTile& current_tile, const uint terminal_row, const uint terminal_col);

	  void display(const DisplayStatistics& player_stats) override;
	  
    MapDisplayArea get_map_display_area() override;

	  std::string display_screen(const Screen& current_screen) override;
    void display_header(const std::string& header_text, WINDOW* cur_window, const int display_line = 0);

	  void confirm(const std::string& confirmation_message) override;

	  void clear_screen() override;

    virtual Display* clone() override;

    WINDOW* get_current_screen();
    WINDOW* get_message_buffer_screen();

    // Creation/destruction methods.
    // Generally, don't use these.  Know what you're doing!
    WINDOW* create_screen(int height, int width, int start_row, int start_col);
    void destroy_screen(WINDOW *screen);

    bool serialize(std::ostream& stream) const override;
    bool deserialize(std::istream& stream) override;

  private:
    ClassIdentifier internal_class_identifier() const override;

  protected:
    friend class SW_Display_CursesDisplayFixture; // test fixture
   
    bool uses_colour() const;
    int get_cursor_mode(const CursorSettings cs) const;

    // Setup colours the way ncurses requires.
    void initialize_colours();

    // These two methods are used to turn on/off colours for use
    // with mvprintw, etc.
    void enable_colour(const int colour, WINDOW* window);
    void disable_colour(const int colour, WINDOW* window);

    // Set colour sets the colour without checking to see if we're in
    // monochrome mode or not.
    void set_colour(const int colour, WINDOW* window);

    // Clear the message buffer.
    int clear_message_buffer();

    void refresh_terminal_size();
    void display_text_component(WINDOW* window, int* row, int* col, TextComponentPtr text_component, const uint line_increment);
    void display_options_component(WINDOW* window, int* row, int* col, OptionsComponentPtr options_component);
    std::string display_prompt(WINDOW* screen_window, PromptPtr prompt, int row = 0, int col = 0);

    // Print the current display statistic at the specified row/column, unless we're in a different row than the initial one, and therefore
    // should line up the column with the next-available, previously-used column from the previous row.
    bool print_display_statistic_and_update_row_and_column(const unsigned int initial_row, unsigned int* current_row, unsigned int* current_col, const std::string& stat, const std::string& next_stat, Colour print_colour = Colour::COLOUR_WHITE);

    // Update the row/column position for the synopsis details.  Return false if we can't do any more updates (have run off the screen).
    // Though, ideally that will never happen.
    bool update_synopsis_row_and_column(const unsigned int initial_row, unsigned int* row, unsigned int* column, const std::string& previous_printed_field, const std::string& next_field);

    uint TERMINAL_MAX_ROWS;
    uint TERMINAL_MAX_COLS;
    uint FIELD_SPACE;

    uint MSG_BUFFER_LAST_Y;
    uint MSG_BUFFER_LAST_X;

    // The display is represented as a stack of windows in ncurses; the game window is the lowest, and any screens
    // or subscreens are layered as new windows on top of that.  Each time a screen is done, a window is popped off
    // the stack and the display is re-drawn.
    std::deque<WINDOW*> screens;
    WINDOW* message_buffer_screen;

    // Used to process the prompt
    CursesPromptProcessor prompt_processor;
    bool can_use_colour;
    Colour mono_colour;
    const int cursor_mode;
};

#include <boost/tokenizer.hpp>
#include "Game.hpp"
#include "TextDisplayFormatter.hpp"
#include "TextFormatSpecifiers.hpp"

using namespace std;
using namespace boost;

vector<string> TextDisplayFormatter::format_text(const string& text) const
{
  vector<string> result;

  Game& game = Game::instance();
  DisplayPtr display = game.get_display();

  if (display)
  {
    uint width = display->get_width();
    size_t str_size = text.size();

    // Check to see if the text is less than the screen width.
    // If it is, just add it.
    // Otherwise, split it up appropriately.
    uint cur_pos = 0;
    char_separator<char> separator(" ", " ", boost::keep_empty_tokens); // Keep the tokens!
    tokenizer<char_separator<char>> tokens(text, separator);
    string current_str;

    // Iterate through all the tokens.  As we consider each token, if its
    // width is less than the width of the display, add it to the current
    // string.  If it would take us over, then we've got the largest string
    // possible: add that string to the list, and start creating a new one
    // with the next token.
    for (tokenizer<char_separator<char>>::iterator t_iter = tokens.begin(); t_iter != tokens.end(); t_iter++)
    {
      string current_token = *t_iter;
      process_token(current_token, result, current_str, cur_pos, width);
    }

    // Once we're done, we'll still potentially have text that has not been
    // added yet.  Add this, as well.
    if (!current_str.empty())
    {
      result.push_back(current_str);
    }
  }

  return result;
}

void TextDisplayFormatter::process_token(const string& current_token, vector<string>& result, string& current_str, uint& cur_pos, const uint width) const
{
  // If the token is "%p" (a "new paragraph" token), then finish adding
  // the current line, add another blank line, and be sure to discard
  // the "%p" token.
  // JCD FIXME: If I add quite a few more text format specifiers, break
  // this up into some kind of processing class.
  if (current_token == TextFormatSpecifiers::NEW_PARAGRAPH)
  {
    result.push_back(current_str);
    current_str.clear();
    cur_pos = 0;

    result.push_back(current_str);
  }
  else
  {
    if ((cur_pos + current_token.length()) > (width-1))
    {
      result.push_back(current_str);
      current_str.clear();
      cur_pos = 0;
    }

    current_str.append(current_token);
    cur_pos += current_token.size();
  }
}
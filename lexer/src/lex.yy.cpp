// lex.yy.cpp generated by reflex 1.4.3 from quack.lxx

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define REFLEX_OPTION_YYLTYPE             yy::location
#define REFLEX_OPTION_YYSTYPE             yy::parser::semantic_type
#define REFLEX_OPTION_bison_cc            true
#define REFLEX_OPTION_bison_cc_namespace  yy
#define REFLEX_OPTION_bison_cc_parser     parser
#define REFLEX_OPTION_bison_locations     true
#define REFLEX_OPTION_header_file         "lex.yy.h"
#define REFLEX_OPTION_lex                 yylex
#define REFLEX_OPTION_lexer               Lexer
#define REFLEX_OPTION_namespace           yy
#define REFLEX_OPTION_noyywrap            true
#define REFLEX_OPTION_outfile             "lex.yy.cpp"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top{ user code %}                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 1 "quack.lxx"

/* Quack scanner
 *
 * 2018 Fall version for RE/flex,
 * skeleton.
 *
 */

#include "quack.tab.hxx"  /* Generated by bison. */
#include "Messages.h"


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ABSTRACT LEXER CLASS                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/abslexer.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LEXER CLASS                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

namespace yy {

class Lexer : public reflex::AbstractLexer<reflex::Matcher> {
 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  Lexer(
      const reflex::Input& input = reflex::Input(),
      std::ostream&        os    = std::cout)
    :
      AbstractBaseLexer(input, os)
  {
  }
  static const int INITIAL = 0;
  static const int single_line_comment = 1;
  static const int comment = 2;
  static const int string = 3;
  static const int triple_quote = 4;
  virtual void yylloc_update(yy::location& yylloc)
  {
    yylloc.begin.line = static_cast<unsigned int>(matcher().lineno());
    yylloc.begin.column = static_cast<unsigned int>(matcher().columno());
    yylloc.end.line = static_cast<unsigned int>(yylloc.begin.line + matcher().lines() - 1);
    yylloc.end.column = static_cast<unsigned int>(yylloc.begin.column + matcher().columns() - 1);
  }
  virtual int yylex(yy::parser::semantic_type *lvalp, yy::location *llocp)
  {
    return yylex(*lvalp, *llocp);
  }
  virtual int yylex(yy::parser::semantic_type& yylval, yy::location& yylloc);
};

} // namespace yy

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %{ user code %}                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 13 "quack.lxx"

/* Externally visible file name --
* for error messages.
* FIXME: Need to extract this from structures that reflex passes
* or else find another way to dig it out.
*/
std::string yyfilename = "What file is this, anyway?";

/* Some strings can't be matched in one gulp.  We need
 * a buffer in which to assemble them from parts.
 */
std::string string_buf = "";

void yyerror (const std::string &msg, yy::position* where) {
     std::cout << where << ": " << msg;
}

/* Some long messages that don't fit well in the code below */

std::string BAD_ESC_MSG =
  "Illegal escape code; only \\\\, \\0, \\t, \\n, \\r, \\n are permitted";
std::string BAD_NL_STR =
  "Unclosed string?  Encountered newline in quoted string.";


#line 43 "quack.lxx"
/* You'll probably want scanner states for both multi-line
   * comments and for triple-quote strings.  Do comments first,
   * because they're easier (you just throw away the text).  Then
   * do triple-quote strings.  You might consider doing the same for
   * normal strings with a single pair of double quotes, to avoid
   * writing a really ugly regex.  'comment' here is an example of
   * how to declare an exclusive scanner state.
   */

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  BISON C++                                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 2: rules                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int yy::Lexer::yylex(yy::parser::semantic_type& yylval, yy::location& yylloc)
{
  static const char *REGEX_INITIAL = "(?m)(==)|([<]=)|([>]=)|([/][/])|([*+\\x2d/])|([(),\\x2e;->\\x7b\\x7d])|(class)|(def)|(extends)|(if)|(elif)|(else)|(while)|(return)|(and)|(or)|(not)|([0-9]+)|(:[\\x09\\x20]*[0-9A-Z_a-z]*)|([A-Z_a-z][0-9A-Z_a-z]*)|([\"][\"][\"])|(\")|([\\x09\\x0a\\x20]*)|([/][\\x2a])|(.)";
  static const reflex::Pattern PATTERN_INITIAL(REGEX_INITIAL);
  static const char *REGEX_single_line_comment = "(?m)(\\n)|(.)";
  static const reflex::Pattern PATTERN_single_line_comment(REGEX_single_line_comment);
  static const char *REGEX_comment = "(?m)([^\\x2a]*)|([\\x2a][^/])|([\\x2a][/])";
  static const reflex::Pattern PATTERN_comment(REGEX_comment);
  static const char *REGEX_string = "(?m)(\\n)|(\\\\[^\"0\\x5cnrt])|(\\\\\")|(\\\\.)|(\")";
  static const reflex::Pattern PATTERN_string(REGEX_string);
  static const char *REGEX_triple_quote = "(?m)(\"\"\")|(.)";
  static const reflex::Pattern PATTERN_triple_quote(REGEX_triple_quote);
  if (!has_matcher())
  {
    matcher(new Matcher(PATTERN_INITIAL, stdinit(), this));
  }
  switch (start())
  {
    case INITIAL:
#line 63 "quack.lxx"
/* We start with some tokens that are not value-bearing,
   * i.e., the parser needs to know only which token was matched.
   */

  /* Multi-character punctuation with names */
    break;
  }
  while (true)
  {
    switch (start())
    {
      case INITIAL:
        matcher().pattern(PATTERN_INITIAL);
        matcher().scan();
        yylloc_update(yylloc);
        switch (matcher().accept())
        {
          case 0:
            if (matcher().at_end())
            {
#line 162 "quack.lxx"
{ return EOF; }

            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 63: ==
#line 63 "quack.lxx"
{ return parser::token::EQUALS; }
            break;
          case 2: // rule at line 64: [<]=
#line 64 "quack.lxx"
{ return parser::token::ATMOST; }
            break;
          case 3: // rule at line 65: [>]=
#line 65 "quack.lxx"
{ return parser::token::ATLEAST;}


            break;
          case 4: // rule at line 70: [/][/]
#line 70 "quack.lxx"
{ start(single_line_comment); }
            break;
          case 5: // rule at line 81: [*+\x2d/]
#line 81 "quack.lxx"
{ return text()[0]; }
            break;
          case 6: // rule at line 82: [(),\x2e;->\x7b\x7d]
#line 82 "quack.lxx"
{ return text()[0]; }

  /* Keywords are essentially another kind of punctuation,
   * but since they also match the identifier pattern, we
   * need to be sure we give their patterns *before* the
   * identifier pattern.  Most scanner generators work this
   * way, but WARNING for Python PLY users:  PLY prioritizes
   * by length of the regular expression!
   * Here are a couple of examples.
   */

            break;
          case 7: // rule at line 93: class
#line 93 "quack.lxx"
{ return parser::token::CLASS; }
            break;
          case 8: // rule at line 94: def
#line 94 "quack.lxx"
{ return parser::token::DEF; }
            break;
          case 9: // rule at line 95: extends
#line 95 "quack.lxx"
{ return parser::token::EXTENDS; }
            break;
          case 10: // rule at line 96: if
#line 96 "quack.lxx"
{ return parser::token::IF; }
            break;
          case 11: // rule at line 97: elif
#line 97 "quack.lxx"
{ return parser::token::ELIF; }
            break;
          case 12: // rule at line 98: else
#line 98 "quack.lxx"
{ return parser::token::ELSE; }
            break;
          case 13: // rule at line 99: while
#line 99 "quack.lxx"
{ return parser::token::WHILE; }
            break;
          case 14: // rule at line 100: return
#line 100 "quack.lxx"
{ return parser::token::RETURN; }
            break;
          case 15: // rule at line 101: and
#line 101 "quack.lxx"
{ return parser::token::AND; }
            break;
          case 16: // rule at line 102: or
#line 102 "quack.lxx"
{ return parser::token::OR;  }
            break;
          case 17: // rule at line 103: not
#line 103 "quack.lxx"
{ return parser::token::NOT; }


   /* The following tokens are value-bearing:
    * We pass a value back to the parser by copying
    * it into the yylval parameter.  The parser
    * expects string values for identifiers and
    * string literals in yylval.str.  It expects integer
    * values for integer literals in yylval.num.
    * I'll give pattern for integer literals.
    * You'll need another for identifiers.  Remember to
    * 'strdup' the value returned by 'text()', or else it will
    * be clobbered by subsequent calls.
    */

            break;
          case 18: // rule at line 118: [0-9]+
#line 118 "quack.lxx"
{ yylval.num = atoi(text()); return parser::token::INT_LIT; }
            break;
          case 19: // rule at line 119: :[\x09\x20]*[0-9A-Z_a-z]*
#line 119 "quack.lxx"
{ yylval.str = strdup(text()); return parser::token::TYPECASE; }
            break;
          case 20: // rule at line 120: [A-Z_a-z][0-9A-Z_a-z]*
#line 120 "quack.lxx"
{ yylval.str = strdup(text()); return parser::token::IDENT; }

            break;
          case 21: // rule at line 122: ["]["]["]
#line 122 "quack.lxx"
{ start(triple_quote); }
            break;
          case 22: // rule at line 135: "
#line 135 "quack.lxx"
{ start(string); }
            break;
          case 23: // rule at line 146: [\x09\x0a\x20]*
#line 146 "quack.lxx"
{ ; }

   /* Multi-line comments */
            break;
          case 24: // rule at line 149: [/][\x2a]
#line 149 "quack.lxx"
{ start(comment); }
            break;
          case 25: // rule at line 157: .
#line 157 "quack.lxx"
{   report::error("Unexpected character '" + std::string(text()) + "'" +
       " at line " + std::to_string(lineno()) +
       ", column " + std::to_string(columno()));
   }

            break;
        }
        break;
      case single_line_comment:
        matcher().pattern(PATTERN_single_line_comment);
        matcher().scan();
        yylloc_update(yylloc);
        switch (matcher().accept())
        {
          case 0:
            if (matcher().at_end())
            {
#line 162 "quack.lxx"
{ return EOF; }

            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 71: \n
#line 71 "quack.lxx"
{ start(INITIAL); }
            break;
          case 2: // rule at line 72: .
#line 72 "quack.lxx"
{ ; }


   /* Single character punctuation.  Because a character
    * in C or C++ can be treated as an integer, Bison lets
    * us return a character as a token.
    * JFlex/Cup do not let you do this in Java.
    * Here are a few ... there are more.
    */
            break;
        }
        break;
      case comment:
        matcher().pattern(PATTERN_comment);
        matcher().scan();
        yylloc_update(yylloc);
        switch (matcher().accept())
        {
          case 0:
            if (matcher().at_end())
            {
#line 150 "quack.lxx"
{ report::error("Comment not terminated before EOF " + std::string(text())); }
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 151: [^\x2a]*
#line 151 "quack.lxx"
{ ; }
            break;
          case 2: // rule at line 152: [\x2a][^/]
#line 152 "quack.lxx"
{ ; }
            break;
          case 3: // rule at line 153: [\x2a][/]
#line 153 "quack.lxx"
{ start(INITIAL); }



            break;
        }
        break;
      case string:
        matcher().pattern(PATTERN_string);
        matcher().scan();
        yylloc_update(yylloc);
        switch (matcher().accept())
        {
          case 0:
            if (matcher().at_end())
            {
#line 162 "quack.lxx"
{ return EOF; }

            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 136: \n
#line 136 "quack.lxx"
{ report::error("Single quoted string contains illegal newline " + std::string(text()) + " at line " + std::to_string(lineno())); }
            break;
          case 2: // rule at line 137: \\[^"0\x5cnrt]
#line 137 "quack.lxx"
{ report::error(BAD_ESC_MSG + std::string(text()) + " at line "+std::to_string(lineno())); }
            break;
          case 3: // rule at line 138: \\"
#line 138 "quack.lxx"
{ ; }
            break;
          case 4: // rule at line 139: \\.
#line 139 "quack.lxx"
{ ; }
            break;
          case 5: // rule at line 140: "
#line 140 "quack.lxx"
{ yylval.str = strdup(text()); start(INITIAL); return parser::token::STRING_LIT; }




  /* Ignore whitespace */
            break;
        }
        break;
      case triple_quote:
        matcher().pattern(PATTERN_triple_quote);
        matcher().scan();
        yylloc_update(yylloc);
        switch (matcher().accept())
        {
          case 0:
            if (matcher().at_end())
            {
#line 162 "quack.lxx"
{ return EOF; }

            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 123: """
#line 123 "quack.lxx"
{ yylval.str = strdup(text()); start(INITIAL); return parser::token::STRING_LIT; }
            break;
          case 2: // rule at line 124: .
#line 124 "quack.lxx"
{ ; }


  /* You *can* write a one-line regular expression for matching a quoted string,
   * but you probably can't read it.  (I can't read mine.)  Scanner states and
   * simpler patterns are probably a better idea.
   * You almost certainly scanner states for triple quotes.
   * The tricky bit is catching errors, such as disallowed escape sequences
   * and unclosed quotes.
   */

            break;
        }
        break;
      default:
        start(0);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 3: user code                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 165 "quack.lxx"

/* No main program here */


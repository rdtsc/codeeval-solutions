%{
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define BASE_TYPE long double
#define BASE_TYPE_FORMAT "Lf"

typedef struct Symbol
{
  int type;
  const char* name;
  union
  {
    BASE_TYPE payload;
    BASE_TYPE(* delegate)(BASE_TYPE);
  } trait;
} Symbol;

static int yylex();
static void yyerror(const char*);
static BASE_TYPE factorial(BASE_TYPE);
static void dump(FILE*, BASE_TYPE, unsigned);

static FILE* inputStream;
%}

%union
{
  BASE_TYPE value;
  const Symbol* symbol;
}

%token <value>  VALUE
%token <symbol> CONSTANT FUNCTION
%type  <value>  exp
%left '+' '-'
%left '*' '/'
%left '%'
%left UNEG
%right '^' '!'

%%
input: | input line;

line: '\n' | exp '\n' { dump(stdout, $1, 5); };

exp: VALUE                { $$ = $1;                     } |
     CONSTANT             { $$ = $1->trait.payload;      } |
     FUNCTION '(' exp ')' { $$ = $1->trait.delegate($3); } |
     exp '+' exp          { $$ = $1 + $3;                } |
     exp '-' exp          { $$ = $1 - $3;                } |
     exp '*' exp          { $$ = $1 * $3;                } |
     exp '/' exp          { $$ = $1 / $3;                } |
     '-' exp %prec UNEG   { $$ = -$2;                    } |
     exp '%' exp          { $$ = fmodl($1, $3);          } |
     exp '^' exp          { $$ = powl($1, $3);           } |
     exp '!'              { $$ = factorial($1);          } |
     '|' exp '|'          { $$ = ($2 < 0) ? -$2 : $2;    } |
     '(' exp ')'          { $$ = $2;                     } ;
%%

static int yylex()
{
  static const Symbol symbols[] =
  {
    #define REGISTER_FUNCTION(Name, Function) { \
      .type = FUNCTION,                         \
      .name = (#Name),                          \
      .trait.delegate = (Function)              \
    }

    #define REGISTER_CONSTANT(Name, Value) { \
      .type = CONSTANT,                      \
      .name = (#Name),                       \
      .trait.payload = (Value)               \
    }

    REGISTER_CONSTANT(Pi, 3.1415926535897932384626433832795L),
    REGISTER_CONSTANT(e,  2.7182818284590452353602874713527L),

    REGISTER_FUNCTION(sqrt, sqrtl),
    REGISTER_FUNCTION(cos,   cosl),
    REGISTER_FUNCTION(sin,   sinl),
    REGISTER_FUNCTION(tan,   tanl),
    REGISTER_FUNCTION(lg,  log10l),
    REGISTER_FUNCTION(ln,    logl)

    #undef REGISTER_CONSTANT
    #undef REGISTER_FUNCTION
  };

  int cursor = 0;

  while((cursor = fgetc(inputStream)) == ' ');

  // Fetch numbers.
  if(isdigit(cursor) || (cursor == '.'))
  {
    ungetc(cursor, inputStream);
    fscanf(inputStream, "%" BASE_TYPE_FORMAT, &yylval.value);
    return VALUE;
  }

  // Fetch symbols.
  else if(isalpha(cursor))
  {
    char symbolName[32] = "";

    for(char* c = symbolName; isalpha(cursor) && (cursor != EOF); ++c)
    {
      assert((size_t)(c - symbolName) < sizeof symbolName);

      *c = cursor;
      cursor = fgetc(inputStream);
    }

    ungetc(cursor, inputStream);

    // Account for modulus.
    if(!strcmp(symbolName, "mod")) return '%';

    const Symbol* symbol = NULL;

    // Find the symbol in the base table.
    for(size_t i = 0; i < (sizeof symbols / sizeof *symbols); ++i)
    {
      if(!strcmp(symbolName, symbols[i].name))
      {
        symbol = &symbols[i];
        break;
      }
    }

    assert(symbol);

    yylval.symbol = symbol;
    return symbol->type;
  }

  return (cursor == EOF ? 0 : cursor);
}

static void yyerror(const char* const message)
{
  assert(message);
  fputs(message, stderr);
  fputc('\n', stderr);
}

static BASE_TYPE factorial(const BASE_TYPE value)
{
  return (value <= 1) ? (value) : (value * factorial(value - 1));
}

static void dump(FILE* const outputStream,
                 const BASE_TYPE value,
                 const unsigned precision)
{
  assert(outputStream && !ferror(outputStream));

  static char outputBuffer[64] = "";

  // Inject value into buffer.
  {
    static const char* format = "%.*" BASE_TYPE_FORMAT;

    snprintf(outputBuffer, sizeof(outputBuffer), format, precision, value);
  }

  // Kill trailing zeros.
  {
    assert(strlen(outputBuffer) >= 1);

    char* cursor = &outputBuffer[strlen(outputBuffer) - 1];

    while(*cursor == '0') *cursor-- = '\0';

    if(*cursor == '.') *cursor = '\0';
  }

  fputs(outputBuffer, outputStream);
  fputc('\n', outputStream);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[1024] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  yyparse();

  // The CRT takes care of cleanup.
}

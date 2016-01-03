%{
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define YYSTYPE long double
#define TYPE_FORMAT "Lf"

static int yylex();
static void yyerror(const char*);
static void dump(FILE*, YYSTYPE, unsigned);

static FILE* inputStream;
%}

%token VAL
%left '+' '-'
%left '*' '/'
%left UNEG
%right '^'

%%
input: | input line;

line: '\n' | exp '\n' { dump(stdout, $1, 5); };

exp: VAL                 { $$ = $1;           } |
     exp '+' exp         { $$ = $1 + $3;      } |
     exp '-' exp         { $$ = $1 - $3;      } |
     exp '*' exp         { $$ = $1 * $3;      } |
     exp '/' exp         { $$ = $1 / $3;      } |
     '-' exp  %prec UNEG { $$ = -$2;          } |
     exp '^' exp         { $$ = powl($1, $3); } |
     '(' exp ')'         { $$ = $2;           } ;
%%

static int yylex()
{
  int cursor = 0;

  while((cursor = fgetc(inputStream)) == ' ');

  if(isdigit(cursor) || (cursor == '.'))
  {
    ungetc(cursor, inputStream);
    fscanf(inputStream, "%" TYPE_FORMAT, &yylval);
    return VAL;
  }

  return (cursor == EOF ? 0 : cursor);
}

static void yyerror(const char* const message)
{
  assert(message);
  fputs(message, stderr);
  fputc('\n', stderr);
}

static void dump(FILE* const outputStream,
                 const YYSTYPE value,
                 const unsigned precision)
{
  assert(outputStream && !ferror(outputStream));

  static char outputBuffer[64] = "";

  // Inject value into buffer.
  {
    static const char* format = "%.*" TYPE_FORMAT;

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

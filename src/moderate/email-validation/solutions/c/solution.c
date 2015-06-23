#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

// Non-STDC.
#include <regex.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  #define WC   "[a-z0-9]"
  #define WCR  "[a-z0-9-]+"
  #define UCA  "[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+"
  #define UCB  "[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~@-]+"
  #define USER "(" UCA "|\"" UCB "\")"
  #define HOST WC "(" WCR WC ")?(\\." WC "(" WCR WC ")?)*"

  // HACK: This is a terrible way to check for email address validity and
  // does not satisfy RFC-5322 in any way. It should be noted that the W3C
  // HTML5 specification relaxes validity rules a little which results in a
  // more manageable expression than one fully satisfying the RFC above. In
  // light of this, and since the problem statement doesn't give us much
  // information to work with, below is a naive approach of verifying the
  // validity of an email address based on a lax definition of a "valid
  // e-mail address" as described in the HTML5 form specification.
  static const char* const pattern = "^" USER "@" HOST "$";

  regex_t w3cEmailSpec = {0};

  {
    static const int regexFlags =
      (REG_EXTENDED | REG_ICASE | REG_NEWLINE | REG_NOSUB);

    if(regcomp(&w3cEmailSpec, pattern, regexFlags)) assert(false);
  }

  #undef HOST
  #undef USER
  #undef UCB
  #undef UCA
  #undef WCR
  #undef WC

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const bool isValidEmail =
      (regexec(&w3cEmailSpec, lineBuffer, 0, NULL, 0) != REG_NOMATCH);

    puts(isValidEmail ? "true" : "false");
  }

  // The CRT takes care of cleanup.
}

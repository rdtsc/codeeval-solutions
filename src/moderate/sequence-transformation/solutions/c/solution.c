#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct {const char* head, * tail;} Sequence;

static inline bool seqIsWellFormed(const Sequence* const sequence)
{
  const bool isValid = (sequence && sequence->head && sequence->tail);

  return (isValid && (sequence->tail >= sequence->head));
}

static inline void seqInit(const char* const string, Sequence* const target)
{
  assert(string && *string && target);

  target->head = target->tail = string;
  target->tail  += strlen(string);

  assert(seqIsWellFormed(target));
}

static inline bool seqIsExhausted(const Sequence* const sequence)
{
  assert(seqIsWellFormed(sequence));

  return (sequence->head >= sequence->tail);
}

static inline size_t seqGetLength(const Sequence* const sequence)
{
  assert(seqIsWellFormed(sequence));

  return (sequence->tail - sequence->head);
}

static const char* seqGetNextTarget(const Sequence* const sequence)
{
  assert(seqIsWellFormed(sequence));

  const char* result = sequence->head;

  for(char c = '\0'; result < sequence->tail; ++result) if(*result != c)
  {
    if(c) break;

    c = *result;
  }

  return result;
}

static bool seqTryMatch(Sequence* mask, Sequence* target)
{
  assert(seqIsWellFormed(mask) && seqIsWellFormed(target));

  const bool isMaskDrained   = seqIsExhausted(mask),
             isTargetDrained = seqIsExhausted(target);

  // The mask and resulting sequences should be exhausted if there's a match.
  if(isMaskDrained) return isTargetDrained;

  // Only 1's can be mapped to B's.
  else if((*target->head == 'B') && (*mask->head != '1')) return false;

  // Still have some unused characters.
  else if(isTargetDrained) return false;

  // More mask characters left than sequence characters.
  else if(seqGetLength(mask) > seqGetLength(target)) return false;

  ++mask->head;

  const char* const nextTarget = seqGetNextTarget(target);

  Sequence rhs = {nextTarget, target->tail};

  if((rhs.head == rhs.tail) || seqTryMatch(mask, &rhs)) return true;

  target->head++;

  Sequence lhs = *mask;

  return seqTryMatch(&lhs, target);
}

static bool seqIsMatch(Sequence mask, Sequence target)
{
  assert(seqIsWellFormed(&mask) && seqIsWellFormed(&target));

  return seqTryMatch(&mask, &target);
}

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

  for(char lineBuffer[1536] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* lhs = lineBuffer,
              * rhs = lineBuffer;

    // Isolate the two strings.
    {
      char* delimiter = strchr(lineBuffer, ' ');

      assert(delimiter);

      // Kill the space.
      *delimiter++ = '\0';

      rhs = delimiter;

      // Trim the right string.
      for(char* c = delimiter; *c; ++c) if(*c == '\n') *c = '\0';
    }

    /*
      A regex would work nicely here. For instance: '0' -> /(?:A+)/
                                                    '1' -> /(?:(?:A+)|(?:B+))/

      Alas, the speed/memory penalty on CodeEval is too high for now.
    */

    Sequence mask = {0}, target = {0};

    seqInit(lhs, &mask);
    seqInit(rhs, &target);

    puts(seqIsMatch(mask, target) ? "Yes" : "No");
  }

  // The CRT takes care of cleanup.
}

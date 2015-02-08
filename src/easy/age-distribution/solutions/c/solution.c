#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(int_fast32_t age = 0; fscanf(inputStream, "%" SCNdFAST32, &age) == 1;)
  {
    if(age < 0 || age > 100) puts("This program is for humans");

    else puts
    (
      age <=  2 ? "Still in Mama's arms" :
      age <=  4 ? "Preschool Maniac"     :
      age <= 11 ? "Elementary school"    :
      age <= 14 ? "Middle school"        :
      age <= 18 ? "High school"          :
      age <= 22 ? "College"              :
      age <= 65 ? "Working for the man"  :
                  "The Golden Years"
    );
  }

  // The CRT takes care of cleanup.
}

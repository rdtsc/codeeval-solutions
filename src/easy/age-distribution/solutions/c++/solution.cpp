#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using input_iterator  = std::istream_iterator<std::int_fast32_t>;
  using output_iterator = std::ostream_iterator<const char*>;

  std::transform(input_iterator(inputStream),
                 input_iterator(),
                 output_iterator(std::cout, "\n"),
                 [](const input_iterator::value_type age)
  {
    if(age < 0 || age > 100) return "This program is for humans";

    return age <=  2 ? "Still in Mama's arms" :
           age <=  4 ? "Preschool Maniac"     :
           age <= 11 ? "Elementary school"    :
           age <= 14 ? "Middle school"        :
           age <= 18 ? "High school"          :
           age <= 22 ? "College"              :
           age <= 65 ? "Working for the man"  :
                       "The Golden Years";
  });
}

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

namespace Sequence
{
  struct SequenceRange
  {
    using iterator_type = std::string::const_iterator;

    SequenceRange(const std::string& source) : head(source.cbegin()),
                                               tail(source.cend())
    {
    }

    SequenceRange(iterator_type head, iterator_type tail) : head(head),
                                                            tail(tail)
    {
    }

    bool isWellFormed() const
    {
      return (this->tail >= this->head);
    }

    bool isExhausted() const
    {
      assert(this->isWellFormed());

      return (this->head >= this->tail);
    }

    std::size_t getLength() const
    {
      assert(this->isWellFormed());

      return (this->tail - this->head);
    }

    void advance()
    {
      assert(this->isWellFormed());

      ++this->head;
    }

    bool operator<(const SequenceRange& other) const
    {
      assert(this->isWellFormed());

      return (this->getLength() < other.getLength());
    }

    bool operator>(const SequenceRange& other) const
    {
      assert(this->isWellFormed());

      return (other < *this);
    }

    operator bool() const
    {
      assert(this->isWellFormed());

      return !this->isExhausted();
    }

    iterator_type head, tail;
  };

  namespace Details
  {
    static SequenceRange::iterator_type
      getNextTarget(const SequenceRange& sequence)
    {
      assert(sequence.isWellFormed());

      auto result = sequence.head;

      using value_type = SequenceRange::iterator_type::value_type;

      for(value_type c = {}; result < sequence.tail; ++result) if(*result != c)
      {
        if(c) break;

        c = *result;
      }

      return result;
    }

    static bool tryMatch(SequenceRange& mask, SequenceRange& target)
    {
      assert(mask.isWellFormed() && target.isWellFormed());

      // The mask and resulting sequences are exhausted if there's a match.
      if(!mask) return !target;

      // Only 1's can be mapped to B's.
      else if((*target.head == 'B') && (*mask.head != '1')) return false;

      // Still have some unused characters.
      else if(!target) return false;

      // More mask characters left than sequence characters.
      else if(mask > target) return false;

      mask.advance();

      SequenceRange rhs(getNextTarget(target), target.tail);

      if((rhs.head == rhs.tail) || tryMatch(mask, rhs)) return true;

      target.advance();

      SequenceRange lhs(mask);

      return tryMatch(lhs, target);
    }
  }

  static bool isMatch(SequenceRange mask, SequenceRange target)
  {
    assert(mask.isWellFormed() && target.isWellFormed());

    return Details::tryMatch(mask, target);
  }
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::string mask, target; inputStream >> mask >> target;)
  {
    /*
      A regex would work nicely here. For instance: '0' -> /(?:A+)/
                                                    '1' -> /(?:(?:A+)|(?:B+))/

      Alas, the speed/memory penalty on CodeEval is too high for now.
    */

    std::cout << (Sequence::isMatch(mask, target) ? "Yes" : "No") << '\n';
  }
}

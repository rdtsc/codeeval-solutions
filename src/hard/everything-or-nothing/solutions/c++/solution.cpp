#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>

class CodeEvalFacet : public std::numpunct<char>
{
  protected: virtual std::string do_truename() const override
  {
    return "True";
  }

  protected: virtual std::string do_falsename() const override
  {
    return "False";
  }
};

// Values as per the problem statement.
enum class Permission : unsigned char
{
  Grant = 1,
  Write = 2,
  Read  = 4
};

template<std::size_t UserCount, std::size_t FileCount> class Action
{
  /*
    Representing either:
    - "userId=>fileId=>rule", or;
    - "userId=>fileId=>rule=>newRule=>subjectId"
  */

  public: explicit Action(const std::string& actions) :
    userId(), fileId(), rule(), newRule(), subjectId()
  {
    assert(!actions.empty());

    std::istringstream tokenizer(actions);

    std::string chunk;
    unsigned value = 0;

    // User ID.
    if(tokenizer >> chunk >> value)
    {
      assert(this->isUserChunk(chunk, value));

      this->userId = value;
    }

    // File ID.
    if(tokenizer >> chunk >> value)
    {
      assert(this->isFileChunk(chunk, value));

      this->fileId = value;
    }

    // Permission.
    if(tokenizer >> chunk)
    {
      assert(this->isValidAction(chunk));

      this->rule = this->getRuleValue(chunk);
    }

    // Trailing data is only present if this is a grant operation.
    if(this->rule != 1) return;

    // New permission.
    if(tokenizer >> chunk)
    {
      assert(this->isValidAction(chunk));

      this->newRule = this->getRuleValue(chunk);
    }

    // Target user ID.
    if(tokenizer >> chunk >> value)
    {
      assert(this->isUserChunk(chunk, value));

      this->subjectId = value;
    }
  }

  private: bool isUserChunk(const std::string& chunk, const unsigned id) const
  {
    return ((chunk == "user") && (id <= UserCount));
  }

  private: bool isFileChunk(const std::string& chunk, const unsigned id) const
  {
    return ((chunk == "file") && (id <= FileCount));
  }

  private: bool isValidAction(const std::string& action) const
  {
    return (action == "grant" || action == "write" || action == "read");
  }

  private: unsigned char getRuleValue(const std::string& rule) const
  {
    assert(!rule.empty());

    switch(rule.front())
    {
      case 'g': return static_cast<unsigned char>(Permission::Grant);
      case 'w': return static_cast<unsigned char>(Permission::Write);
      case 'r': return static_cast<unsigned char>(Permission::Read);
    }

    assert(false);

    return 0;
  }

  private: template<std::size_t Value> static constexpr std::size_t
    getFieldWidth(const std::size_t value = Value, const std::size_t power = 0)
  {
    return (value <= 1) ?
           (power + ((2 << (power - 1)) < Value)) :
           (getFieldWidth<Value>((value / 2), (power + 1)));
  }

  #define VALUE(Enumerator) static_cast<std::size_t>(Permission::Enumerator)

  private: static constexpr auto maxRuleValue =
    VALUE(Grant) + VALUE(Write) + VALUE(Read);

  #undef VALUE

  #define BASED_ON(Value) getFieldWidth<(Value)>()

  public: unsigned char userId    : BASED_ON(UserCount);
  public: unsigned char fileId    : BASED_ON(FileCount);
  public: unsigned char rule      : BASED_ON(maxRuleValue);
  public: unsigned char newRule   : BASED_ON(maxRuleValue);
  public: unsigned char subjectId : BASED_ON(UserCount);

  #undef BASED_ON
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Set bools to be emitted as their textual equivalents in English.
  std::cout.imbue(std::locale(std::cout.getloc(), [&]
  {
    std::boolalpha(std::cout);

    return new CodeEvalFacet();
  }()));

  // Base access control list as per the problem statement.
  static const std::array<std::array<unsigned char, 3>, 6> defaultAcl =
  {{
    /*
      0: none
      1: grant
      2: write
      3: write, grant
      4: read
      5: read, grant
      6: read, write
      7: read, write, grant
    */

    //           file_1  file_2  file_3
    /* user_1 */  {7,      3,      0},
    /* user_2 */  {6,      2,      4},
    /* user_3 */  {5,      1,      5},
    /* user_4 */  {3,      7,      1},
    /* user_5 */  {6,      0,      2},
    /* user_6 */  {4,      2,      6}
  }};

  using Action = Action<defaultAcl.size(), defaultAcl.front().size()>;

  const auto isNoise = std::not1(std::ptr_fun(::isalnum));

  std::istringstream tokenizer;

  for(std::string line; std::getline(inputStream, line);)
  {
    tokenizer.clear();
    tokenizer.str(line);

    // Revert to stock ACL for each test case.
    auto acl = defaultAcl;

    using input_iterator = std::istream_iterator<std::string>;

    const auto isValidActionChain = std::all_of(input_iterator(tokenizer),
                                                input_iterator(),
                                                [&](std::string actions) -> bool
    {
      // Remove delimiter noise.
      std::replace_if(actions.begin(), actions.end(), isNoise, ' ');

      const Action cmd(actions);

      // IDs are 1-based in the input.
      const auto ownerRights = acl[cmd.userId - 1][cmd.fileId - 1];

      // Self-administer supplied permission.
      if(!cmd.subjectId) return (ownerRights & cmd.rule);

      const auto canGrant =
        (ownerRights & static_cast<decltype(ownerRights)>(Permission::Grant));

      // Insufficient rights.
      if(!canGrant) return false;

      // Apply supplied permission to another user.
      acl[cmd.subjectId - 1][cmd.fileId - 1] |= cmd.newRule;

      return true;
    });

    std::cout << isValidActionChain << '\n';
  }
}

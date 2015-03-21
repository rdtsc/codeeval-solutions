#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>

template<typename T> struct Node
{
  using value_type = T;

  explicit Node(const T value, const std::size_t left = 0,
    const std::size_t right = 0) : value(value),
                                   left(left),
                                   right(right)
  {
  }

  T value;
  std::size_t left, right;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using Node = Node<unsigned>;
  using Leaf = Node;

  static const Node nodes[] =
  {
    /* 0 */  Node(30, 1, 2),  /*      0    */
    /* 1 */  Node(8, 3, 4),   /*     / \   */
    /* 2 */  Leaf(52),        /*    1   2  */
    /* 3 */  Leaf(3),         /*   / \     */
    /* 4 */  Node(20, 5, 6),  /*  3   4    */
    /* 5 */  Leaf(10),        /*     / \   */
    /* 6 */  Leaf(29)         /*    5   6  */
  };

  for(Node::value_type a = 0, b = 0; inputStream >> a >> b;)
  {
    const auto lowestAncestorIndex = [&]
    {
      for(std::size_t i = 0;;)
      {
        const auto nodeValue = nodes[i].value;

        if((a < nodeValue) && (b < nodeValue)) i = nodes[i].left;

        else if((a > nodeValue) && (b > nodeValue)) i = nodes[i].right;

        else return i;
      }
    }();

    assert(lowestAncestorIndex < sizeof nodes / sizeof *nodes);

    std::cout << nodes[lowestAncestorIndex].value << '\n';
  }
}

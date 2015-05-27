#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static unsigned getRouteCount(const int x, const int y)
{
  static bool board[4][4] = {{false}};

  static const size_t boardWidth  = (sizeof *board / sizeof **board),
                      boardHeight = (sizeof  board / sizeof  *board);

  assert(boardWidth == boardHeight && boardWidth == 4 && boardHeight == 4);

  unsigned routeCount = 0;

  // We've reached our destination at the lower-right cell.
  if((size_t)x == (boardWidth - 1) && (size_t)y == (boardHeight - 1))
  {
    return ++routeCount;
  }

  // Guard bounds.
  else if(x < 0 || y < 0 || (size_t)x >= boardWidth || (size_t)y >= boardHeight)
  {
    return routeCount;
  }

  // Unused cell.
  else if(!board[y][x] && (board[y][x] = true))
  {
    #define NEXT(xOffset, yOffset) getRouteCount(x + (xOffset), y + (yOffset))

    // Proceed to horizontal and vertical neighbors.
    routeCount = NEXT(+1, 0) + NEXT(0, +1) +
                 NEXT(-1, 0) + NEXT(0, -1);

    board[y][x] = false;

    #undef NEXT
  }

  return routeCount;
}

int main(void)
{
  printf("%u\n", getRouteCount(0, 0));
}

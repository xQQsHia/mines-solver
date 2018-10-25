/** This file contains a function to print a board in ASCII characters.

    @author Alejandro J. Mujica
*/

# ifndef MINES_PRINT_H
# define MINES_PRINT_H

void print_board(const Board & board)
{
  for (size_t j = 0; j < board.cols() + 2; ++j)
    cout << '-';
  cout << endl;
  
  for (size_t i = 0; i < board.rows(); ++i)
    {
      cout << '|';
      for (size_t j = 0; j < board.cols(); ++j)
	{
	  StatusValue status = board.get_status(i, j);

	  switch (status)
	    {
	    case StatusValue::UNCOVERED:
	      if (board.is_uncovered_mine(i, j))
		cout << '*';
	      else
		{
		  auto n = board.get_num_mines_around(i, j);
		  if (n > 0)
		    cout << n;
		  else
		    cout << ' ';
		}
	      break;
	    case StatusValue::COVERED: cout << '?'; break;
	    case StatusValue::FLAG: cout << 'F';
	    default: break;
	    }
	}
      cout << "|\n";
    }
  for (size_t j = 0; j < board.cols() + 2; ++j)
    cout << '-';
  cout << endl;
}

# endif

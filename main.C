/** This is a demo program to show your solution behavior.
    
    @author Alejandro J. Mujica
*/

# include <iostream>
# include <chrono>
# include <utility>
# include <list>
# include <cassert>

using namespace std;
using namespace chrono;

# include <tclap/CmdLine.h>

using namespace TCLAP;

# include <board.H>
# include <mines-print.H>

using Position = pair<size_t, size_t>;

ostream & operator << (ostream & out, const Position & p)
{
  out << '(' << get<0>(p) << ',' << get<1>(p) << ')';
  return out;
}

void print_position_set(const list<Position> & ps)
{
  for (auto p : ps)
    cout << p;
  cout << endl;
}

# include <solver.H>

int parse(int argc, char * argv[])
{
  CmdLine cmd(argv[0], ' ', "0.0");

  ValueArg<size_t> height("l", "height", "matrix height", false, 8,
			  "matrix height");

  cmd.add(height);

  ValueArg<size_t> width("w", "width", "matrix width", false, 8,
			 "matrix width");

  cmd.add(width);

  ValueArg<size_t> num_mines("m", "num-mines", "number of mines", false, 10,
			     "number of mines");

  cmd.add(num_mines);

  ValueArg<unsigned> seed("s", "seed", "seed for random number generator",
			  false,
			  system_clock::now().time_since_epoch().count(),
			  "seed for random number generator");

  cmd.add(seed);

  ValueArg<string> file_name("f", "file", "file with board for testing",
			     false, "board-8x8x10.txt",
			     "name of file with board for testing");
  
  cmd.add(file_name);

  cmd.parse(argc, argv);

  if (height.getValue() * width.getValue() < num_mines.getValue())
    {
      cout << "Error: Too much mines for matrix\n";
      return 1;
    }

  mt19937 g(seed.getValue() % mt19937::max());

  Board board(height.getValue(), width.getValue(), num_mines.getValue());

  {
    ifstream in(file_name.getValue());
    board.load(in);
    in.close();
  }
  /* Puedes sustituir todo el bloque anterior por board.generate_random(g) si
     deseas un tablero aleatorio, sólo que para eso no hay casos de prueba.
  */

  print_board(board);

  {
    Position p;

    uniform_int_distribution<int> unif_r(0, board.rows() - 1);
    uniform_int_distribution<int> unif_c(0, board.cols() - 1);

    p.first = unif_r(g);
    p.second = unif_c(g);

    list<Position> s = get_covered_positions_around(board, p);

    cout << "get_covered_positions_around(board, " << p << ") = ";
    print_position_set(s);
  }
  
  {
    Position p;

    uniform_int_distribution<int> unif_r(0, board.rows() - 1);
    uniform_int_distribution<int> unif_c(0, board.cols() - 1);

    p.first = unif_r(g);
    p.second = unif_c(g);

    list<Position> s = get_not_uncovered_positions_around(board, p);
    cout << "get_not_uncovered_positions_around(board, " << p << ") = ";
    print_position_set(s);
  }
  
  {
    list<Position> s = get_util_uncovered(board);
    cout << "get_util_uncovered(board) = ";
    print_position_set(s);
  }
  
  {
    pair<bool, Position> s = get_first(board, 2);

    cout << "get_first(board, 2) = ("
	 << (s.first ? "true" : "false") << ',' << s.second << ')' << endl;
  }
  
  {
    list<Position> s = get_covered_positions(board);
    cout << "get_covered_positions(board) = ";
    print_position_set(s);
  }
  
   
  {
    pair<bool, Position> s = select_random_covered(board, g);
    cout << "select_random_covered(board, g) = ("
	 << (s.first ? "true" : "false") << ',' << s.second << ')' << endl;
  }
  
  {    
    pair<bool, Position> s = get_a_position(board, g);
    cout << "get_a_position(board, g) = ("
	 << (s.first ? "true" : "false") << ',' << s.second << ')' << endl;
  }
  
  {
    list<Position> s = get_for_flags(board);
    cout << "get_for_flags(board) = ";
    print_position_set(s);

    for (auto p : s)
      {
	assert(board.hint(p.first, p.second).first);
	board.flag(get<0>(p), get<1>(p));
      }
  }
  
  {
    list<Position> s = get_for_discover(board);
    for (auto p : s)
      assert(not board.hint(p.first, p.second).first);
    
    cout << "get_for_discover(board) = ";
    print_position_set(s);
  }

  solve(board, g);
  cout << "solve(board, g) =\n";
  print_board(board);
   
  return 0;
}

int main(int argc, char * argv[])
{
  return parse(argc, argv);
}

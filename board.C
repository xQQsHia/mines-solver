/** This file contains the implementation of a mines board.
    
    @author Alejandro J. Mujica
*/

# include <sstream>
# include <utility>
# include <stack>
# include <set>

# include <board.H>

using namespace std;

bool Board::is_mine(size_t i, size_t j) const
{
  if (i >= rows() or j >= cols())
    return false;

  return matrix[i][j].second;
}

size_t Board::count_mines_around(size_t i, size_t j) const
{
  size_t ret_val = 0;
  
  ret_val += is_mine(i - 1, j - 1) ? 1 : 0;
  ret_val += is_mine(i - 1, j) ? 1 : 0;
  ret_val += is_mine(i - 1, j + 1) ? 1 : 0;
  ret_val += is_mine(i, j - 1) ? 1 : 0;
  ret_val += is_mine(i, j + 1) ? 1 : 0;
  ret_val += is_mine(i + 1, j - 1) ? 1 : 0;
  ret_val += is_mine(i + 1, j) ? 1 : 0;
  ret_val += is_mine(i + 1, j + 1) ? 1 : 0;

  return ret_val;
}

Board::Board(size_t _h, size_t _w, size_t _num_mines)
  : matrix(_h, vector<Box>(_w, Box(StatusValue::COVERED, false))),
    num_mines(_num_mines), num_flags(0), num_uncovered_boxes(0)
  {
    // Empty
  }

Board::Board(const Board & mat)
  : matrix(mat.matrix), num_mines(mat.num_mines),
    num_flags(mat.num_flags),
    num_uncovered_boxes(mat.num_uncovered_boxes)
{
  // Empty
}

Board::Board(Board && mat)
  : matrix(std::move(mat.matrix)), num_mines(0), num_flags(0),
    num_uncovered_boxes(0)
{
  std::swap(num_mines, mat.num_mines);
  std::swap(num_flags, mat.num_flags);
  std::swap(num_uncovered_boxes, mat.num_uncovered_boxes);
}

using Pos = pair<size_t, size_t>;

Pos extract_random_pos(set<Pos> & s, mt19937 & rng)
{
  uniform_int_distribution<int> unif(0, s.size() - 1);
  int r = unif(rng);

  auto it = s.begin();

  while (r-- > 0)
    ++it;

  Pos ret = *it;
  s.erase(ret);
  return ret;
}

void Board::generate_random(mt19937 & rng)
{
  set<Pos> pos_set;
  
  for (size_t i = 0; i < rows(); ++i)
    for(size_t j = 0; j < cols(); ++j)
      pos_set.insert(make_pair(i, j));
      
  for (size_t k = 0; k < num_mines; ++k)
    {
      Pos p = extract_random_pos(pos_set, rng);
      matrix[p.first][p.second].second = true;
    }
}


size_t Board::rows() const
{
  return matrix.size();
}

size_t Board::cols() const
{
  return matrix[0].size();
}

size_t Board::get_num_flags() const
{
  return num_flags;
}
 
size_t Board::get_num_mines() const
{
  return num_mines;
}

size_t Board::get_num_uncovered_boxes() const
{
  return num_uncovered_boxes;
}

bool Board::is_uncovered_mine(size_t i, size_t j) const
{
  const Box & b = matrix[i][j];
  return get<0>(b) == StatusValue::UNCOVERED and get<1>(b);
}

size_t Board::get_num_mines_around(size_t i, size_t j) const
{
  try_pos(i, j);
  
  if (matrix[i][j].first != StatusValue::UNCOVERED)
    return UNKNOWN_VALUE;

  return count_mines_around(i, j);
}

StatusValue Board::get_status(size_t i, size_t j) const
{
  try_pos(i, j);

  return matrix[i][j].first;
}

bool Board::flag(size_t i, size_t j)
{
  try_pos(i, j);
  
  StatusValue & s = matrix[i][j].first;

  if (s == StatusValue::UNCOVERED)
    return false;

  if (s == StatusValue::FLAG)
    {
      s = StatusValue::COVERED;
      --num_flags;
    }
  else
    {
      if (num_flags == num_mines)
	return false;

      s = StatusValue::FLAG;
      ++num_flags;
    }

  return true;
}

void Board::discover(size_t _i, size_t _j)
{
  stack<Pos> st;
  st.push(make_pair(_i, _j));

  while (not st.empty())
    {
      Pos p = st.top();
      st.pop();

      size_t i = p.first;
      size_t j = p.second;
      
      if (i >= rows() or j >= cols())
	continue;

      StatusValue & s = matrix[i][j].first;  
  
      if (s == StatusValue::UNCOVERED or s == StatusValue::FLAG)
	continue;
      
      s = StatusValue::UNCOVERED;
      ++num_uncovered_boxes;
      
      if (get<1>(matrix[i][j]))
	return;
      
      if (count_mines_around(i, j) > 0)
	continue;
      
      st.push(make_pair(i - 1, j - 1));
      st.push(make_pair(i - 1, j));
      st.push(make_pair(i - 1, j + 1));
      st.push(make_pair(i, j + 1));
      st.push(make_pair(i + 1, j + 1));
      st.push(make_pair(i + 1, j));
      st.push(make_pair(i + 1, j - 1));
      st.push(make_pair(i, j - 1));
    }
}

 
bool Board::are_uncovered_all() const
{
  return num_uncovered_boxes == cols() * rows() - num_mines;
}
 
HintType Board::hint(size_t i, size_t j) const
{
  try_pos(i, j);

  HintType ret_val;

  get<0>(ret_val) = matrix[i][j].second;
  get<1>(ret_val) = count_mines_around(i, j);

  return ret_val;
}

Board & Board::operator = (const Board & b)
{
  if (&b == this)
    return *this;

  matrix = b.matrix;
  num_mines = b.num_mines;
  num_flags = b.num_flags;
  num_uncovered_boxes = b.num_uncovered_boxes;

  return *this;
}

Board & Board::operator = (Board && b)
{
  std::swap(matrix, b.matrix);
  std::swap(num_mines, b.num_mines);
  std::swap(num_flags, b.num_flags);
  std::swap(num_uncovered_boxes, b.num_uncovered_boxes);

  return *this;
}

void Board::try_pos(size_t i, size_t j) const
{
  if (i < rows() and j < cols())
    return;
  
  std::stringstream sstr;
  sstr << "Invalid position: (" << i << ", " << j << ")";
  throw std::overflow_error(sstr.str());
}

void Board::save(ofstream & out)
{
  for (auto i = 0; i < rows(); ++i)
    {
      for (auto j = 0; j < cols(); ++j)
	out << int(matrix[i][j].first) << ' '
	    << int(matrix[i][j].second) << ' ';
      out << endl;
    }
}

void Board::load(ifstream & in)
{
  int s, m;
  for (auto i = 0; i < rows(); ++i)
    for (auto j = 0; j < cols(); ++j)
      {
	in >> s >> m;
	matrix[i][j].first = StatusValue(s);
	matrix[i][j].second = bool(m);
      }
}

/**
   Write all the expected info. The curly braces are to represent variables.
   You should remove them when writing your values.
   
   @id {your document id number} 
   @author {your name and your family name}
   @email {your email}
*/

list<Position> get_covered_positions_around(const Board &, const Position &)
{
  return list<Position>();
}

list<Position> get_not_uncovered_positions_around(const Board &,
						  const Position &)
{
  return list<Position>();
}

list<Position> get_util_uncovered(const Board &)
{
  return list<Position>();
}

pair<bool, Position> get_first(const Board &, size_t)
{
  return make_pair(false, Position());
}

list<Position> get_covered_positions(const Board &)
{
  return list<Position>();
}

pair<bool, Position> select_random_covered(const Board &, mt19937 &)
{
  return make_pair(false, Position());
}

pair<bool, Position> get_a_position(const Board &, mt19937 &)
{
  return make_pair(false, Position());
}

list<Position> get_for_flags(const Board &)
{
  return list<Position>();
}
  
list<Position> get_for_discover(const Board &)
{
  return list<Position>();
}
  
void solve(Board &, mt19937 &)
{

}

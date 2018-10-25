/** This file contains the definition of a mines board.
    
    @author Alejandro J. Mujica
*/

# ifndef BOARD_H
# define BOARD_H

# include <fstream>
# include <random>
# include <vector>

enum class StatusValue
{
  UNCOVERED,
  COVERED,
  FLAG,
  NUM_STATUS_VALUES
};

/* La primera posición indica el estado de la celda (Uncovered, Covered, Flag).
   La segunda si contiene mina o no.
*/
using Box = std::pair<StatusValue, bool>;

/* Tipo que representa una pista. El primer elemento indica si la casilla
   contiene o no una mina. El segundo indica la cantidad de minas que tiene la
   casilla alrededor.
*/
using HintType = std::pair<bool, size_t>;

class Board
{
public:
  static constexpr size_t UNKNOWN_VALUE = 9;
  
private:
  std::vector<std::vector<Box>> matrix;

  size_t num_mines;
  size_t num_flags;
  size_t num_uncovered_boxes;

  bool is_mine(size_t, size_t) const;

  size_t count_mines_around(size_t, size_t) const;

public:
  Board(size_t, size_t, size_t);

  Board(const Board &);

  Board(Board &&);

  void generate_random(std::mt19937 &);

  size_t rows() const;

  size_t cols() const;

  size_t get_num_flags() const;

  size_t get_num_mines() const;

  size_t get_num_uncovered_boxes() const;

  /* Retorna true si una casilla destapada es una mina. false en cualquier otro
     caso (no es mina o no está destapada).
  */
  bool is_uncovered_mine(size_t i, size_t j) const;

  /* Retorna la cantidad de minas que rodean la casilla sólo si está destapada.
     En caso de que esté tapada, entonces retorna UNKNOWN_VALUE.
  */
  size_t get_num_mines_around(size_t, size_t) const;

  // Retorna el valor de estado de la casilla (i, j): UNCOVERED, COVERED, FLAG.
  StatusValue get_status(size_t, size_t) const;

  // Coloca o quita una bandera en la casilla (i, j).
  bool flag(size_t, size_t);

  // Destapa (recursivamente) la casilla (i, j).
  void discover(size_t, size_t);

  // Indica si ya se destaparon todas las casillas que no contienen minas.
  bool are_uncovered_all() const;

  // Revela información de la casilla (i, j).
  HintType hint(size_t, size_t) const;
  
  Board & operator = (const Board &);

  Board & operator = (Board &&);

  void try_pos(size_t, size_t) const;

  void save(std::ofstream &);

  void load(std::ifstream &);
};

# endif // MATRIX_H

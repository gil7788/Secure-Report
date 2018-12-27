#ifndef ___MATRIX___
#define ___MATRIX___

#include <vector>
#include <ostream>

template<class Number>
class Matrix {
/* Description:
 * This class hold matrix
 * */
private:
	std::vector< std::vector<Number> > _data;
public:
	Matrix(int c, int r) {
		_data.resize(c);
		for (unsigned int i = 0; i < _data.size(); ++i)
			_data[i].resize(r);
	}

	Matrix(const Matrix &m) : _data(m._data) {}

	Number &operator()(int c, int r) { return _data[c][r]; }
	const Number &operator()(int c, int r) const { return _data[c][r]; }

	int row() const { return _data.size(); }
	int col() const { return _data[0].size(); }
};

template<class Number>
std::ostream &operator<<(std::ostream &s, const Matrix<Number> &m) {
	for (int i_row = 0; i_row < m.row(); ++i_row) {
		for (int i_col = 0; i_col < m.col(); ++i_col) {
			s << m(i_col, i_row) << " ";
		}
		s << std::endl;
	}
	return s;
}

#endif

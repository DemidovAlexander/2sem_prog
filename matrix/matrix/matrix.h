#pragma once
#include <cstdlib>
#include <utility>
#include <map>

typedef std::pair<size_t, size_t> Position;

template <typename ElementsType, size_t numRows, size_t numColumns>
class Matrix {
private:
	std::map<Position, ElementsType> matrix;
public:
	Matrix() : matrix()
	{
	}
	Matrix(const Matrix &other) {
		matrix = other.matrix;
	}
	Matrix &operator=(const Matrix &other) {
		if (this != &other) {
			matrix = other.matrix;
        }
        return *this;
    }
	void Swap(Matrix &other) {
		std::swap(matrix, other.matrix);
    }
	void ReadMatrix(std::istream &stream) {
		for (size_t i = 0; i < numRows; ++i)
			for (size_t j = 0; j < numColumns; ++j) {
				ElementsType element;
				stream >> element;
				if (element != 0) {
					Position position(i, j);
					matrix[position] = element;
				}
			}
	}

//iterators
private:
	template <typename ElementsType, typename IteratorType>
	class BasicIterator : public std::iterator<std::bidirectional_iterator_tag, ElementsType> {
	private:
		IteratorType curElement;
		explicit BasicIterator(IteratorType curElement_) {
			curElement = curElement_;
	}
	public:	
		BasicIterator() : 
			curElement(NULL)
		{
		}
		template<typename OtherElementsType, typename OtherIteratorType>
		BasicIterator(const BasicIterator<OtherElementsType, OtherIteratorType> &other) :
			curElement(other.curElement)
		{
		}
	//elements access
		size_t GetRow() {
			return (curElement->first).first;
		}
		size_t GetColumn() {
			return (curElement->first).second;
		}
		Position GetCoordinates() {
			return curElement->first;
		}
		ElementsType &operator*() const {
			return curElement->second;
		}
		ElementsType *operator->() const {
			return curElement->second;
		}
	//increments
		BasicIterator &operator++() {
			++curElement;
			return *this;
		}
		BasicIterator operator++(int) {
			BasicIterator result = *this;
			++curElement;
			return result;
		}
	//decrements
		BasicIterator &operator--() {
			--curElement;
			return *this;
		}
		BasicIterator operator--(int) {
			BasicIterator result = *this;
			--curElement;
			return result;
		}
	//comparison
		template <typename OtherElementsType, typename OtherIteratorType>
		bool operator==(const BasicIterator<OtherElementsType, OtherIteratorType> &other) const {
			return curElement == other.curElement;
		}
		template <typename OtherElementsType, typename OtherIteratorType>
		bool operator!=(const BasicIterator<OtherElementsType, OtherIteratorType> &other) const {
			return curElement != other.curElement;
		}		

		friend class Matrix;
		friend class BasicIterator<const ElementsType, IteratorType>;
	};

public:
	typedef BasicIterator<typename ElementsType, typename std::map<Position, ElementsType>::iterator> Iterator;
	typedef BasicIterator<typename const ElementsType, typename std::map<Position, ElementsType>::const_iterator> ConstIterator;
	Iterator Begin() {
		return Iterator(matrix.begin());
	}
	Iterator End() {
		return Iterator(matrix.end());
    }
	ConstIterator Begin() const {
		return ConstIterator(matrix.cbegin());
	}
	ConstIterator End() const {
		return ConstIterator(matrix.cend());
	}

//size and access with index
public:
	ElementsType &operator() (size_t x, size_t y) {
		Position position(x, y);
		return matrix[position];
    }
	const ElementsType operator()(size_t x, size_t y) const {
		Position position(x, y);
		if (matrix.find(position) == matrix.end()) {
			return 0;
		} else {
			return (matrix.find(position))->second;
		}
	}

//comparison
public:
	bool operator<(const Matrix &right) const{
		Matrix::ConstIterator firstIterator = Begin();
		Matrix::ConstIterator secondIterator = right.Begin();
		Position nullPosition(0, 0);
		if ((firstIterator.GetCoordinates() > nullPosition) && (secondIterator.GetCoordinates() > nullPosition)) {
			return false;
		}
		while ((firstIterator != End()) || (secondIterator != right.End())) {
			if (firstIterator.GetCoordinates() < secondIterator.GetCoordinates()) {
				if (*firstIterator >= 0) {
					return false;
				}
				++firstIterator;
				continue;
			}
			if (secondIterator.GetCoordinates() < firstIterator.GetCoordinates()) {
				if (*secondIterator <= 0) {
					return false;
				}
				++secondIterator;
				continue;
			}
			if (*secondIterator <= *firstIterator) {
				return false;
			}
			++firstIterator;
		    ++secondIterator;
		}
		return true;
	}
	bool operator==(const Matrix &right) const {
		Matrix::ConstIterator firstIterator = Begin();
		Matrix::ConstIterator secondIterator = right.Begin();
		while ((firstIterator != End()) && (secondIterator != right.End()) && (firstIterator.GetCoordinates() == secondIterator.GetCoordinates())
				&& (*firstIterator == *secondIterator)) {
					++firstIterator;
					++secondIterator;
		}
		if ((firstIterator == End()) && (secondIterator == right.End())) {
			return true;
		} else {
			return false;
		}		
	}
	bool operator!=(const Matrix &right) const {
		return !((*this) == right);
	}
	bool operator<=(const Matrix &right) const {
		return (((*this) == right) || ((*this) < right));
	}
	bool operator>(const Matrix &right) const {
		return (right < (*this));
	}
	bool operator>=(const Matrix &right) const {
		return (((*this) == right) || ((*this) > right));
	}
	
//arithmetic
public:
	Matrix operator*(const ElementsType factor) const {
		if (factor == 0) {
			Matrix result;
			return result;
		}
		Matrix result(*this);
		for(typename Matrix::Iterator iterator = result.Begin(),
			end = result.End(); iterator != end; ++iterator)
		{
			*iterator *= factor;
		}
		return result;
	}
    Matrix &operator*=(const ElementsType &factor) {
		if (factor == 0) {
			Matrix result;
			(*this) = result;
			return (*this);
		}
		for(Iterator iterator = Begin(),
			end = End(); iterator != end; ++iterator) 
		{
			*iterator *= factor;
		}
		return *this;
	}
	Matrix operator-() const {
		return (*this) * (-1); 
    }
	Matrix operator+(const Matrix &other) const {
		Matrix result(*this);
		for(typename Matrix::ConstIterator iterator = other.Begin(),
			end = other.End(); iterator != end; ++iterator) {
				result(iterator.GetRow(), iterator.GetColumn()) = result(iterator.GetRow(), iterator.GetColumn()) + *iterator;
				if (result(iterator.GetRow(), iterator.GetColumn()) == 0) {
					result.matrix.erase(iterator.GetCoordinates());
				}
		}
		return result;
	}
    Matrix &operator+=(const Matrix &other) {
		for (typename Matrix::ConstIterator iterator = other.Begin(),
			end = other.End(); iterator != end; ++iterator) {
				(*this)(iterator.GetRow(), iterator.GetColumn()) = (*this)(iterator.GetRow(), iterator.GetColumn()) + *iterator;
				if ((*this)(iterator.GetRow(), iterator.GetColumn()) == 0) {
					matrix.erase(iterator.GetCoordinates());
				}
		}
        return *this;
    }
	Matrix operator-(const Matrix &other) const {		
        return (*this) + (-other);
    }
    Matrix &operator-=(const Matrix &other) {
		(*this) += -other;
        return *this;
    }
	template<size_t otherColumns>
    Matrix operator*(const Matrix<ElementsType, numColumns, otherColumns> &other) const {
		Matrix<ElementsType, numRows, otherColumns> result;
		for (size_t i = 0; i < numRows; ++i)
			for (size_t j = 0; j < otherColumns; ++j) {
				result(i, j) = 0;
				for (size_t k = 0; k < numColumns; ++k) {
					result(i, j) += (*this)(i, k) * other(k, j);
				}
				if (result(i, j) == 0) {
					Position resultPosition(i, j);
					result.matrix.erase(resultPosition);
				}          
			}
		return result;
	}
    Matrix &operator*=(const Matrix &other) {
		(*this) = (*this) * other;
		return *this;
	}
	void Transpose() {
		for(typename Matrix::ConstIterator iterator = Begin(),
			end = End(); iterator != end; ++iterator) {
				if (iterator.GetRow() < iterator.GetColumn()) {
					Position firstPosition(iterator.GetRow(), iterator.GetColumn());
					Position secondPosition(iterator.GetColumn(), iterator.GetRow());
					std::swap(matrix[firstPosition], matrix[secondPosition]);
					if (matrix[secondPosition] == 0) {
						matrix.erase(secondPosition);
					}
				}
		}
	}
private:
	void SwapRows(size_t first, size_t second) {
		for (size_t i = 0; i < numColumns; ++i) {
			Position firstPosition(first, i);
			Position secondPosition(second, i);
			std::swap(matrix[firstPosition], matrix[secondPosition]);
			if (matrix[firstPosition] == 0) {
				matrix.erase(firstPosition);
			}
			if (matrix[secondPosition] == 0) {
				matrix.erase(secondPosition);
			}
		}
	}
	void AddRowToRow(size_t first, size_t second, ElementsType factor) {
		for (size_t i = 0; i < numColumns; ++i) {
			Position firstPosition(first, i);
			Position secondPosition(second, i);
			matrix[firstPosition] = matrix[firstPosition] + matrix[secondPosition] * factor;
			if (matrix[firstPosition] == 0) {
				matrix.erase(firstPosition);
			}
			if (matrix[secondPosition] == 0) {
				matrix.erase(secondPosition);
			}
		}
	}
	void ChangeRow(size_t row, ElementsType factor) {
		for (size_t i = 0; i < numColumns; ++i) {
			Position position(row, i);
			matrix[position] = factor * matrix[position];
			if (matrix[position] == 0) {
				matrix.erase(position);
			}
		}
	}
public:
	void Inverse() {
		Matrix result;
		for (size_t i = 0; i < numColumns; ++i) {
			result(i, i) = 1;
		}

		for (size_t i = 0; i < numColumns; ++i) {
			Position topPosition(i, i);
			for (size_t j = i + 1; j < numRows; ++j) {
				Position curPosition(j, i);
				if ((matrix[topPosition] == 0) && (matrix[curPosition] != 0)) {
					SwapRows(i, j);
					result.SwapRows(i, j);
				}
				if ((matrix[topPosition] != 0) && (matrix[curPosition] != 0)) {
					ElementsType factor = matrix[curPosition];
					ChangeRow(j, -matrix[topPosition]);
					AddRowToRow(j, i, factor);
					result.ChangeRow(j, -matrix[topPosition]);
					result.AddRowToRow(j, i, factor);
				}
				if (matrix[curPosition] == 0) {
					matrix.erase(curPosition);
				}
			}	
			if (matrix[topPosition] == 0) {
				std::cerr << "Undefined operation";
				exit(1);
			}
		}
		
		for (size_t i = numColumns - 1; i > 0; --i) 
			for (size_t j = i; j > 0; --j) {
				Position curPosition(j - 1, i);
				Position fixedPosition(i, i);
				if ((matrix[curPosition] != 0)) {
					ElementsType factor = matrix[curPosition];
					ChangeRow(j - 1, -matrix[fixedPosition]);
					AddRowToRow(j - 1, i, factor);
					result.ChangeRow(j - 1, -matrix[fixedPosition]);
					result.AddRowToRow(j - 1, i, factor);
				}
				if (matrix[curPosition] == 0) {
					matrix.erase(curPosition);
				}
			}

		for (size_t i = 0; i < numColumns; ++i) {
			Position curPosition(i, i);
			result.ChangeRow(i, 1 / matrix[curPosition]);
		}

		(*this) = result;
	}
	
	friend class Matrix;
};

template<typename ElementsType, size_t numRows, size_t numColumns> 
Matrix<ElementsType, numRows, numColumns> operator*(const ElementsType factor, const Matrix<ElementsType, numRows, numColumns> &matrix) {
	return matrix * factor;
}

template<typename ElementsType, size_t numRows, size_t numColumns> 
std::ostream &operator<<(std::ostream &stream, const Matrix<ElementsType, numRows, numColumns>  &matrix) {
	for (size_t i = 0; i < numRows; ++i) {
		for (size_t j = 0; j < numColumns; ++j) {
			stream << matrix(i, j) << ' ';
		}
		stream << '\n';
	}
	return stream;
}

namespace std {
	template<typename ElementsType, size_t numRows, size_t numColumns> 
	void swap(Matrix<ElementsType, numRows, numColumns> &left, Matrix<ElementsType, numRows, numColumns> &right) {
		left.Swap(right);
	}
}

#include "matrix.h"
#include <iostream>
#include <fstream>

int main() {
	std::ifstream input("input1.txt"); 
	std::ofstream output("output1.txt"); 
	Matrix<double, 3, 3> matrix1;
	matrix1.ReadMatrix(input); 

	Matrix<double, 3, 3> matrix2;
	matrix2.ReadMatrix(input);

	matrix1 = matrix1;
	output << "copy of matrix1:\n" << matrix1 << '\n';
	Matrix<double, 3, 3> matrix3(matrix1);
	output << "copy of matrix1:\n" << matrix3 << '\n';
	matrix3 = matrix2;
	output << "copy of matrix2:\n" << matrix3 << '\n';

	output << "matrix1 after std::swap(matrix1, matrix2):\n";
	std::swap(matrix1, matrix2);
	output << matrix1 << "\nmatrix1 after matrix1.Swap(matrix2):\n";
	matrix1.Swap(matrix2);
	output << matrix1 << '\n';

	output << "Testing iterators:\n\n";

	output << "Nonzero matrix1's elements (using Iterator):\n";
	for (Matrix<double, 3, 3>::Iterator iterator = matrix1.Begin(),
		end = matrix1.End(); iterator != end; ++iterator) {
			output << "row" << iterator.GetRow() << " column" << iterator.GetColumn() << ": ";
			output << *iterator << '\n';
	}
	output << "\nNonzero matrix1's elements (using ConstIterator):\n";
	for (Matrix<double, 3, 3>::ConstIterator iterator = matrix1.Begin(),
		end = matrix1.End(); iterator != end; ++iterator) {
			output << "row" << iterator.GetRow() << " column" << iterator.GetColumn() << ": ";
			output << *iterator << '\n';
	}

	Matrix<double, 3, 3>::Iterator iterator = matrix1.Begin();
	Matrix<double, 3, 3>::ConstIterator firstConstIterator = matrix1.Begin();
	Matrix<double, 3, 3>::ConstIterator secondConstIterator = iterator;
	Matrix<double, 3, 3>::ConstIterator thirdConstIterator(iterator);

	output << "\nIterator matrix1.Begin() value: " << *iterator;
	output << "\nConstIterator matrix1.Begin() value: " << *firstConstIterator;
	output << "\nIterator matrix1.Begin() -> ConstIterator (using =) value: " << *secondConstIterator;
	output << "\nIterator matrix1.Begin() -> ConstIterator (using copy construtor) value: " << *thirdConstIterator;

	Matrix<double, 3, 3>::Iterator iteratorBegin = matrix1.Begin();
	Matrix<double, 3, 3>::Iterator iteratorEnd = matrix1.End();
	Matrix<double, 3, 3>::ConstIterator constIteratorBegin = matrix1.Begin();
	Matrix<double, 3, 3>::ConstIterator constIteratorEnd = matrix1.End();
	output << "\n\nmatrix1.Begin() == matrix1.End(): " << (iteratorBegin == iteratorEnd) << "\n";
	output << "matrix1.Begin() != matrix1.End(): " << (iteratorBegin != iteratorEnd) << "\n";
	output << "matrix1.Begin() == matrix1.End(): " << (constIteratorBegin == constIteratorEnd) << "\n";
	output << "matrix1.Begin() != matrix1.End(): " << (constIteratorBegin != constIteratorEnd) << "\n";
	output << "matrix1.Begin() == matrix1.Begin(): " << (iteratorBegin == constIteratorBegin) << "\n";
	output << "matrix1.End() != matrix1.End(): " << (iteratorEnd != constIteratorEnd) << "\n";

	output << "\nTesting comparison:\n\n";

	output << "matrix1 < matrix2: " << (matrix1 < matrix2) << "\n";
	output << "matrix1 == matrix2: " << (matrix1 == matrix2) << "\n";
	output << "matrix1 != matrix2: " << (matrix1 != matrix2) << "\n";
	output << "matrix1 <= matrix2: " << (matrix1 <= matrix2) << "\n";
	output << "matrix1 > matrix2: " << (matrix1 > matrix2) << "\n";
	output << "matrix1 >= matrix2: " << (matrix1 >= matrix2) << "\n";

	output << "\nTesting arithmetic:\n\n";

	output << "-matrix1:\n" << -matrix1 << '\n';
	output << "matrix1 + matrix2:\n" << matrix1 + matrix2 << '\n';
	output << "matrix1 - matrix2:\n" << matrix1 - matrix2 << '\n';
	output << "matrix1 * matrix2:\n" << matrix1 * matrix2 << '\n';
	output << "matrix2 * matrix1:\n" << matrix2 * matrix1 << '\n';
	matrix1 += matrix2;
	output << "matrix1 += matrix2:\n" << matrix1 << '\n';
	matrix1 -= matrix2;
	output << "matrix1 -= matrix2:\n" << matrix1 << '\n';
	matrix3 *= matrix1;
	output << "matrix3 *= matrix1:\n" << matrix3 << '\n';

	double factor;
	input >> factor;
	output << "factor: " << factor;

	output << "\n\nmatrix1 * factor:\n" << matrix1 * factor << '\n';
	output << "factor * matrix1:\n" << factor * matrix1 << '\n';
	matrix1 *= factor;
	output << "matrix1 *= factor:\n" << matrix1 << '\n';

	matrix2.Transpose();
	output << "transpose matrix2:\n" << matrix2 << '\n';
	matrix2.Transpose();
	output << "transpose matrix2:\n" << matrix2 << '\n';

	matrix2.Inverse();
	output << "inverse matrix2:\n" << matrix2 << '\n';

	return 0;
}
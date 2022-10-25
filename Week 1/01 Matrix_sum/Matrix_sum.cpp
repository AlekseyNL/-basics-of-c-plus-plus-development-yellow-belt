/ Matrix_sum

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;


class Matrix
{
private:
	vector <vector<int> > m_matrix;
	int m_num_rows = 0;
	int m_num_cols = 0;

public:

	//	Конструктор по умолчанию, создаёт матрицу с нулём строк и нулём столбцов.
	Matrix()
	{
		m_matrix.clear();
	}

	/*	Метод Reset, принимает два целочисленных параметра, которые задают 
		новое количество строк и столбцов матрицы; метод Reset меняет размеры
		матрицы на заданные и обнуляет все её элементы. 
	*/
	void Reset(int num_rows, int num_cols)
	{
		if (num_rows < 0 || num_cols < 0)
		{
			throw out_of_range("Error range");
		}

		m_matrix.clear();
		m_num_rows = num_rows;
		m_num_cols = num_cols;
		m_matrix.resize(m_num_rows, vector<int>(m_num_cols));
	}
	
	//	Конструктор от двух целочисленных параметров: num_rows и num_cols,
	//	— которые задают количество строк и столбцов матрицы. 
	Matrix(const int num_rows, const int num_cols) 
	{
		Reset(num_rows, num_cols);
	}

	/*	Константный метод At, который принимает номер строки и номер столбца 
		и возвращает значение в соответствущей ячейке матрицы. 
	*/
	int At(const int row, const int col) const
	{
		return m_matrix.at(row).at(col);
	}

	/*	Неконстантный метод At с такими же параметрами, но возвращающий ссылку 
		на значение в соответствущей ячейке матрицы.
 	*/
	int& At(const int row, const int col)
	{
		return m_matrix.at(row).at(col);
	}

	/*	Константные методы GetNumRows и GetNumColumns, которые возвращают 
		количество строк и столбцов матрицы соответственно.
  	*/
	int GetNumRows() const
	{
		return m_num_rows;
	}
	int GetNumColumns() const
	{
		return m_num_cols;
	}

	/*	Оператор ввода. Cначала задаётся количество строк и столбцов
		(именно в этом порядке), а затем все элементы матрицы: сначала элемент
		первой строки и первого столбца, затем элемент первой строки
		и второго столбца и так далее
	*/
	friend istream& operator>> (istream& in, Matrix &any);

	/*	Оператор вывода. Выводит матрицу в формате: в первой строке
      		количество строк и столбцов, во второй — элементы первой строки, 
		в третьей — элементы второй строки и т.д 
	*/
	friend ostream& operator<< (ostream& out, const Matrix &any);

	/*	Оператор проверки на равенство (==). Возвращаeт true, 
		если сравниваемые матрицы имеют одинаковый размер и все их 
		соответствующие элементы равны между собой, иначе false.
	*/
	friend bool operator== (const Matrix& one, const Matrix& two);

	/*	Оператор сложения. Принимает две матрицы и возвращает их сумму.
		Если переданные матрицы имеют разные размеры этот оператор
		выбрасывает стандартное исключение invalid_argument. 
	*/
	friend Matrix operator+ (const Matrix& one, const Matrix& two);
};


istream& operator>> (istream& in, Matrix &any)
{
	int num_rows;
	int num_cols;
	in >> num_rows;
	in >> num_cols;	
	any.Reset(num_rows, num_cols);

	for (int i = 0; i < num_rows; i++)
	{
		for (int j = 0; j < num_cols; j++)
		{
			in >> any.At(i, j);
		}
	}
	return in;
}

ostream& operator<< (ostream& out, const Matrix& any)
{
	out << endl;
	out << any.m_num_rows << " " << any.m_num_cols << endl;

	for (int i = 0; i < any.m_num_rows; i++)
	{
		for (int j = 0; j < any.m_num_cols; j++)
		{
			if (j != (any.m_num_cols - 1))
			{
				out << any.At(i, j) << " ";
			}
			else
			{
				out << any.At(i, j);
			}
		}
		out << endl; 
	}
	return out;
}

bool operator== (const Matrix& one, const Matrix& two)
{
	if (one.m_num_rows != two.m_num_rows || one.m_num_cols != two.m_num_cols)
	{
		return false;
	}
	for (int i = 0; i < one.m_num_rows; i++)
	{
		for (int j = 0; j < one.m_num_cols; j++)
		{
			if (one.At(i, j) != two.At(i, j))
			{
				return false;
			}
		}
	}
	return true;
}

Matrix operator+ (const Matrix& one, const Matrix& two)
{
	if (one.m_num_rows != two.m_num_rows || one.m_num_cols != two.m_num_cols)
	{
		throw invalid_argument("invalid argumen");
	}

	Matrix temp(one.m_num_rows, one.m_num_cols);

	for (int i = 0; i < one.m_num_rows; i++)
	{
		for (int j = 0; j < one.m_num_cols; j++)
		{
			temp.At(i, j) = one.At(i, j) + two.At(i, j);
		}
	}
	return temp;
}

int main() {
	setlocale(LC_ALL, "");

	Matrix one;
	Matrix two;

	cin >> one >> two;
	cout << one+two << endl;

	
	return 0;
}

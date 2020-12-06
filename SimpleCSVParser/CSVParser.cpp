#include<iostream>
#include<vector>
#include<string>
#include<fstream>

class CSVColumn {
	std::string data;
public:
	CSVColumn(const char* v_data) {
		this->data = v_data;
	}
	

	std::string getData() {
		return data;
	}

	friend std::ostream& operator<<(std::ostream& out, CSVColumn& column);
};

std::ostream& operator<<(std::ostream& out, CSVColumn& column) {
	out << column.data;
	return out;
}

class CSVLine {
	std::vector<CSVColumn*> columns;
public:
	CSVLine() {}

	void addColumn(CSVColumn* column) {
		this->columns.push_back(column);
	}

	CSVColumn& operator[](unsigned index) {
		if (index < this->columns.size()) {
			return *(this->columns[index]);
		}
	}

	friend std::ostream& operator<<(std::ostream& out, CSVLine& line);
};

std::ostream& operator<<(std::ostream& out, CSVLine& line) {
	int size = line.columns.size();
	for (int i = 0; i < size; i++) {
		out << *(line.columns[i]);

		if (i < size - 1) {
			out << ',';
		}
	}
	return out;
}

class CSVFile {
	std::vector<CSVLine*> lines;
public:
	CSVFile() {}
	void addLine(CSVLine* line) {
		this->lines.push_back(line);
	}

	CSVLine& operator[](unsigned index) {
		if (index < this->lines.size()) {
			return *(this->lines[index]);
		}
	}

	friend std::ostream& operator<<(std::ostream& out, CSVFile& file);

	void load(const char* file_path) {
		std::ifstream file;
		file.open(file_path, std::ifstream::in);

		char column[10000];
		bool quote = false;
		int j = 0;

		CSVLine* l = new CSVLine();

		char c = file.get();

		while (file.good()) {
			if (c == '"') {
				quote = !quote;
				c = file.get();
				continue;
			}

			if (c == '\n') {
				column[j] = '\0';
				j = 0;

				l->addColumn(new CSVColumn(column));
				this->addLine(l);

				l = new CSVLine();
				c = file.get();
				continue;
			}

			if (c == ',' && quote == false) {
				column[j] = '\0';
				j = 0;
				l->addColumn(new CSVColumn(column));
			}
			else {
				column[j] = c;
				j++;
			}
			c = file.get();
		}

		file.close();
	}
};

std::ostream& operator<<(std::ostream& out, CSVFile& file) {
	int size = file.lines.size();
	for (int i = 0; i < size; i++) {
		out << *(file.lines[i]) << std::endl;
	}
	return out;
}



int main() {

	CSVFile csv;
	csv.load("feed_componente_pc_test.csv");

	std::cout << csv;
	std::cout << csv[1][1];

	std::cout << csv[1][0].getData();
	return 0;
}
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Core {
public:
  Core(): midterm(0), final(0) { }
  Core(std::istream& is) { read(is); }

  std::string name() const;
  double grade() const;
  std::istream& read(std::istream&);
protected:
  std::istream& read_common(std::istream&);
  double midterm, final;
  std::vector<double> homework;
private:
  std::string n;
};


class Grad: public Core {
public:
  Grad(): thesis(0) { }
  Grad(std::istream& is) { read(is); }

  double grade() const;
  std::istream& read(std::istream&);
private:
  double thesis;
};


string Core::name() const { return n; }

double grade(double midterm, double final, double homework) {
  return 0.2 * midterm + 0.4 * final + 0.4 * homework;
}

double median(vector<double> vec) {
  typedef vector<double>::size_type vec_sz;
  vec_sz size = vec.size();
  if (size == 0) 
    throw domain_error("median of an empty vector");
  sort(vec.begin(), vec.end());
  vec_sz mid = size / 2;
  return size % 2 == 0 ? (vec[mid] + vec[mid-1]) / 2 : vec[mid];
}

double grade(double midterm, double final, const vector<double>& hw) {
  if (hw.size() == 0)
    throw domain_error("Student has done no homework");
  return grade(midterm, final, median(hw));
}

double Core::grade() const {
  return ::grade(midterm, final, homework);
}

istream& Core::read_common(istream& in) {
  in >> n >> midterm >> final; 
  return in;
}

istream& read_hw(istream& in, vector<double>& hw) {
  if (in) {
    hw.clear();
    double x;
    while (in >> x) 
      hw.push_back(x);
    in.clear();
  }
  return in;
}

istream& Core::read(istream& in) {
  read_common(in);
  read_hw(in, homework);
  return in;
}

istream& Grad::read(istream& in) {
  read_common(in);
  in >> thesis;
  read_hw(in, homework);
  return in;
}

double Grad::grade() const {
  return min(Core::grade(), thesis);
}


int main() {
  return 0;
}
















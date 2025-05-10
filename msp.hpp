#ifndef __MSP_H__
#define __MSP_H__

#include "pbc/PBC.h"
#include <bits/stdc++.h>

using namespace std;

struct MSP {
  const Pairing& e;
  const int party_count;

  MSP(const Pairing& e, const int& party_count)
    : e(e), party_count(party_count) { }

  virtual pair<int, int> size() const = 0;
  virtual int label(const int& i) const = 0;

  virtual vector<Zr> dot(const vector<Zr>& col) const = 0;
  virtual vector<Zr> solve(const vector<bool>& active) const = 0;
};

struct MatrixMSP : MSP {
  const vector<int> labels;
  const vector<vector<Zr>> matrix;
  const vector<vector<Zr>> solutions;

  MatrixMSP(const Pairing& e, const int& party_count, const vector<int>& labels, const vector<vector<int>>& matrix, const vector<vector<int>>& solutions)
    : MSP(e, party_count), labels(labels), matrix(make_zr(e, matrix)), solutions(make_zr(e, solutions)) { }

  pair<int, int> size() const {
    return make_pair(matrix.size(), matrix[0].size());
  }

  int label(const int& i) const {
    return labels[i];
  }

  vector<Zr> dot(const vector<Zr>& col) const {
    const auto [rows, cols] = size();
    vector<Zr> result(rows, Zr(e, 0L));
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        result[i] += matrix[i][j] * col[j];
      }
    }
    return result;
  }

  vector<Zr> solve(const vector<bool>& active) const {
    const auto [rows, _cols] = size();
    for (const auto& solution : solutions) {
      bool ok = true;
      for (int i = 0; i < rows; i++) {
        if (solution[i] != Zr(e, 0L) && !active[i]) {
          ok = false;
          break;
        }
      }
      if (ok) {
        return solution;
      }
    }
    return vector<Zr>();
  }

private:
  static vector<vector<Zr>> make_zr(const Pairing& e, const vector<vector<int>>& matrix) {
    vector<vector<Zr>> zr_matrix;
    zr_matrix.reserve(matrix.size());
    for (const auto& row : matrix) {
      zr_matrix.emplace_back();
      zr_matrix.back().reserve(row.size());
      for (const auto& value : row) {
        zr_matrix.back().emplace_back(e, long(value));
      }
    }
    return zr_matrix;
  }
};

struct OrMSP : MSP {
  OrMSP(const Pairing& e, const int& party_count)
    : MSP(e, party_count) { }

  pair<int, int> size() const {
    return make_pair(party_count, party_count);
  }

  int label(const int& i) const {
    return i;
  }

  vector<Zr> dot(const vector<Zr>& col) const {
    const auto [rows, cols] = size();
    Zr sum(e, 0L);
    for (int j = 0; j < cols; j++) {
      sum += col[j];
    }
    return vector(rows, sum);
  }

  vector<Zr> solve(const vector<bool>& active) const {
    const auto [rows, _cols] = size();
    for (int i = 0; i < rows; i++) {
      if (active[i]) {
        vector<Zr> solution(rows, Zr(e, 0L));
        solution[i] = Zr(e, 1L);
        return solution;
      }
    }
    return vector<Zr>();
  }
};

struct AndMSP : MSP {
  AndMSP(const Pairing& e, const int& party_count)
    : MSP(e, party_count) { }

  pair<int, int> size() const {
    return make_pair(party_count, party_count);
  }

  int label(const int& i) const {
    return i;
  }

  vector<Zr> dot(const vector<Zr>& col) const {
    return col;
  }

  vector<Zr> solve(const vector<bool>& active) const {
    const auto [rows, _cols] = size();
    for (int i = 0; i < rows; i++) {
      if (!active[i]) {
        return vector<Zr>();
      }
    }
    return vector<Zr>(rows, Zr(e, 1L));
  }
};

struct ThresholdMSP : MSP {
  const int threshold;

  ThresholdMSP(const Pairing& e, const int& party_count, const int& threshold)
    : MSP(e, party_count), threshold(threshold) { }

  pair<int, int> size() const {
    return make_pair(party_count, threshold);
  }

  int label(const int& i) const {
    return i;
  }

  vector<Zr> dot(const vector<Zr>& col) const {
    const auto [rows, cols] = size();
    vector<Zr> result(rows, Zr(e, 0L));
    for (int i = 0; i < rows; i++) {
      Zr power(e, 1L);
      for (int j = 0; j < cols; j++) {
        result[i] += power * col[j];
        power *= Zr(e, long(i + 1));
      }
    }
    return result;
  }

  vector<Zr> solve(const vector<bool>& active) const {
    const auto [rows, cols] = size();
    vector<int> active_rows;
    for (int i = 0; i < rows; i++) {
      if (active[i]) {
        active_rows.push_back(i);
        if (int(active_rows.size()) == cols) {
          break;
        }
      }
    }
    if (int(active_rows.size()) < cols) {
      return vector<Zr>();
    }
    ifstream pairing_in("pairing.param");
    string p;
    pairing_in >> p >> p >> p >> p >> p >> p >> p >> p;
    pairing_in.close();
    ofstream temp_out("temp.txt");
    temp_out << cols << ' ' << p << '\n';
    for (const auto& i : active_rows) {
      long power = 1;
      for (int j = 0; j < cols; j++) {
        temp_out << power << " \n"[j == cols - 1];
        power *= i + 1;
      }
    }
    temp_out.close();
    system("python threshold.py");
    ifstream temp_in("temp.txt");
    vector<Zr> solution(rows, Zr(e, 0L));
    for (const auto& i : active_rows) {
      long value;
      temp_in >> value;
      solution[i] = Zr(e, value);
    }
    temp_in.close();
    system("rm temp.txt");
    return solution;
  }
};

#endif

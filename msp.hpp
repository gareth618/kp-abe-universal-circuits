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

  virtual vector<Zr> dot(const vector<Zr>& row) const = 0;
  virtual vector<Zr> solve(const vector<bool>& active) const = 0;
};

struct MatrixMSP : MSP {
  const vector<int> labels;
  const vector<vector<Zr>> matrix;

  MatrixMSP(const Pairing& e, const int& party_count, const vector<int>& labels, const vector<vector<int>>& matrix)
    : MSP(e, party_count), labels(labels), matrix(make_matrix(e, matrix)) { }

  pair<int, int> size() const {
    return make_pair(matrix.size(), matrix[0].size());
  }

  int label(const int& i) const {
    return labels[i];
  }

  vector<Zr> dot(const vector<Zr>& row) const {
    const auto [rows, cols] = size();
    vector<Zr> result(rows, Zr(e, 0L));
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        result[i] += matrix[i][j] * row[j];
      }
    }
    return result;
  }

private:
  static vector<vector<Zr>> make_matrix(const Pairing& e, const vector<vector<int>>& matrix) {
    const auto [rows, cols] = make_pair(int(matrix.size()), int(matrix[0].size()));
    vector zr_matrix(rows, vector<Zr>(cols));
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        zr_matrix[i][j] = Zr(e, long(matrix[i][j]));
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

  vector<Zr> dot(const vector<Zr>& row) const {
    const auto [rows, cols] = size();
    Zr sum(e, 0L);
    for (int j = 0; j < cols; j++) {
      sum += row[j];
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

  vector<Zr> dot(const vector<Zr>& row) const {
    return row;
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

#endif

#ifndef __MSP_H__
#define __MSP_H__

#include "pbc/PBC.h"
#include <bits/stdc++.h>

using namespace std;

struct MSP {
  const int party_count;

  MSP(const int& party_count) : party_count(party_count) { }

  virtual pair<int, int> size() const = 0;
  virtual int label(const int& i) const = 0;
  virtual long at(const int& i, const int& j) const = 0;
  virtual Zr dot(const Pairing& e, const int& i, const vector<Zr>& row) const = 0;
};

struct MatrixMSP : MSP {
  const vector<int> labels;
  const vector<vector<long>> matrix;

  MatrixMSP(const int& party_count, const vector<int>& labels, const vector<vector<long>>& matrix) : MSP(party_count), labels(labels), matrix(matrix) { }

  pair<int, int> size() const {
    return make_pair(matrix.size(), matrix[0].size());
  }

  int label(const int& i) const {
    return labels[i];
  }

  long at(const int& i, const int& j) const {
    return matrix[i][j];
  }

  Zr dot(const Pairing& e, const int& i, const vector<Zr>& row) const {
    Zr sum(e, 0L);
    for (int j = 0; j < size().second; j++) {
      sum += Zr(e, matrix[i][j]) * row[j];
    }
    return sum;
  }
};

struct OrMSP : MSP {
  OrMSP(const int& party_count) : MSP(party_count) { }

  pair<int, int> size() const {
    return make_pair(party_count, party_count);
  }

  int label(const int& i) const {
    return i;
  }

  long at(const int& i, const int& j) const {
    return 1;
  }

  Zr dot(const Pairing& e, const int& i, const vector<Zr>& row) const {
    Zr sum(e, 0L);
    for (int j = 0; j < size().second; j++) {
      sum += row[j];
    }
    return sum;
  }
};

struct AndMSP : MSP {
  AndMSP(const int& party_count) : MSP(party_count) { }

  pair<int, int> size() const {
    return make_pair(party_count, party_count);
  }

  int label(const int& i) const {
    return i;
  }

  long at(const int& i, const int& j) const {
    return i == j ? 1 : 0;
  }

  Zr dot(const Pairing& e, const int& i, const vector<Zr>& row) const {
    return row[i];
  }
};

#endif

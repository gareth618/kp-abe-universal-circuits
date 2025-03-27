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
  virtual int at(const int& i, const int& j) const = 0;
};

struct MatrixMSP : MSP {
  const vector<int> labels;
  const vector<vector<int>> matrix;

  MatrixMSP(const int& party_count, const vector<int>& labels, const vector<vector<int>>& matrix) : MSP(party_count), labels(labels), matrix(matrix) { }

  pair<int, int> size() const {
    return make_pair(matrix.size(), matrix[0].size());
  }

  int label(const int& i) const {
    return labels[i];
  }

  int at(const int& i, const int& j) const {
    return matrix[i][j];
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

  int at(const int& i, const int& j) const {
    return 1;
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

  int at(const int& i, const int& j) const {
    return i == j ? 1 : 0;
  }
};

#endif

#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include "msp.hpp"

struct Node {
  virtual ~Node() = default;
};

struct Gate : Node {
  const shared_ptr<MSP> msp;
  const vector<shared_ptr<Node>> inputs;

  Gate(const shared_ptr<MSP>& msp, const vector<shared_ptr<Node>>& parties)
    : msp(msp), inputs(make_inputs(msp, parties)) { }

  vector<Zr> share(const Pairing& e, const Zr& secret) const {
    const auto [_rows, cols] = msp->size();
    vector<Zr> random(cols);
    Zr sum(e, 0L);
    for (int j = 0; j < cols - 1; j++) {
      random[j] = Zr(e, true);
      sum += random[j];
    }
    random[cols - 1] = secret - sum;
    return msp->dot(random);
  }

  pair<bool, GT> reconstruct(const Pairing& e, const vector<pair<bool, GT>>& results) const {
    const auto [rows, _cols] = msp->size();
    vector<bool> active;
    active.reserve(rows);
    for (int i = 0; i < rows; i++) {
      active.push_back(results[i].first);
    }
    const auto solution = msp->solve(active);
    if (solution.empty()) {
      return make_pair(false, GT(e, true));
    }
    GT result(e, true);
    for (int i = 0; i < rows; i++) {
      result *= results[i].second ^ solution[i];
    }
    return make_pair(true, result);
  }

private:
  static vector<shared_ptr<Node>> make_inputs(const shared_ptr<MSP>& msp, const vector<shared_ptr<Node>>& parties) {
    const auto [rows, _cols] = msp->size();
    vector<shared_ptr<Node>> inputs;
    inputs.reserve(rows);
    for (int i = 0; i < rows; i++) {
      inputs.push_back(parties[msp->label(i)]);
    }
    return inputs;
  }
};

struct Input : Node {
  const int attribute;

  Input(int attribute)
    : attribute(attribute) { }
};

#endif

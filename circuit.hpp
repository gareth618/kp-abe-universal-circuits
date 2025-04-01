#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include "msp.hpp"

struct Node {
  virtual ~Node() = default;
};

struct Gate : Node {
  const shared_ptr<MSP> msp;
  const vector<shared_ptr<Node>> inputs;

  Gate(const shared_ptr<MSP>& msp, const vector<shared_ptr<Node>>& parties) : msp(msp), inputs(make_inputs(parties)) { }

  vector<Zr> share(const Pairing& e, const Zr& secret) const {
    const auto& [rows, cols] = msp->size();
    vector<Zr> random(cols);
    Zr sum(e, 0L);
    for (int j = 0; j < cols - 1; j++) {
      random[j] = Zr(e, true);
    }
    random[cols - 1] = secret - sum;
    vector<Zr> secrets;
    secrets.reserve(rows);
    for (int i = 0; i < rows; i++) {
      secrets.push_back(msp->dot(e, i, random));
    }
    return secrets;
  }

  pair<bool, GT> reconstruct(const Pairing& e, const vector<pair<bool, GT>>& results) const {
  }

private:
  vector<shared_ptr<Node>> make_inputs(const vector<shared_ptr<Node>>& parties) {
    vector<shared_ptr<Node>> inputs;
    inputs.reserve(msp->party_count);
    for (int i = 0; i < msp->size().first; i++) {
      inputs.push_back(parties[msp->label(i)]);
    }
    return inputs;
  }
};

struct Input : Node {
  const int attribute;

  Input(int attribute) : attribute(attribute) { }
};

#endif

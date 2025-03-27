#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include "msp.hpp"

struct Node {
  virtual ~Node() = default;
};

struct Gate : Node {
  const shared_ptr<MSP> msp;
  const vector<shared_ptr<Node>> inputs;

  Gate(const shared_ptr<MSP>& msp, const vector<shared_ptr<Node>>& inputs) : msp(msp), inputs(inputs) { }

  vector<Zr> share(const Pairing& e, const Zr& secret) const;
  pair<bool, GT> reconstruct(const Pairing& e, const vector<pair<bool, GT>>& results) const;
};

struct Input : Node {
  const long attribute;

  Input(long attribute) : attribute(attribute) { }
};

#endif

#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include "pbc/PBC.h"
#include <bits/stdc++.h>

using namespace std;

struct Node {
  virtual ~Node() = default;
};

struct Gate : Node {
  const vector<shared_ptr<Node>> inputs;

  Gate(const vector<shared_ptr<Node>>& inputs) : inputs(inputs) { }

  vector<Zr> share(const Pairing& e, const Zr& secret) const;
  pair<bool, GT> reconstruct(const Pairing& e, const vector<pair<bool, GT>>& results) const;
};

struct Input : Node {
  const long attribute;

  Input(long attribute) : attribute(attribute) { }
};

#endif

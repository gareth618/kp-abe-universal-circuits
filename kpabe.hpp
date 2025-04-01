#ifndef __SCHEME_H__
#define __SCHEME_H__

#include "circuit.hpp"

struct PublicKey {
  const G1 g;
  const GT Y;
  const vector<G1> T;

  PublicKey(const G1& g, const GT& Y, const vector<G1>& T)
    : g(g), Y(Y), T(T) { }
};

struct MasterKey {
  const G1 g;
  const Zr y;
  const vector<Zr> t;

  MasterKey(const G1& g, const Zr& y, const vector<Zr>& t)
    : g(g), y(y), t(t) { }
};

struct CipherText {
  const vector<int> attributes;
  const G1 gs;
  const GT ct;
  const vector<G1> cts;

  CipherText(const vector<int>& attributes, const G1& gs, const GT& ct, const vector<G1>& cts)
    : attributes(attributes), gs(gs), ct(ct), cts(cts) { }
};

struct DecryptionKey {
  const shared_ptr<Gate> circuit;
  const vector<G1> dks;

  DecryptionKey(const shared_ptr<Gate>& circuit, const vector<G1>& dks)
    : circuit(circuit), dks(dks) { }
};

struct KPABE {
  const Pairing& e;
  const int attribute_count;

  KPABE(const Pairing& e, const int& attribute_count)
    : attribute_count(attribute_count), e(e) { }

  pair<PublicKey, MasterKey> setup() const {
    const Zr y(e, true);
    vector<Zr> t;
    t.reserve(attribute_count);
    for (int i = 0; i < attribute_count; i++) {
      t.emplace_back(e, true);
    }
    const G1 g(e, false);
    const GT Y = e(g, g) ^ y;
    vector<G1> T;
    T.reserve(attribute_count);
    for (int i = 0; i < attribute_count; i++) {
      T.push_back(g ^ t[i]);
    }
    return make_pair(PublicKey(g, Y, T), MasterKey(g, y, t));
  }

  CipherText encrypt(const GT& m, const vector<int>& attributes, const PublicKey& pk) const {
    const Zr s(e, true);
    const GT ct = m * (pk.Y ^ s);
    vector<G1> cts;
    cts.reserve(attribute_count);
    for (int i = 0; i < attribute_count; i++) {
      cts.push_back(pk.T[i] ^ s);
    }
    return CipherText(attributes, pk.g ^ s, ct, cts);
  }

  DecryptionKey keygen(const shared_ptr<Gate>& circuit, const MasterKey& mk) const {
    vector<G1> dks;

    const function<void(const shared_ptr<Node>&, const Zr&)> dfs = [&](auto node, auto secret) {
      const auto gate = dynamic_pointer_cast<Gate>(node);
      if (gate) {
        const auto secrets = gate->share(e, secret);
        for (int i = 0; i < int(gate->inputs.size()); i++) {
          dfs(gate->inputs[i], secrets[i]);
        }
      } else {
        const auto input = dynamic_pointer_cast<Input>(node);
        dks.push_back(mk.g ^ secret / mk.t[input->attribute]);
      }
    };

    dfs(circuit, mk.y);
    return DecryptionKey(circuit, dks);
  }

  pair<bool, GT> decrypt(const CipherText& ct, const DecryptionKey& dk) const {
    int i = -1;

    const function<pair<bool, GT>(const shared_ptr<Node>&)> dfs = [&](auto node) {
      const auto gate = dynamic_pointer_cast<Gate>(node);
      if (gate) {
        vector<pair<bool, GT>> results;
        results.reserve(gate->inputs.size());
        for (const auto& input : gate->inputs) {
          results.push_back(dfs(input));
        }
        return gate->reconstruct(e, results);
      } else {
        const auto input = dynamic_pointer_cast<Input>(node);
        i++;
        if (ranges::find(ct.attributes, input->attribute) != ct.attributes.end()) {
          return make_pair(true, e(dk.dks[i], ct.cts[input->attribute]));
        }
        return make_pair(false, GT(e, true));
      }
    };

    const auto result = dfs(dk.circuit);
    if (result.first) {
      return make_pair(true, ct.ct / result.second);
    }
    return result;
  }
};

#endif

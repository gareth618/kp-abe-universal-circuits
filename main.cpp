#include "kpabe.hpp"

int main() {
  const Pairing e(fopen("pairing.param", "r"));
  const KPABE kpabe(e, 6);
  const auto [pk, mk] = kpabe.setup();

  const auto test = [&](const shared_ptr<Gate>& circuit, const set<int>& attributes, const bool& expected_ok) {
    static int i = 0;
    const GT pt(e, false);
    const CipherText ct = kpabe.encrypt(pt, attributes, pk);
    const DecryptionKey dk = kpabe.keygen(circuit, mk);
    const auto& [ok, gt] = kpabe.decrypt(ct, dk);
    if (!ok && !expected_ok || ok && expected_ok && pt == gt) {
      cout << "Test " + to_string(++i) + " passed.\n";
    } else {
      cout << "Test " + to_string(++i) + " failed.\n";
    }
  };

  const shared_ptr<Node> v1_node1 = make_shared<Input>(0);
  const shared_ptr<Node> v1_node2 = make_shared<Input>(1);
  const shared_ptr<Node> v1_node3 = make_shared<Input>(2);
  const shared_ptr<Gate> v1_node4 = make_shared<Gate>(make_shared<AndMSP>(e, 3), vector({v1_node1, v1_node2, v1_node3}));

  test(v1_node4, set({0, 1, 2}), true);
  test(v1_node4, set({0, 2}), false);

  const shared_ptr<Node> v2_node1 = make_shared<Input>(3);
  const shared_ptr<Node> v2_node2 = make_shared<Input>(0);
  const shared_ptr<Node> v2_node3 = make_shared<Input>(5);
  const shared_ptr<Node> v2_node4 = make_shared<Gate>(make_shared<OrMSP>(e, 2), vector({v2_node1, v2_node3}));
  const shared_ptr<Gate> v2_node5 = make_shared<Gate>(make_shared<AndMSP>(e, 3), vector({v2_node1, v2_node2, v2_node4}));

  test(v2_node5, set({3, 0}), true);
  test(v2_node5, set({0, 5}), false);
  test(v2_node5, set({3, 0, 5, 4}), true);
  test(v2_node5, set({3, 5, 1}), false);
  test(v2_node5, set({1, 4}), false);

  const shared_ptr<Node> v3_node1 = make_shared<Input>(0);
  const shared_ptr<Node> v3_node2 = make_shared<Input>(1);
  const shared_ptr<Node> v3_node3 = make_shared<Input>(2);
  const shared_ptr<Node> v3_node4 = make_shared<Input>(3);
  const shared_ptr<Gate> v3_node5 = make_shared<Gate>(make_shared<MatrixMSP>(e, 4, vector({0, 1, 2, 2, 3}), vector({
    vector({0, 0, 0, 0, 1}),
    vector({1, 1, 1, 1, 0}),
    vector({0, 0, 0, 0, 1}),
    vector({0, 0, 0, 1, 0}),
    vector({1, 1, 1, 0, 0}),
  }), vector({
    vector({1, 1, 0, 0, 0}),
    vector({0, 1, 1, 0, 0}),
    vector({0, 0, 1, 1, 1}),
  })), vector({v3_node1, v3_node2, v3_node3, v3_node4}));

  test(v3_node5, set({0, 1}), true);
  test(v3_node5, set({1, 2}), true);
  test(v3_node5, set({2, 3}), true);
  test(v3_node5, set({0, 3, 4}), false);
  test(v3_node5, set({1, 3}), false);
  return 0;
}

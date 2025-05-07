# A Hybrid KP-ABE Scheme for Universal Circuits

A universal circuit is a circuit where each internal node is embedded with a monotone span program computing a specific monotone access structure. Thus, it does not limit us to only using classic gates like `AND`, `OR`, and threshold.

This KP-ABE scheme combines the ideas of Goyal et al. for monotone span programs and of Hu–Gao for monotone Boolean circuits.

This implementation uses the C++ wrapper of the PBC (pairing-based cryptography) library and defers to Python for some linear algebra operations.

```cpp
const shared_ptr<Node> v4_node1 = make_shared<Input>(0);
const shared_ptr<Node> v4_node2 = make_shared<Input>(1);
const shared_ptr<Node> v4_node3 = make_shared<Input>(2);
const shared_ptr<Node> v4_node4 = make_shared<Input>(3);
const shared_ptr<Node> v4_node5 = make_shared<Input>(4);
const shared_ptr<Node> v4_node6 = make_shared<Input>(5);
const shared_ptr<Node> v4_node7 = make_shared<Gate>(make_shared<MatrixMSP>(e, 4, vector({0, 1, 2, 2, 3}), vector({
  vector({0, 0, 0, 0, 1}),
  vector({1, 1, 1, 1, 0}),
  vector({0, 0, 0, 0, 1}),
  vector({0, 0, 0, 1, 0}),
  vector({1, 1, 1, 0, 0}),
}), vector({
  vector({1, 1, 0, 0, 0}),
  vector({0, 1, 1, 0, 0}),
  vector({0, 0, 1, 1, 1}),
})), vector({v4_node1, v4_node2, v4_node3, v4_node4}));
const shared_ptr<Node> v4_node8 = make_shared<Gate>(make_shared<OrMSP>(e, 2), vector({v4_node3, v4_node5}));
const shared_ptr<Gate> v4_node9 = make_shared<Gate>(make_shared<AndMSP>(e, 3), vector({v4_node7, v4_node8, v4_node6}));

test(v4_node9, set({0, 2, 3, 5}), true);
test(v4_node9, set({1, 2, 4}), false);
```

In the above example, the access policy is a universal circuit made up of, respectively, `U`, `OR`, and `AND` gates, the former gate using a custom monotone span program.

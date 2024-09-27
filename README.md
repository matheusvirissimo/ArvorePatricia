# Patricia Tree
Implementation of a Patricia Tree for the Data Structures II course

A Patricia Tree is a compressed version of a Trie data structure that stores strings or binary keys efficiently by collapsing nodes with only one child. It is used for efficient prefix-based search operations and is particularly space-efficient when compared to a regular Trie.

# Key Features:
1. **Compressed Structure**: Patricia Trees compress paths in the Trie where nodes only have a single child, reducing the space required for storing sparse data and improving efficiency.
2. **Efficient Search Operations**: Like a Trie, Patricia Trees allow for efficient prefix-based searches. The search time is proportional to the length of the query, not the number of stored keys.
3. **No Redundant Nodes**: Unlike a standard Trie, Patricia Trees do not store unnecessary intermediate nodes, making them more space-efficient.
4. **Binary and String Storage**: Patricia Trees can be used to store binary keys, which makes them useful in a wide variety of applications, including IP routing and dictionaries.

# Basic Properties:
- **Compressed Paths**: Nodes with only one child are compressed into a single node, reducing depth and improving lookup efficiency.
- **Internal Node Representation**: Only nodes that differentiate between two or more possible outcomes are retained.
- **Flexible Key Storage**: Keys stored in Patricia Trees can be binary sequences or strings, depending on the application.

# Operations:
- **Search**: Efficient prefix-based search by following paths down the tree, making it optimal for longest-prefix match queries.
- **Insertion**: Inserting a new key involves finding the common prefix and creating new nodes where the keys differ.
- **Deletion**: Removes a key and potentially compresses the tree further if nodes no longer diverge.


# mini_redis

A C project exploring a tree-based in-memory data structure and a basic TCP server/client command flow (work in progress).

## Current development

### Repository layout

- `include/tree.h`, `src/tree.c` - tree data structures and operations
- `include/server.h`, `src/server.c` - TCP server scaffolding and command parsing/dispatch (in progress)
- `test_tree.c` - standalone test/demo program for the tree module (root-level)
- `english_words.txt` - sample word data
- `tree` - compiled artifact (if present)

### Tree module (`src/tree.c`)

The tree is a hierarchical structure:
- **Node**: represents a path segment in a chain (`north` parent, `south` child, `east` first leaf, `path`)
- **Leaf**: stores a key/value pair under a node (`key`, `value`, `size`, `east` next leaf)

Implemented/available functionality:
- `example_tree()` / `example_path()` - builds a sample deep path chain (`/a`, `/a/b`, ..., `/a/.../z`)
- `create_node(parent, path)` - allocates and links a child node
- `create_leaf(parent, key, value, count)` - allocates and appends a leaf to a node
- `find_last(parent)` - finds the last leaf in a node's leaf list
- `find_node_linear(path)` - linear node lookup by path (implementation in progress/needs validation)
- `lookup_linear(path, key)` - linear key lookup under a path (implementation in progress; currently has known issues to fix)
- `print_tree(fd, root)` - prints the tree structure and leaf data

### Server module (`src/server.c`)

Current state is experimental scaffolding:
- Socket setup/bind/listen/accept flow (`initServer`, `mainloop`)
- Per-connection child process handling (`fork`, `childloop`)
- Basic line parsing into `cmd`, `folder`, and `args`
- Command handler table/dispatch concept (`CmdHandler`, handler callbacks) present but incomplete

Note: `server.c`/`server.h` currently contain compile-time issues and incomplete wiring; stabilization and proper error handling/messages are part of ongoing work.

## Build and run commands

### Tree demo (as implemented in `src/tree.c`)

```bash
gcc -Iinclude src/tree.c -o tree
./tree
```

### Tree test program (`test_tree.c`)

`test_tree.c` provides its own `main`, while `src/tree.c` also defines `main` (demo). Compile by renaming the demo `main` only in `src/tree.c`:

```bash
gcc -Iinclude -Dmain=tree_demo_main -c src/tree.c -o /tmp/tree.o
gcc -Iinclude -c test_tree.c -o /tmp/test_tree.o
gcc /tmp/test_tree.o /tmp/tree.o -o test_tree
./test_tree
```

### Server (when compilation issues are resolved)

```bash
gcc -Iinclude src/server.c -o server
./server 12049
```

## Learning the code and tests

Suggested path:

1. **Read data structures first**: `include/tree.h` (`TreeNode`, `TreeLeaf`, tags, API).
2. **Trace construction**: follow `example_tree()` in `src/tree.c` to see how nodes are created/linked.
3. **Inspect storage**: read `create_node`, `create_leaf`, and `find_last` to understand `north/south/east` usage.
4. **Visualize output**: run `./tree` or `./test_tree` and compare printed structure with the in-memory model.
5. **Study lookup behavior**: step through `find_node_linear` and `lookup_linear` with sample paths/keys; verify assumptions with assertions or a debugger.
6. **Exercise tests**: use `test_tree.c` as a harness to validate node creation, leaf insertion, and key lookup on a larger tree.

Useful commands while learning:

```bash
# build and run tree demo
gcc -Iinclude src/tree.c -o tree && ./tree

# build and run tree tests (separate mains)
gcc -Iinclude -Dmain=tree_demo_main -c src/tree.c -o /tmp/tree.o \
  && gcc -Iinclude -c tests/test_tree.c -o /tmp/test_tree.o \
  && gcc /tmp/test_tree.o /tmp/tree.o -o test_tree \
  && ./test_tree

# debug a crash (example)
gdb ./test_tree
```

## Diagrams

### Tree structure (conceptual)

Nodes form a path chain via `south`; each node can own a linked list of leaves via `east`.

```text
(root "/")
  |
  south
  v
[Node "/a"] --east--> [Leaf k1:v1] --east--> [Leaf k2:v2] --east--> NULL
  |
  south
  v
[Node "/a/b"] --east--> [Leaf k3:v3] --east--> NULL
  |
  south
  v
[Node "/a/b/c"] --east--> NULL
  |
  south
  v
 ...
```

### Node and leaf relationships

```text
            north
        <-----------
[Node P]             [Node C]
        -----------> 
            south

[Node C].east -> [Leaf L1].east -> [Leaf L2].east -> NULL

Leaf fields: tag, key, value, size, south(back-link), east(next)
Node fields: tag, path, north, south, east(first leaf)
```

### Server request flow (intended)

```text
Client
  |
  | TCP connect/send: "<cmd> <path> <args>"
  v
Server (accept)
  |
  v
childloop: parse -> cmd/path/args
  |
  v
command dispatch (getcmd/handler table)
  |
  +--> insert/update/delete/get/list ...
  |
  v
response to client
```

## Future development

### CLI / command interface

Planned command-style interface (server protocol and/or local CLI) for manipulating tree data:

- `insert <path> <key> <value>` - insert a leaf at `path`
- `get <path> <key>` - retrieve value for `key` at `path`
- `update <path> <key> <value>` - update existing key/value
- `delete <path> <key>` - remove a key/leaf (or node, depending on design)
- `list <path>` - list keys/children under a path
- `create_path <path>` - create missing nodes along a path
- `help` - show supported commands

Example usage:

```text
insert /a/b username harsh
get /a/b username
update /a/b username baghel
list /a/b
delete /a/b username
```

### Planned improvements

- Fix and harden tree lookup/path resolution (`find_node_linear`, `lookup_linear`) and define exact path semantics
- Add proper error handling and user-facing error messages across tree and server operations
- Complete server command parsing/dispatch and wire handlers to tree operations
- Support insert/delete/update/list behaviors consistently (including edge cases)
- Expand automated tests for tree and server commands (positive, negative, and boundary cases)
- Improve memory management (ownership, frees, cleanup on delete/shutdown)
- Add persistence/serialization format if needed
- Add build tooling (e.g. Makefile) and usage documentation for CLI/client workflows

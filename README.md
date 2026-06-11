# server-http

A minimal HTTP server written in C, built from scratch without external libraries. The project focuses on the implementation of core data structures used to manage connections and parse HTTP requests.

## Features

- TCP server listening on port `6700`
- HTTP/1.1 request parsing (request line, headers, body)
- Custom implementations of common data structures
- Leveled logging system (DEBUG, NOTICE, WARNING)
- AddressSanitizer support for debugging

## Project Structure

```
server-http/
├── src/
│   ├── main.c                  # Server entry point, connection loop
│   ├── network.c / network.h   # TCP socket abstraction (accept, send, recv)
│   ├── request.c / request.h   # HTTP request parser
│   ├── data-struct/
│   │   ├── linkedList.c/h      # Doubly linked list with iterator
│   │   ├── queue.c/h           # Queue built on top of the linked list
│   │   └── dictionary.c/h      # Hash map with FNV-1 hashing
│   └── malloc-utils/
│       └── malloc_utils.c/h    # Safe malloc wrappers
└── test/
    └── unit/
        └── client_test.tcl     # Basic client test script
```

## Data Structures

| Structure | Description |
|---|---|
| **Linked List** | Generic doubly linked list with head/tail pointers and a bidirectional iterator |
| **Queue** | Wrapper over the linked list exposing `push`, `pop`, and `peek` operations |
| **Dictionary** | Hash map using separate chaining; supports string and integer keys; uses FNV-1 hashing |

## Build

Requires `gcc` and `make`.

```bash
# Build the server
make all

# Build with AddressSanitizer
make bug-compile

# Clean build artifacts
make clean

# Show available targets
make help
```

## Usage

Start the server (listens on port `6700` by default):

```bash
./server-http
```

Send a test request:

```bash
curl http://localhost:6700/
# → Hello World!
```

## Notes

- The server handles one connection at a time (no threading or `select`/`epoll` yet).
- The current response is a static `200 OK` with `Hello World!` regardless of the request.
- The `dict` type is used internally to store parsed request-line fields (method, URI) and header fields.

## Client

Let's take brpc as an example.

### Channel

- Client-side of RPC sends requests. It's called Channel rather than "Client" in brpc. 
- A channel represents a **communication line** to one server or multiple servers, which can be used for calling services.

While in trpc, cliend-side of RPC is called proxy.(The proxy could interface to anything: a network connection, a large object in memory. So proxy also make sense.)

- A Channel can be shared by all threads in the process.(Same as trpc proxy, it's very important.) 
- However creation and destroying of Channel is not thread-safe, make sure the channel is initialized and destroyed only by one thread.

Obviously, a channel also needs an option.

- Some RPC implementations have so-called "ClientManager", including configurations and resource management at the client-side, which is not needed by brpc.
- "thread-num", "connection-type" such parameters are either in brpc::ChannelOptions or global gflags. Advantages of doing so:
  - Convenience. You don't have to pass a "ClientManager" when the Channel is created, and you don't have to store the "ClientManager". Otherwise code has to pass "ClientManager" layer by layer, which is troublesome. gflags makes configurations of global behaviors easier.
  - Share resources. For example, servers and channels in brpc share background workers (of bthread).
  - Better management of Lifetime. Destructing a "ClientManager" is very error-prone, which is managed by brpc right now.

Oops, trpc is just the oposite of brpc when using options. It has a similar struct called options and client config should be filled in this struct.
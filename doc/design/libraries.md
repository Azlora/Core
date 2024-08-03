# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libazlora_cli*         | RPC client functionality used by *azlora-cli* executable |
| *libazlora_common*      | Home for common functionality shared by different executables and libraries. Similar to *libazlora_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libazlora_consensus*   | Stable, backwards-compatible consensus functionality used by *libazlora_node* and *libazlora_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libazloraconsensus*    | Shared library build of static *libazlora_consensus* library |
| *libazlora_kernel*      | Consensus engine and support library used for validation by *libazlora_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libazloraqt*           | GUI functionality used by *azlora-qt* and *azlora-gui* executables |
| *libazlora_ipc*         | IPC functionality used by *azlora-node*, *azlora-wallet*, *azlora-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libazlora_node*        | P2P and RPC server functionality used by *azlorad* and *azlora-qt* executables. |
| *libazlora_util*        | Home for common functionality shared by different executables and libraries. Similar to *libazlora_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libazlora_wallet*      | Wallet functionality used by *azlorad* and *azlora-wallet* executables. |
| *libazlora_wallet_tool* | Lower-level wallet functionality used by *azlora-wallet* executable. |
| *libazlora_zmq*         | [ZeroMQ](../zmq.md) functionality used by *azlorad* and *azlora-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libazlora_consensus* and *libazlora_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libazlora_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libazlora_node* code lives in `src/node/` in the `node::` namespace
  - *libazlora_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libazlora_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libazlora_util* code lives in `src/util/` in the `util::` namespace
  - *libazlora_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

azlora-cli[azlora-cli]-->libazlora_cli;

azlorad[azlorad]-->libazlora_node;
azlorad[azlorad]-->libazlora_wallet;

azlora-qt[azlora-qt]-->libazlora_node;
azlora-qt[azlora-qt]-->libazloraqt;
azlora-qt[azlora-qt]-->libazlora_wallet;

azlora-wallet[azlora-wallet]-->libazlora_wallet;
azlora-wallet[azlora-wallet]-->libazlora_wallet_tool;

libazlora_cli-->libazlora_util;
libazlora_cli-->libazlora_common;

libazlora_common-->libazlora_consensus;
libazlora_common-->libazlora_util;

libazlora_kernel-->libazlora_consensus;
libazlora_kernel-->libazlora_util;

libazlora_node-->libazlora_consensus;
libazlora_node-->libazlora_kernel;
libazlora_node-->libazlora_common;
libazlora_node-->libazlora_util;

libazloraqt-->libazlora_common;
libazloraqt-->libazlora_util;

libazlora_wallet-->libazlora_common;
libazlora_wallet-->libazlora_util;

libazlora_wallet_tool-->libazlora_wallet;
libazlora_wallet_tool-->libazlora_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class azlora-qt,azlorad,azlora-cli,azlora-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libazlora_wallet* and *libazlora_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libazlora_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libazlora_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libazlora_common* should serve a similar function as *libazlora_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libazlora_util* and *libazlora_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for azlora-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libazlora_kernel* is not supposed to depend on *libazlora_common*, only *libazlora_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libazlora_kernel* should only depend on *libazlora_util* and *libazlora_consensus*.

- The only thing that should depend on *libazlora_kernel* internally should be *libazlora_node*. GUI and wallet libraries *libazloraqt* and *libazlora_wallet* in particular should not depend on *libazlora_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libazlora_consensus*, *libazlora_common*, and *libazlora_util*, instead of *libazlora_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libazloraqt*, *libazlora_node*, *libazlora_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libazlora_node* to *libazlora_kernel* as part of [The libazlorakernel Project #24303](https://github.com/azlora/azlora/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/azlora/azlora/issues/15732)

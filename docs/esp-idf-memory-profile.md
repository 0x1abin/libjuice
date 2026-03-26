# libjuice ESP-IDF Memory Profile

Measured on ESP32-C5, ESP-IDF v6.0, connectivity example (2 local ICE agents, Google STUN server).
Kconfig parameters at default values.

## Kconfig Defaults

| Parameter | Value |
|-----------|-------|
| `JUICE_ICE_MAX_CANDIDATES` | 8 |
| `JUICE_TASK_STACK_SIZE` | 6144 |
| `JUICE_MAX_STUN_SERVERS` | 1 |
| `JUICE_MAX_RELAY_SERVERS` | 1 |
| `JUICE_STUN_MAX_CREDENTIAL_LEN` | 128 |
| `JUICE_BUFFER_SIZE` | 1500 |
| `JUICE_LOG_BUFFER_SIZE` | 256 |
| `JUICE_ENABLE_SERVER` | off |

## Heap Memory (Runtime)

| Measurement | Bytes |
|-------------|-------|
| Heap before agent create | 202,872 |
| Agent 1 cost (`juice_create`) | 6,356 |
| Agent 2 cost (`juice_create`) | 6,092 |
| **Total 2-agent heap cost** | **12,448** |
| Heap after `juice_destroy` both | 200,136 |
| Post-destroy residual (shared registry/mutex) | 2,736 |
| Runtime minimum free heap | 170,364 |

Single agent heap: ~6.2 KB. The first agent costs slightly more due to shared `conn_registry` allocation.

## Stack Usage (FreeRTOS High Water Mark)

| Task | Allocated | Used | Remaining | Utilization |
|------|-----------|------|-----------|-------------|
| `juice poll` (internal thread) | 6,144 | 5,860 | 284 | 95.4% |
| `connectivity_task` (user task) | 8,192 | 4,052 | 4,140 | 49.4% |

The internal poll thread handles STUN message processing (read/write/HMAC) which is the deepest call chain. The 284-byte margin is tight but stable across repeated runs.

## Flash / Static Memory (libjuice component)

| Section | Bytes | Description |
|---------|-------|-------------|
| `.text` | 56,726 | Code in flash |
| `.rodata` | 256 | Constants in flash |
| `.bss` | 1,057 | Zero-initialized static data in RAM |
| `.data` | 184 | Initialized static data in RAM |
| **Total component** | **58,223** | **~57 KB** |

RAM static footprint (`.bss` + `.data`): **1,241 bytes**.

## Key Struct Sizes (32-bit RISC-V)

| Struct | Size (bytes) | Instances per Agent | Subtotal |
|--------|-------------|---------------------|----------|
| `juice_agent_t` | ~10,772 | 1 | 10,772 |
| `ice_description_t` | ~2,152 | 2 (local + remote) | 4,304 |
| `agent_stun_entry_t` | ~336 | 17 | 5,712 |
| `ice_candidate_pair_t` | ~36 | 16 | 576 |
| `ice_candidate_t` | ~260 | 8 per description | 2,080 |
| `stun_message_t` | ~1,252 | stack temporary | - |
| `stun_credentials_t` | ~492 | in stun_message_t | - |

`juice_agent_t` is heap-allocated and embeds `ice_description_t` x2, `agent_stun_entry_t[]`, and `ice_candidate_pair_t[]` inline.

`stun_message_t` (~1,252 B) is placed on the stack during STUN read/write operations, which is the primary driver of the poll thread's stack consumption.

## Per-Agent Summary

| Resource | Cost |
|----------|------|
| Heap (agent struct + conn_impl + socket) | ~6.2 KB |
| Internal thread stack (shared across agents) | 6 KB |
| Flash code + data (fixed, one-time) | 57 KB |
| RAM static (.bss + .data, fixed) | 1.2 KB |

## Total System Cost (2-Agent Connectivity Test)

| Resource | Bytes | Notes |
|----------|-------|-------|
| Heap peak | ~12.4 KB | 2 agents |
| Stack (internal thread) | 6 KB | Shared poll thread |
| Stack (user task) | 8 KB | Application-dependent |
| Flash | 57 KB | libjuice component only |
| RAM static | 1.2 KB | Global state |
| **Total RAM at runtime** | **~28 KB** | heap + stacks + static |

## Tuning Notes

- Reducing `JUICE_ICE_MAX_CANDIDATES` from 8 to 4 saves ~2 KB per agent (fewer candidates, pairs, STUN entries)
- Reducing `JUICE_TASK_STACK_SIZE` below 6144 risks stack overflow during STUN processing
- `JUICE_BUFFER_SIZE` at 1500 (MTU) is optimal; STUN messages are typically < 1000 bytes
- Increasing `JUICE_STUN_MAX_CREDENTIAL_LEN` above 128 is needed only for TURN servers with long-form credentials
- The user task stack can be reduced to ~5 KB if SDP buffers are heap-allocated (as in the current example)

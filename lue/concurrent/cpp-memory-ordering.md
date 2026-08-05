# Links to articles
 - https://www.modernescpp.com/index.php/category/blog/multithreading/multithreading-memory-model/
 - (1) https://www.modernescpp.com/index.php/c-memory-model/
 - https://www.modernescpp.com/index.php/fences-as-memory-barriers/
 

# Memory ordering decision tree

    ┌─────────────────────────┐
                             │ Do you need memory      │
                             │ ordering at all?        │
                             └───────────┬─────────────┘
                                         │
                          ┌──────────────┴──────────────┐
                          │                             │
                         NO                            YES
                          │                             │
                          ▼                             ▼
                ┌─────────────────┐         ┌─────────────────────┐
                │    relaxed      │         │ Is there a single    │
                │                 │         │ global order needed? │
                └─────────────────┘         └───────────┬─────────┘
                                                        │
                                         ┌──────────────┴──────────────┐
                                         │                             │
                                        YES                           NO
                                         │                             │
                                         ▼                             ▼
                               ┌─────────────────┐         ┌─────────────────────┐
                               │    seq_cst      │         │ What type of op?    │
                               │  (safest)       │         └───────────┬─────────┘
                               └─────────────────┘                     │
                                                        ┌──────────────┼──────────────┐
                                                        │              │              │
                                                      Load          Store          RMW
                                                        │              │              │
                                                        ▼              ▼              ▼
                                                  ┌──────────┐  ┌──────────┐  ┌──────────┐
                                                  │ acquire  │  │ release  │  │ acq_rel  │
                                                  └──────────┘  └──────────┘  └──────────┘
                                                  

# Memory Order Comparison Table

| Memory Order | Atomic | Order Before | Order After | Global Order | Typical Use |
|--------------|--------|--------------|-------------|--------------|-------------|
| `relaxed` | ✓ | ✗ | ✗ | ✗ | Counters |
| `consume` | ✓ | ✗ | Dependent only | ✗ | Pointer chains (rare) |
| `acquire` | ✓ | ✗ | ✓ Reads | ✗ | Consumer, load |
| `release` | ✓ | ✓ Writes | ✗ | ✗ | Producer, store |
| `acq_rel` | ✓ | ✓ Writes | ✓ Reads | ✗ | RMW operations |
| `seq_cst` | ✓ | ✓ All | ✓ All | ✓ | Default, complex cases |

**Legend:**
- **Atomic** = Guarantees the operation itself is atomic (no torn reads/writes)
- **Order Before** = Prevents reordering of operations *before* this one
- **Order After** = Prevents reordering of operations *after* this one
- **Global Order** = All threads see operations in a single total order

 

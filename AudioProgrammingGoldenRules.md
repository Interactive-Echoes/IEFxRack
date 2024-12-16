# Audio Programming Golden Rules

### Audio Thread Rules:
- Consider Algorithm Complexity
- Consider Data Structure Complexity (something like std::vector/arrays preferred)
- No Locks, strive for atomic data (keep locks very minimal)
- No memory allocations (Including resizing/adding/reallocating to std::vector)
- No Logging
- No OS Calls unless the OS guarantees real time performance (some embedded OS)
- No disk access (std::fread, file IO, serialization etc...)
- No Network Activity
- Don't trust 3rd Party Library calls

### Data Model
- Data best to live on UI Thread
- Understand floating point vs fixed maths
    - floating points is easier math but comes with NANs, noise and hides multiple issues
    - Fixed integer math is more explicit and forces to think about overflow and resolution issues
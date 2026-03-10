# Arbitrary Precision Calculator in C

A command-line arbitrary precision calculator written in C that performs arithmetic on numbers of unlimited size. Standard C integer types overflow at 64 bits. This calculator sidesteps that limit entirely by representing every number as a doubly linked list of digits and reimplementing all arithmetic the same way you would on paper, one digit at a time.

---

## What it does

The calculator accepts two operands and an operator as command-line arguments and prints the exact result with no overflow, no approximation, and no upper bound on the size of the inputs.

Supported operations:

| Operator | Operation      | Example input                        |
|----------|----------------|--------------------------------------|
| `+`      | Addition       | `./calc.out 99999999999999999999 + 1`|
| `-`      | Subtraction    | `./calc.out 1000000000000000000 - 1` |
| `x`      | Multiplication | `./calc.out 123456789 x 987654321`   |
| `/`      | Division       | `./calc.out 1000000000000000000 / 3` |
| `%`      | Modulus        | `./calc.out 1000000000000000000 % 7` |
| `^`      | Exponentiation | `./calc.out 2 ^ 64`                  |

All operations support negative operands. The sign of the result is determined before any arithmetic is performed, so the arithmetic functions themselves always work on positive magnitudes.

---

## Project structure

```
.
├── apc.h              # Core header — Dlist struct, macros, arithmetic prototypes
├── list.h             # Header for doubly linked list utility functions
├── apc_utils.c        # Input validation, string-to-list conversion, sign resolution
├── list.c             # Doubly linked list — insert, delete, compare, copy, print
├── main.c             # Entry point — argument parsing, dispatch, output
├── addition.c         # Digit-wise addition with carry
├── subtraction.c      # Digit-wise subtraction with borrowing
├── multiplication.c   # Grade-school multiplication with partial products
├── division.c         # Long division using repeated subtraction
├── modulus.c          # Remainder via long division
├── power.c            # Fast exponentiation by squaring (recursive)
└── makefile           # Wildcard-based build — auto-detects all .c files
```

---

## Building

Requires GCC and Make.

```bash
make
```

The makefile automatically collects every `.c` file in the directory, compiles each into an object file inside `obj/`, and links them into the executable `calc.out`.

To remove all build artifacts:

```bash
make clean
```

---

## Usage

```bash
./calc.out <operand1> <operator> <operand2>
```

For help:

```bash
./calc.out --help
```

Note: use `x` for multiplication, not `*`. The shell expands `*` as a filename wildcard before the program ever receives it.

---

## Sample output

```
./calc.out 12345678901234567890 + 98765432109876543210
```

```
------------------------------
Input:    12345678901234567890
Operator: +
Input:    98765432109876543210
------------------------------
Result:   111111111011111111100
------------------------------
```

---

## Implementation notes

**Data structure**

Every number is stored as a doubly linked list of single digits. The head node holds the most significant digit and the tail holds the least significant digit. Doubly linked nodes are required because addition and subtraction traverse from tail to head (right to left), while printing and division traverse from head to tail (left to right).

**Sign handling**

`apc_utils.c` resolves the sign of the result before any arithmetic is called. The function `check_operation_type()` inspects the signs of both operands and the operator, determines what actual operation to perform on the magnitudes, and sets a `sign_flag` that `main.c` uses when printing. The arithmetic functions in `addition.c`, `subtraction.c`, and so on never receive or produce negative numbers.

**Subtraction precondition**

`subtraction.c` always expects the first operand to be greater than or equal to the second. `main.c` enforces this by calling `compare_list()` before dispatch and swapping the arguments if necessary. The sign flag accounts for the swap.

**Multiplication**

Implements the standard grade-school algorithm. The outer loop iterates over digits of the multiplier from least significant to most significant. For each digit, the entire multiplicand is multiplied digit by digit and the partial product is accumulated into the result via `addition()`. Place value is handled by inserting a zero into an empty partial-product list before the inner loop runs, which causes the zero to end up at the tail (least significant position) as digits are prepended to the front.

**Division and modulus**

Both use the long division algorithm. A working window is built from the leading digits of the dividend, the divisor is subtracted from it repeatedly until the remainder is smaller than the divisor, and the subtraction count becomes the next quotient digit. The next dividend digit is then appended to the remainder and the process repeats. `division.c` discards the final remainder and keeps the quotient. `modulus.c` discards the quotient and copies the final remainder into the result.

**Exponentiation**

Uses recursive exponentiation by squaring:

```
base ^ 0   = 1
base ^ 1   = base
base ^ n   = (base ^ (n/2)) ^ 2           if n is even
base ^ n   = (base ^ (n/2)) ^ 2 * base    if n is odd
```

This reduces the number of multiplications from O(n) to O(log n). The exponent is halved by calling `division()` with a divisor of 2. Whether the exponent is odd or even is determined by inspecting the least significant digit of the exponent list. Exponents greater than 9999 are rejected before the computation begins.

Rather than using global variables for sign or operation type, all sign and operation decisions are computed once in `check_operation_type()` and passed into the dispatch block as local variables. This keeps the arithmetic functions stateless and reusable.

**Memory management**

Every node is heap-allocated via `malloc`. Every temporary list created during a computation is freed before the function that created it returns. `dl_delete_list()` is called in `main.c` on all three operand and result lists after the result is printed. There are no static buffers and no fixed limits on the size of the numbers being computed.

---

## Limitations

- The exponent in power operations is limited to 4 digits (maximum 9999) to keep computation time practical.
- Division and modulus use repeated subtraction to find each quotient digit rather than trial multiplication, which means up to 9 subtractions per digit of the quotient.
- One digit is stored per node. Packing more digits per node (a common optimization in production big-integer libraries) would reduce memory usage and improve cache performance but was not a goal of this implementation.
- Negative exponents are not supported. The result would be a fraction, and this calculator operates on integers only.

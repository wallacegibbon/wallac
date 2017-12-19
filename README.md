# Introduction

(Not finished yet)

A C compiler written in pure C, do not depend on tools like LEX or YACC.

This is not a standard C implementation, it's a subset of C89 standard with the following limits:

- `while`, `if` and `switch` are supported. `do...while` and `for` are not.
- `struct` is supported. `enum` and `union` are not.
- Octal number in string is partly supported.
  For example, `"\0"` works fine, but `"\123"` will be treated as `"123"`,
  not decimal number `83`.


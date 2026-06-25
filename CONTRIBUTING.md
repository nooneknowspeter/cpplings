# Contributing to cpplings

Thank you for your interest in contributing to cpplings! 

This document provides guidelines and instructions for contributing.

## Code of Conduct

This project adheres to a code of conduct that all contributors are expected to follow. 
Please be respectful and constructive in all interactions.

## Getting Started

### Prerequisites

- Zig >= 0.15.1
- A C++ compiler that supports >= C++23 (zig c++, g++, or clang++)
- Git

### Setting Up Your Development Environment

1. Fork the repository on GitHub
2. Clone your fork locally:
   ```bash
   git clone https://github.com/<your_username>/cpplings.git
   cd cpplings
   ```
3. Install dependencies using Nix (recommended):
   ```bash
   nix develop
   # or
   direnv allow # direnv users
   ```
   Or manually:
   - Install a compiler, -std=c++23
   - Install Zig >= 0.15.1
   - Install clang-tools (for formatting)

### Building

```bash
zig build
```

### Running Tests

```bash
zig build tests
```

## Project Structure

```
cpplings/
├── exercises/           # Exercise files (user modifies these)
│   ├── 00_intro/
│   ├── 01_variables/
│   └── ...
├── patches/
│   └── solutions/      # Complete solutions for exercises
├── src/                # CLI source code (Zig)
│   └── ...
├── tests/              # Test files
└── build.zig           # Zig build configuration
```

## Creating Exercises

### Exercise File Naming

Exercises follow the pattern: `{NN}_{topic}.cpp`

- `00_intro/01_intro.cpp`
- `01_variables/01_variables.cpp`
- `02_functions/03_functions.cpp`

### Exercise Structure

Each exercise has two versions:

1. **Exercise file** (`exercises/`): Contains TODOs and errors for users to fix
2. **Solution file** (`patches/solutions/`): Complete working solution

### Exercise Template

```cpp
// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Explanation of the concept being taught.
//
// https://www.learncpp.com/cpp-tutorial/...
// any other reference and learning material

#include <gtest/gtest.h>

// TODO: Implement the function that does X.
// NOTE: Use the test case as reference.
int functionName(params)
{
    // Code with TODOs for the user to complete
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(ChapterName, ExerciseName)
{
    ASSERT_EQ(functionName(test_input), expected_output);
}
```

### Solution Template

Solutions include full explanations:

```cpp
// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// The same explaination as the exercise or a 
// detailed explanation of the concept.
//
// Key points:
// - Point 1
// - Point 2
//
// https://www.learncpp.com/cpp-tutorial/...
// any other reference and learning material

#include <gtest/gtest.h>

// Complete implementation with comments
int functionName(params)
{
    return result;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(ChapterName, ExerciseName)
{
    ASSERT_EQ(functionName(test_input), expected_output);
}
```

## Exercise Conventions

### TODO Comments

Use TODO comments to indicate what users should fix:

```cpp
// TODO: Add the missing keyword
// TODO: Fix the function signature
// NOTE: Hints or reminders
```

### Test Structure

- Use Google Test (gtest) for assertions
- Keep tests simple and focused
- Test edge cases where appropriate

### Code Requirements

- Must compile with `-std=c++23`
- Should pass all tests
- Follow modern C++ practices (C++17+ features encouraged, show pre C++17 differences for history)

## Code Style

### C++ Code

- Use `auto` when type is obvious
- Follow [C++ core guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) best practices
- Microsoft style convention

### Zig Code

- Follow Zig standard library conventions
- Use `b` context for string handling and runtime structures
- Proper error handling with `try`/`catch`
- Constants and imports must be UPPER_SNAKE_CASE
- Variables must be snake_case
- Functions must be camelCase
- Structs and enums must be PascalCase

### Formatting

Run formatting before committing:

> [!NOTE]
> 
> `treefmt` must be installed with the formatters listed in the [`flake.nix`](./flake.nix).

```bash
treefmt
```

## Testing

### Running Exercises

```bash
# Run the CLI
zig build run

# Run against solutions
zig build run -- -s

# Generate patches
zig build run -- -p
```

### Running CLI Tests

```bash
zig build tests
```

## Pull Request Process

1. **Fork and Branch**: Create a feature branch from `develop`
   ```bash
   git checkout -b feature/your-feature-name
   ```

   or

   ```bash
   git switch -c feature/your-feature-name
   ```

2. **Make Changes**: Implement your changes following the conventions

3. **Test**: Ensure all tests pass
   ```bash
   zig build tests
   ```

4. **Generate Patches**: Generate the unified diffs of the added exercises and solutions
    ```bash
    zig build run -- -p
    ```

5. **Format**: Run the formatter
   ```bash
   treefmt
   ```

   can be done inline with:

   ```bash
   nix develop -c treefmt
   ```

6. **Commit**: Use clear, descriptive commit messages
   ```bash
   git commit -m "feat: add exercise for C++ strings and string_view"
   ```

7. **Push and PR**: Push to your fork and create a pull request

### Commit Message Format

Please use semantic commit messages:

- `feat:` New feature
- `fix:` Bug fix
- `docs:` Documentation changes
- `refactor:` Code refactoring
- `test:` Adding or updating tests
- `chore:` Build process or auxiliary tool changes

## Questions?

If you have questions or need help, please open an issue on GitHub.

## License

By contributing, you agree that your contributions will be licensed under the same license as the project.

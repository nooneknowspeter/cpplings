# Patches

This directory contains solution files and patch files for cpplings exercises.

## Directory Structure

```
patches/
├── solutions/           # Complete solution files
│   ├── 00_intro/
│   ├── 01_variables/
│   └── ...
└── README.md           # This file
```

## Solutions

The `solutions/` subdirectory contains the complete, working solutions for each exercise. These files are used:

- When running `cpplings -s` to verify all exercises compile correctly
- For generating patch files
- As reference when users are stuck

### Solution File Format

Solution files have the same structure as exercise files but with:

- All TODOs completed
- Full comments and explanations
- Working test cases
- Expected output

## Patches

Patch files (`.patch`) are generated using the unified diff format. They show the differences between a solution file and its corresponding exercise file.

### Generating Patches

Run the CLI with the patch flag:

```bash
zig build run -- -p
```

This generates patches for all exercise-solution pairs.

### Patch File Format

Patches use the unified diff format:

```diff
--- patches/solutions/01_variables/01_variables.cpp
+++ exercises/01_variables/01_variables.cpp
@@ -1,20 +1,10 @@
 // Comment explaining the concept
 #include <iostream>

-// Complete solution code
-// with explanatory comments
+// TODO: Fix this line
+int x{};

 int main() { return 0; }
```

- Lines starting with `-` are in the solution but removed in the exercise
- Lines starting with `+` are added in the exercise
- Lines without either are unchanged

## How It Works

1. **Exercise File** (`exercises/`): Users work on these files
2. **Solution File** (`patches/solutions/`): Complete, working reference
3. **Patch File**: Unified diff showing the differences

The patch system allows:

- Users to see hints by comparing exercise to solution
- Automatic verification that exercises are solvable
- Easy maintenance of exercise content

## Contributing

When adding new exercises:

1. Create the exercise file in `exercises/{chapter}/`
2. Create the solution file in `patches/solutions/{chapter}/`
3. Ensure the solution file has full explanations
4. Generate the patch file using `cpplings -p`

## Example Workflow

```bash
# User is stuck on an exercise
# They can look at the solution for hints:
cat patches/solutions/01_variables/01_variables.cpp

# Or generate a patch to see the differences:
diff -u patches/solutions/01_variables/01_variables.cpp \
      exercises/01_variables/01_variables.cpp
```

## Notes

- Patch files are generated automatically
- Do not edit patch files manually
- If an exercise changes, regenerate patches with `cpplings -p`

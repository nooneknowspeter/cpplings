const STD = @import("std");
const COMPILE_FLAGZ = @import("compile_flagz");
const CONFIG = @import("config");
const BUILTIN = @import("builtin");

pub fn build(b: *STD.Build) !void {
    if (BUILTIN.zig_version.minor < 15) {
        @compileError("Zig >= v0.15.1 is required...");
    }

    const COMPILER_FLAGS = [_][]const u8{
        "-std=c++23",
        "-Wall",
        "-Werror",
        "-Wextra",
        "-pedantic",
    };

    const TARGET = b.standardTargetOptions(.{});
    const OPTIMIZE = b.standardOptimizeOption(.{});

    // dependencies
    const DEP_GTEST = b.dependency(
        "googletest",
        .{},
    );

    // cli
    {
        const CPPLINGS_CLI = b.addExecutable(.{
            .name = "cpplings_cli",
            .root_module = b.createModule(.{
                .target = TARGET,
                .optimize = OPTIMIZE,
                .valgrind = true,
                .link_libcpp = true,
                .link_libc = true,
                .single_threaded = false,
            }),
        });

        CPPLINGS_CLI.root_module.addIncludePath(b.path("include"));
        CPPLINGS_CLI.root_module.addIncludePath(b.path("src"));

        // TODO: pass in cpp source files
        CPPLINGS_CLI.root_module.addCSourceFile(.{
            .file = b.path("src/main.cpp"),
            .flags = &COMPILER_FLAGS,
        });

        b.installArtifact(CPPLINGS_CLI);
        const CPPLINGS_CLI_ARTIFIACT = b.addRunArtifact(CPPLINGS_CLI);

        const CPPLINGS_CLI_RUN_STEP = b.step("run", "Run cpplings cli");
        CPPLINGS_CLI_RUN_STEP.dependOn(&CPPLINGS_CLI_ARTIFIACT.step);
        CPPLINGS_CLI_ARTIFIACT.step.dependOn(b.getInstallStep());

        if (b.args) |args| {
            if (args.len > 0) {
                CPPLINGS_CLI_ARTIFIACT.addArgs(args);
            }
        }
    }

    // tests
    {
        const CPPLINGS_CLI_TESTS = b.addExecutable(.{
            .name = "cpplings_tests",
            .root_module = b.createModule(.{
                .target = TARGET,
                .optimize = OPTIMIZE,
                .link_libc = true,
                .link_libcpp = true,
                .valgrind = true,
                .single_threaded = true,
            }),
        });

        CPPLINGS_CLI_TESTS.root_module.addIncludePath(b.path("include"));
        CPPLINGS_CLI_TESTS.root_module.addIncludePath(b.path("src"));

        // TODO: pass in cpp source files
        CPPLINGS_CLI_TESTS.root_module.addCSourceFile(.{
            .file = b.path("src/tests/test.cpp"),
            .flags = &COMPILER_FLAGS,
        });

        b.installArtifact(CPPLINGS_CLI_TESTS);
        const CPPLINGS_CLI_TESTS_ARTIFACT = b.addRunArtifact(CPPLINGS_CLI_TESTS);

        const CPPLINGS_CLI_TESTS_STEP = b.step("tests", "Run cpplings tests");
        CPPLINGS_CLI_TESTS_STEP.dependOn(&CPPLINGS_CLI_TESTS_ARTIFACT.step);
        CPPLINGS_CLI_TESTS_ARTIFACT.step.dependOn(b.getInstallStep());
    }

    // exercises
    {
        const CPPLINGS_EXERCISE = b.addExecutable(.{
            .name = "cpplings_exercise",
            .root_module = b.createModule(.{
                .target = TARGET,
                .optimize = OPTIMIZE,
                .link_libc = true,
                .link_libcpp = true,
                .single_threaded = false,
            }),
        });

        if (b.args) |args| {
            if (args.len > 0) {
                const EXERCISE_FILENAMES = args;

                CPPLINGS_EXERCISE.root_module.addCSourceFiles(.{
                    .flags = &COMPILER_FLAGS,
                    .files = EXERCISE_FILENAMES,
                });

                CPPLINGS_EXERCISE.root_module.addIncludePath(b.path("include"));
                CPPLINGS_EXERCISE.root_module.addIncludePath(b.path("exercises"));
                CPPLINGS_EXERCISE.root_module.linkLibrary(DEP_GTEST.artifact("gtest"));
                // CPPLINGS_EXERCISE.root_module.linkLibrary(DEP_GTEST.artifact("gtest_main"));

                b.installArtifact(CPPLINGS_EXERCISE);
            }
        }

        const CPPLINGS_EXERCISE_ARTIFACT = b.addRunArtifact(CPPLINGS_EXERCISE);

        const CPPLINGS_RUN_EXERCISE_STEP = b.step("exercises", "Build and run cpplings_exercise exercise");
        CPPLINGS_RUN_EXERCISE_STEP.dependOn(&CPPLINGS_EXERCISE_ARTIFACT.step);
        CPPLINGS_EXERCISE_ARTIFACT.step.dependOn(b.getInstallStep());

        if (b.args) |args| {
            if (args.len > 0) {
                CPPLINGS_EXERCISE_ARTIFACT.addArgs(args);
            }
        }
    }

    // create compile flags generator
    // TODO: update compile flagz to zig 0.16.0 std
    // TODO: variable args, multi args
    // TODO: facade + dependency injection; find system include paths inside lib
    // TODO: strategy pattern; use different compilers to get include paths
    {
        var cflags = COMPILE_FLAGZ.addCompileFlags(b);
        cflags.addIncludePath(b.path("include"));
        cflags.addIncludePath(b.path("src"));
        cflags.addIncludePath(DEP_GTEST.path("include"));

        const CLANG_PLUS_PLUS = try STD.process.Child.run(.{ .allocator = b.allocator, .argv = &[_][]const u8{
            "zig",
            "c++",
            "-E",
            "-x",
            "c++",
            "-",
            "-v",
        } });
        var clang_plus_plus_output = STD.mem.splitScalar(
            u8,
            CLANG_PLUS_PLUS.stderr,
            '\n',
        );
        var start_capture = false;

        while (clang_plus_plus_output.next()) |line| {
            if (STD.mem.startsWith(u8, line, "#include <...> search starts here:")) {
                start_capture = true;
                continue;
            }

            if (STD.mem.startsWith(u8, line, "End of search list.")) {
                break;
            }

            if (start_capture) {
                cflags.addIncludePath(.{ .cwd_relative = STD.mem.trim(u8, line, " ") });
            }
        }

        const CFLAGS_STEP = b.step("compile-flags", "Generate compile_flags.txt for C/C++ IDE support");
        CFLAGS_STEP.dependOn(&cflags.step);
    }
}


const STD = @import("std");
const COMPILE_FLAGZ = @import("compile_flagz");
const BUILTIN = @import("builtin");

pub fn build(b: *STD.Build) !void {
    if (BUILTIN.zig_version.minor < 16) {
        @compileError("Zig >= v0.16.0 is required...");
    }

    const TARGET = b.standardTargetOptions(.{});
    const OPTIMIZE = b.standardOptimizeOption(.{});

    const COMPILER_FLAGS = [_][]const u8{
        "-std=c++23",
        "-Wall",
        "-Werror",
        "-Wextra",
        "-pedantic",
    };

    // dependencies
    const DEP_GTEST = b.dependency(
        "googletest",
        .{},
    );

    const DEP_CATCH = b.dependency(
        "catch2",
        .{},
    );

    const DEP_BOOST = b.dependency("boost", .{
        .target = TARGET,
        .optimize = OPTIMIZE,
    });

    // cli
    {
        const CPPLINGS_CLI = b.addExecutable(.{
            .name = "cpplings_cli",
            .root_module = b.createModule(.{
                .target = TARGET,
                .optimize = OPTIMIZE,
                .link_libcpp = true,
                .link_libc = true,
            }),
        });

        b.installArtifact(CPPLINGS_CLI);

        // TODO: pass in cpp source files
        // NOTE: exclude ./src/tests
        CPPLINGS_CLI.root_module.addCSourceFiles(.{
            .flags = &COMPILER_FLAGS,
            .files = &[_][]const u8{
                "src/main.cpp",
                "src/tui.cpp",
            },
        });

        CPPLINGS_CLI.root_module.addIncludePath(b.path("src"));

        const BOOST_ARTIFACT = DEP_BOOST.artifact("boost");
        for (BOOST_ARTIFACT.root_module.include_dirs.items) |include_dir| {
            try CPPLINGS_CLI.root_module.include_dirs.append(
                b.allocator,
                include_dir,
            );
        }

        CPPLINGS_CLI.root_module.linkLibrary(BOOST_ARTIFACT);

        const CPPLINGS_CLI_RUN_STEP = b.step(
            "run",
            "Run cpplings cli",
        );

        const CPPLINGS_CLI_RUN_CMD = b.addRunArtifact(
            CPPLINGS_CLI,
        );
        CPPLINGS_CLI_RUN_STEP.dependOn(&CPPLINGS_CLI_RUN_CMD.step);
        CPPLINGS_CLI_RUN_CMD.step.dependOn(b.getInstallStep());

        if (b.args) |args| {
            if (args.len > 0) {
                CPPLINGS_CLI_RUN_CMD.addArgs(args);
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
                .link_libcpp = true,
                .link_libc = true,
                .single_threaded = true,
            }),
        });

        b.installArtifact(CPPLINGS_CLI_TESTS);

        // TODO: pass in cpp source files
        // NOTE: only ./src/tests
        CPPLINGS_CLI_TESTS.root_module.addCSourceFile(.{
            .file = b.path("src/tests/test.cpp"),
            .flags = &COMPILER_FLAGS,
        });

        CPPLINGS_CLI_TESTS.root_module.addIncludePath(b.path("src"));
        // CPPLINGS_CLI_TESTS.root_module.linkLibrary(DEP_GTEST.artifact("gtest"));
        CPPLINGS_CLI_TESTS.root_module.linkLibrary(DEP_CATCH.artifact("Catch2"));

        const CPPLINGS_CLI_TESTS_STEP = b.step(
            "tests",
            "Run cpplings tests",
        );

        const CPPLINGS_CLI_TESTS_ARTIFACT = b.addRunArtifact(
            CPPLINGS_CLI_TESTS,
        );

        CPPLINGS_CLI_TESTS_STEP.dependOn(
            &CPPLINGS_CLI_TESTS_ARTIFACT.step,
        );
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

                CPPLINGS_EXERCISE.root_module.addIncludePath(
                    b.path("include"),
                );
                CPPLINGS_EXERCISE.root_module.addIncludePath(
                    b.path("exercises"),
                );
                // CPPLINGS_EXERCISE.root_module.linkLibrary(
                //     DEP_GTEST.artifact("gtest"),
                // );
                // CPPLINGS_EXERCISE.root_module.linkLibrary(
                //     DEP_GTEST.artifact("gtest_main"),
                // );

                b.installArtifact(CPPLINGS_EXERCISE);
            }
        }

        const CPPLINGS_EXERCISE_ARTIFACT = b.addRunArtifact(
            CPPLINGS_EXERCISE,
        );

        const CPPLINGS_RUN_EXERCISE_STEP = b.step(
            "exercises",
            "Build and run cpplings_exercise exercise",
        );
        CPPLINGS_RUN_EXERCISE_STEP.dependOn(
            &CPPLINGS_EXERCISE_ARTIFACT.step,
        );
        CPPLINGS_EXERCISE_ARTIFACT.step.dependOn(b.getInstallStep());

        if (b.args) |args| {
            if (args.len > 0) {
                CPPLINGS_EXERCISE_ARTIFACT.addArgs(args);
            }
        }
    }

    // create compile flags generator
    {
        var cflags = COMPILE_FLAGZ.configureCompileFlags(
            b,
            .{
                .language_variant = .cxx23,
                .warnings = .{
                    .Wall = true,
                    .Werror = true,
                },
                .compiler = .zigcxx,
                .paths = &[_]STD.Build.LazyPath{
                    b.path("src"),
                    DEP_GTEST.path("zig-out/include"),
                    DEP_BOOST.path("zig-out/include"),
                },
                .custom = null,
            },
        );

        const CFLAGS_STEP = b.step(
            "compile-flags",
            "Generate compile_flags.txt for C/C++ IDE support",
        );
        CFLAGS_STEP.dependOn(&cflags.step);
    }
}

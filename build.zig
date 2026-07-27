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

        .filesystem = true,
        .process = true,
    });

    // cli
    {
        const SOURCE_CORE = [_][]const u8{
            "src/tui.cpp",
            "src/exercise_iterator.cpp",
            "src/file_watcher.cpp",
            "src/exercise_runner.cpp",
            // "src/patch_system.cpp",
        };

        const SOURCE_MAIN = [_][]const u8{
            "src/main.cpp",
        };

        const SOURCE_TEST = [_][]const u8{
            "src/tests/test.cpp",
        };

        const CPPLINGS_MODULE = b.createModule(.{
            .target = TARGET,
            .optimize = OPTIMIZE,
            .link_libcpp = true,
            .link_libc = true,
            .single_threaded = false,
        });

        const CPPLINGS_CLI = b.addExecutable(.{
            .name = "cpplings_cli",
            .root_module = CPPLINGS_MODULE,
        });

        b.installArtifact(CPPLINGS_CLI);

        const CPPLINGS_CLI_RUN_STEP = b.step(
            "run",
            "Run cpplings cli",
        );
        const TEST_OPTION = b.option(bool, "tests", "Run cpplings tests") orelse false;

        CPPLINGS_CLI.root_module.addCSourceFiles(.{
            .flags = &COMPILER_FLAGS,
            .files = &SOURCE_CORE,
        });

        if (TEST_OPTION) {
            CPPLINGS_CLI.root_module.addCSourceFiles(.{
                .flags = &COMPILER_FLAGS,
                .files = &SOURCE_TEST,
            });
        } else {
            CPPLINGS_CLI.root_module.addCSourceFiles(.{
                .flags = &COMPILER_FLAGS,
                .files = &SOURCE_MAIN,
            });
        }

        CPPLINGS_CLI.root_module.addIncludePath(b.path("src"));

        CPPLINGS_CLI.root_module.linkLibrary(DEP_GTEST.artifact("gtest"));

        CPPLINGS_CLI.root_module.linkLibrary(DEP_CATCH.artifact("Catch2"));

        const BOOST_ARTIFACT = DEP_BOOST.artifact("boost");
        for (BOOST_ARTIFACT.root_module.include_dirs.items) |include_dir| {
            try CPPLINGS_CLI.root_module.include_dirs.append(
                b.allocator,
                include_dir,
            );
        }
        CPPLINGS_CLI.root_module.linkLibrary(BOOST_ARTIFACT);

        const CPPLINGS_CLI_RUN_CMD = b.addRunArtifact(
            CPPLINGS_CLI,
        );
        CPPLINGS_CLI_RUN_CMD.step.dependOn(b.getInstallStep());
        CPPLINGS_CLI_RUN_STEP.dependOn(&CPPLINGS_CLI_RUN_CMD.step);

        if (b.args) |args| {
            if (args.len > 0) {
                CPPLINGS_CLI_RUN_CMD.addArgs(args);
            }
        }
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
            var exercise_filenames: STD.ArrayList([]const u8) = STD.ArrayList([]const u8).empty;

            for (args) |arg| {
                if (STD.mem.endsWith(u8, arg, ".cpp")) {
                    try exercise_filenames.append(b.allocator, arg);
                }
            }

            if (exercise_filenames.items.len > 0) {
                CPPLINGS_EXERCISE.root_module.addCSourceFiles(.{
                    .flags = &COMPILER_FLAGS,
                    .files = exercise_filenames.items,
                });

                CPPLINGS_EXERCISE.root_module.addIncludePath(
                    b.path("exercises"),
                );
                CPPLINGS_EXERCISE.root_module.linkLibrary(
                    DEP_GTEST.artifact("gtest"),
                );
                CPPLINGS_EXERCISE.root_module.linkLibrary(
                    DEP_CATCH.artifact("Catch2"),
                );
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
                    DEP_CATCH.path("zig-out/include"),
                    DEP_BOOST.path("zig-out/include"),
                },
                .custom = &[_][]const u8{
                    "-pedantic",
                    "-D_LIBCPP_HAS_FILESYSTEM=1",
                    "-D_LIBCPP_HAS_THREADS=1",
                    "-D_LIBCPP_HAS_TIME_ZONE_DATABASE=1",
                    "-D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_NONE",
                    "-D_LIBCPP_HAS_MONOTONIC_CLOCK=1",
                },
            },
        );

        const CFLAGS_STEP = b.step(
            "compile-flags",
            "Generate compile_flags.txt for C/C++ IDE support",
        );
        CFLAGS_STEP.dependOn(&cflags.step);
    }
}

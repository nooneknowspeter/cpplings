const STD = @import("std");
const COMPILE_FLAGZ = @import("compile_flagz");
const CONFIG = @import("config");
const BUILTIN = @import("builtin");

pub fn build(b: *STD.Build) !void {
    if (BUILTIN.zig_version.minor < 15) {
        @compileError("Zig >= v0.15.1 is required...");
    }

    const TARGET = b.standardTargetOptions(.{});
    const OPTIMIZE = b.standardOptimizeOption(.{});
    const COMPILER_FLAGS = [_][]const u8{ "-std=c++23", "-Wall", "-Werror", "-Wextra" };

    // dependencies
    const DEP_GTEST = b.dependency("googletest", .{});

    // cli
    const CPPLINGS_CLI = b.addExecutable(.{ .name = "cpplings_cli", .root_module = b.createModule(.{
        .target = TARGET,
        .optimize = OPTIMIZE,
        .root_source_file = b.path("src/main.zig"),
    }) });

    b.installArtifact(CPPLINGS_CLI);

    const CPPLINGS_CLI_STEP = b.step("run", "Run cpplings cli");
    const CPPLINGS_CLI_CMD = b.addRunArtifact(CPPLINGS_CLI);
    CPPLINGS_CLI_STEP.dependOn(&CPPLINGS_CLI_CMD.step);
    CPPLINGS_CLI_CMD.step.dependOn(b.getInstallStep());

    // exercises
    const CPPLINGS_EXERCISE = b.addExecutable(.{
        .name = "cpplings_exercise",
        .root_module = b.createModule(.{
            .target = TARGET,
            .optimize = OPTIMIZE,
            .link_libc = true,
            .link_libcpp = true,
        }),
    });

    if (b.args) |args| {
        if (args.len > 0) {
            const EXERCISE_FILENAMES = args;

            CPPLINGS_EXERCISE.root_module.addCSourceFiles(.{ .flags = &COMPILER_FLAGS, .files = EXERCISE_FILENAMES });

            CPPLINGS_EXERCISE.root_module.addIncludePath(b.path("include"));
            CPPLINGS_EXERCISE.root_module.addIncludePath(b.path("exercises"));
            CPPLINGS_EXERCISE.root_module.linkLibrary(DEP_GTEST.artifact("gtest"));
            // CPPLINGS_EXERCISE.root_module.linkLibrary(DEP_GTEST.artifact("gtest_main"));

            b.installArtifact(CPPLINGS_EXERCISE);
        }
    }

    const CPPLINGS_RUN_EXERCISE_STEP = b.step("exercises", "Build and run cppligns_exercise exercise");
    const CPPLINGS_RUN_EXERCISE_CMD = b.addRunArtifact(CPPLINGS_EXERCISE);
    CPPLINGS_RUN_EXERCISE_STEP.dependOn(&CPPLINGS_RUN_EXERCISE_CMD.step);
    CPPLINGS_RUN_EXERCISE_CMD.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        if (args.len > 0) {
            CPPLINGS_RUN_EXERCISE_CMD.addArgs(args);
        }
    }

    // create compile flags generator
    var cflags = COMPILE_FLAGZ.addCompileFlags(b);
    cflags.addIncludePath(b.path("include"));
    cflags.addIncludePath(b.path("src"));
    cflags.addIncludePath(DEP_GTEST.path("include"));

    const CLANG_PLUS_PLUS = try STD.process.Child.run(.{ .allocator = b.allocator, .argv = &[_][]const u8{ "zig", "c++", "-E", "-x", "c++", "-", "-v" } });
    var clang_plus_plus_output = STD.mem.splitScalar(u8, CLANG_PLUS_PLUS.stderr, '\n');
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

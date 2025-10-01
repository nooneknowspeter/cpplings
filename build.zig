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
    const CPPLIGNS_EXERCISE = b.addExecutable(.{
        .name = "CPPLIGNS_EXERCISE",
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

            CPPLIGNS_EXERCISE.root_module.addCSourceFiles(.{ .flags = &COMPILER_FLAGS, .files = EXERCISE_FILENAMES });

            CPPLIGNS_EXERCISE.root_module.addIncludePath(b.path("include"));
            CPPLIGNS_EXERCISE.root_module.linkLibrary(DEP_GTEST.artifact("gtest"));
            CPPLIGNS_EXERCISE.root_module.linkLibrary(DEP_GTEST.artifact("gtest_main"));

            b.installArtifact(CPPLIGNS_EXERCISE);
        }
    }

    const CPPLINGS_RUN_EXERCISE_STEP = b.step("exercises", "Build and run cppligns_exercise exercise");
    const CPPLINGS_RUN_EXERCISE_CMD = b.addRunArtifact(CPPLIGNS_EXERCISE);
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

    // TODO: automate libcc path
    // $ clang++ -E -x c++ - -v < /dev/null
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/a3c2pnnyycikxs9gnxgakvilajyxhyv2-lldb-19.1.7-dev/include" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/82kmz7r96navanrc2fgckh2bamiqrgsw-gcc-14.3.0/include" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/n7p5cdg3d55fr659qm8h0vynl3rcf26h-compiler-rt-libc-19.1.7-dev/include" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/82kmz7r96navanrc2fgckh2bamiqrgsw-gcc-14.3.0/include/c++/14.3.0" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/82kmz7r96navanrc2fgckh2bamiqrgsw-gcc-14.3.0/include/c++/14.3.0/x86_64-unknown-linux-gnu" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/fbfcll570w9vimfbh41f9b4rrwnp33f3-clang-wrapper-19.1.7/resource-root/include" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/gf3wh0x0rzb1dkx0wx1jvmipydwfzzd5-glibc-2.40-66-dev/include" });

    const CFLAGS_STEP = b.step("compile-flags", "Generate compile_flags.txt for C/C++ IDE support");
    CFLAGS_STEP.dependOn(&cflags.step);
}

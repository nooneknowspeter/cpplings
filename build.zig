const std = @import("std");
const compile_flagz = @import("compile_flagz");
const config = @import("config");
const builtin = @import("builtin");

pub fn build(b: *std.Build) !void {
    if (builtin.zig_version.minor < 15) {
        @compileError("Zig >= v0.15.1 is required...");
    }

    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const compiler_flags = [_][]const u8{ "-std=c++23", "-Wall", "-Werror", "-Wextra" };

    // dependencies
    const dep_gtest = b.dependency("googletest", .{});

    // cli
    const cpplings_cli = b.addExecutable(.{ .name = "cpplings_cli", .root_module = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .root_source_file = b.path("src/main.zig"),
    }) });

    b.installArtifact(cpplings_cli);

    const cpplings_cli_step = b.step("cli", "Run cpplings cli");
    const cpplings_cli_cmd = b.addRunArtifact(cpplings_cli);
    cpplings_cli_step.dependOn(&cpplings_cli_cmd.step);
    cpplings_cli_cmd.step.dependOn(b.getInstallStep());

    // exercises
    const cpplings = b.addExecutable(.{
        .name = "cpplings",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
            .link_libcpp = true,
        }),
    });

    if (b.args) |args| {
        if (args.len > 0) {
            const exercise_filenames = args;

            cpplings.root_module.addCSourceFiles(.{ .flags = &compiler_flags, .files = exercise_filenames });

            cpplings.root_module.addIncludePath(b.path("include"));
            cpplings.root_module.linkLibrary(dep_gtest.artifact("gtest"));
            cpplings.root_module.linkLibrary(dep_gtest.artifact("gtest_main"));

            b.installArtifact(cpplings);
        }
    }

    const cpplings_run_exercise_step = b.step("exercises", "Build and run cpplings exercise");
    const cpplings_run_exercise_cmd = b.addRunArtifact(cpplings);
    cpplings_run_exercise_step.dependOn(&cpplings_run_exercise_cmd.step);
    cpplings_run_exercise_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        if (args.len > 0) {
            cpplings_run_exercise_cmd.addArgs(args);
        }
    }

    // create compile flags generator
    var cflags = compile_flagz.addCompileFlags(b);
    cflags.addIncludePath(b.path("include"));
    cflags.addIncludePath(b.path("src"));
    cflags.addIncludePath(dep_gtest.path("include"));

    // TODO: automate libcc path
    // $ clang++ -E -x c++ - -v < /dev/null
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/a3c2pnnyycikxs9gnxgakvilajyxhyv2-lldb-19.1.7-dev/include" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/82kmz7r96navanrc2fgckh2bamiqrgsw-gcc-14.3.0/include" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/n7p5cdg3d55fr659qm8h0vynl3rcf26h-compiler-rt-libc-19.1.7-dev/include" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/82kmz7r96navanrc2fgckh2bamiqrgsw-gcc-14.3.0/include/c++/14.3.0" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/82kmz7r96navanrc2fgckh2bamiqrgsw-gcc-14.3.0/include/c++/14.3.0/x86_64-unknown-linux-gnu" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/fbfcll570w9vimfbh41f9b4rrwnp33f3-clang-wrapper-19.1.7/resource-root/include" });
    cflags.addIncludePath(.{ .cwd_relative = "/nix/store/gf3wh0x0rzb1dkx0wx1jvmipydwfzzd5-glibc-2.40-66-dev/include" });

    const cflags_step = b.step("compile-flags", "Generate compile_flags.txt for C/C++ IDE support");
    cflags_step.dependOn(&cflags.step);
}

const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{ .name = "cpplings", .target = target, .optimize = optimize, .link_libc = true });

    exe.addCSourceFile(.{ .file = b.path("src/main.cpp") });

    exe.linkLibC();
    exe.linkLibCpp();

    b.installArtifact(exe);

    const run_exe = b.addRunArtifact(exe);

    const run_step = b.step("run", "run cpplings");

    run_step.dependOn(&run_exe.step);
}


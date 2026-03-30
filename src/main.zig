const STD = @import("std");

const CLI = @import("cli.zig");
const PATCH_SYSTEM = @import("patch.zig");
const EXERCISE_RUNNER = @import("exercise_runner.zig");

pub fn main() !void {
    var mem_arena = STD.heap.ArenaAllocator.init(STD.heap.page_allocator);
    defer mem_arena.deinit();

    const ALLOCATOR = mem_arena.allocator();

    if (STD.os.argv.len == 1) {
        try CLI.run(ALLOCATOR, .{});
        return;
    }

    const arg1 = STD.mem.span(STD.os.argv[1]);
    const arg2 = if (STD.os.argv.len > 2) STD.mem.span(STD.os.argv[2]) else "";

    if (STD.mem.eql(u8, arg1, "-h") or STD.mem.eql(u8, arg1, "--help") or STD.mem.eql(u8, arg1, "help")) {
        CLI.help_documentation();
    } else if (STD.mem.eql(u8, arg1, "-s") or STD.mem.eql(u8, arg1, "--solutions")) {
        try CLI.run(ALLOCATOR, .{ .exercises_dir_path = "patches/solutions" });
    } else if (STD.mem.eql(u8, arg1, "-p") or STD.mem.eql(u8, arg1, "--patch")) {
        const PATCH_SYSTEM_INSTANCE = try PATCH_SYSTEM.PatchSystem.init(ALLOCATOR, .{});
        defer PATCH_SYSTEM_INSTANCE.deinit(ALLOCATOR);
    } else if (STD.mem.eql(u8, arg1, "-l") or STD.mem.eql(u8, arg1, "--list")) {
        try CLI.list(ALLOCATOR);
    } else if (STD.mem.eql(u8, arg1, "-H") or STD.mem.eql(u8, arg1, "--hint")) {
        if (arg2.len > 0) {
            try CLI.showHint(ALLOCATOR, arg2);
        } else {
            CLI.help_documentation();
        }
    } else if (STD.mem.eql(u8, arg1, "-r") or STD.mem.eql(u8, arg1, "--reset")) {
        if (arg2.len > 0) {
            try CLI.reset(ALLOCATOR, arg2);
        } else {
            CLI.help_documentation();
        }
    } else if (STD.mem.eql(u8, arg1, "-c") or STD.mem.eql(u8, arg1, "--compiler")) {
        var exercise_runner = EXERCISE_RUNNER.ExerciseRunner.init();
        defer exercise_runner.deinit(ALLOCATOR);

        if (STD.mem.eql(u8, arg2, "g++")) {
            exercise_runner.setStrategy(.gpp);
        } else if (STD.mem.eql(u8, arg2, "clang++")) {
            exercise_runner.setStrategy(.clang);
        } else {
            exercise_runner.setStrategy(.zig);
        }

        try CLI.run(ALLOCATOR, .{ .exercise_runner = exercise_runner });
    } else if (STD.mem.eql(u8, arg1, "-d") or STD.mem.eql(u8, arg1, "--debug")) {
        try CLI.run(ALLOCATOR, .{ .debug_enabled = true });
    } else if (STD.mem.eql(u8, arg1, "--manual-run")) {
        try CLI.run(ALLOCATOR, .{ .manual_run = true });
    } else {
        CLI.help_documentation();
    }
}

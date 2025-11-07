const STD = @import("std");
const CLI = @import("cli/cli.zig");

pub fn help_documentation() void {
    STD.debug.print("{s}\n", .{CLI.ASCII_ART});
    STD.debug.print("cpplings [OPTION]\n\n", .{});
    STD.debug.print("Options:\n", .{});
    STD.debug.print("-h | --help | help -> output this help documentation\n", .{});
    STD.debug.print("-s | --solutions -> run and test cpplings against solutions dir\n", .{});
}

pub fn main() !void {
    var mem_arena = STD.heap.ArenaAllocator.init(STD.heap.page_allocator);
    defer mem_arena.deinit();
    const mem_allocator = mem_arena.allocator();

    if (STD.os.argv.len > 2) {
        STD.debug.print("Too many arguments\n", .{});
        help_documentation();
    }

    if (STD.os.argv.len == 1) {
        try CLI.run(mem_allocator, .{});
        return;
    }

    const PROGRAM_ARGUMENT = STD.mem.span(STD.os.argv[1]);

    if (STD.mem.eql(u8, PROGRAM_ARGUMENT, "-s") or STD.mem.eql(u8, PROGRAM_ARGUMENT, "--solutions")) {
        try CLI.run(mem_allocator, .{ .exercises_dir_path = ".patches/solutions" });
    } else if (STD.mem.eql(u8, PROGRAM_ARGUMENT, "-h") or STD.mem.eql(u8, PROGRAM_ARGUMENT, "--help") or STD.mem.eql(u8, PROGRAM_ARGUMENT, "help")) {
        help_documentation();
    } else {
        help_documentation();
    }
}

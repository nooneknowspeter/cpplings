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

    try CLI.run(mem_allocator);
}

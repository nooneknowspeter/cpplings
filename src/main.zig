const STD = @import("std");
const CLI = @import("cli/cli.zig");

pub fn main() !void {
    var mem_arena = STD.heap.ArenaAllocator.init(STD.heap.page_allocator);
    defer mem_arena.deinit();
    const mem_allocator = mem_arena.allocator();

    try CLI.run(mem_allocator);
}

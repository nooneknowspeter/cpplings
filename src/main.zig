const STD = @import("std");

const CLI = @import("cli.zig");
const PATCH_SYSTEM = @import("patch.zig");

pub fn main() !void {
    var mem_arena = STD.heap.ArenaAllocator.init(STD.heap.page_allocator);
    defer mem_arena.deinit();

    const ALLOCATOR = mem_arena.allocator();

    if (STD.os.argv.len > 2) {
        STD.debug.print("Too many arguments\n", .{});
        CLI.help_documentation();
    }

    if (STD.os.argv.len == 1) {
        try CLI.run(ALLOCATOR, .{});
        return;
    }

    const PROGRAM_ARGUMENT = STD.mem.span(STD.os.argv[1]);

    if (STD.mem.eql(u8, PROGRAM_ARGUMENT, "-s") or STD.mem.eql(u8, PROGRAM_ARGUMENT, "--solutions")) {
        try CLI.run(ALLOCATOR, .{ .exercises_dir_path = ".patches/solutions" });
    } else if (STD.mem.eql(u8, PROGRAM_ARGUMENT, "-p") or STD.mem.eql(u8, PROGRAM_ARGUMENT, "--patch")) {
        const PATCH_SYSTEM_INSTANCE = try PATCH_SYSTEM.PatchSystem.init(ALLOCATOR, .{});
        defer PATCH_SYSTEM_INSTANCE.deinit(ALLOCATOR);
    } else if (STD.mem.eql(u8, PROGRAM_ARGUMENT, "-h") or STD.mem.eql(u8, PROGRAM_ARGUMENT, "--help") or STD.mem.eql(u8, PROGRAM_ARGUMENT, "help")) {
        CLI.help_documentation();
    } else {
        CLI.help_documentation();
    }
}

const STD = @import("std");
const UTIL = @import("./../cli/util.zig");

fn initFSTests() !void {
    STD.fs.cwd().access("tests/test_dir/chapter", .{}) catch {
        try STD.fs.cwd().makePath("tests/test_dir/chapter");
    };

    STD.fs.cwd().access("tests/test_dir/chapter/test.cpp", .{}) catch {
        var file = try STD.fs.cwd().createFile("tests/test_dir/chapter/test.cpp", .{});
        defer file.close();
    };
}

test "iterate test directory and filter files" {
    try initFSTests();

    var list_of_contents: STD.ArrayList([]const u8) = .empty;
    defer list_of_contents.deinit(STD.testing.allocator);

    try UTIL.iterateDirectory(STD.testing.allocator, "tests/test_dir/chapter", .{ .allow_move_contents = true, .include_filter = ".cpp", .is_print_list_of_contents = true, .move_contents_to = &list_of_contents });

    try STD.testing.expect(STD.mem.eql(u8, list_of_contents.items[0], "tests/test_dir/chapter/test.cpp"));
}

test "directory tree" {
    var allocator: STD.mem.Allocator = STD.testing.allocator;

    const dir_tree: *UTIL.DIR_TREE = try allocator.create(UTIL.DIR_TREE);
    defer allocator.destroy(dir_tree);

    try dir_tree.init(allocator, ".patches/solutions");
}

test "run sub process with default arguments" {
    var process_output: STD.ArrayList(u8) = .empty;
    defer process_output.deinit(STD.testing.allocator);

    try UTIL.runSubProcess(STD.testing.allocator, &process_output, .{});

    try STD.testing.expect(STD.mem.eql(u8, process_output.items, "this is a sub-process\n"));
}

test "run hello world sub process" {
    var process_output: STD.ArrayList(u8) = .empty;
    defer process_output.deinit(STD.testing.allocator);

    try UTIL.runSubProcess(STD.testing.allocator, &process_output, .{ .args = "echo hello world" });

    try STD.testing.expect(STD.mem.eql(u8, process_output.items, "hello world\n"));
}

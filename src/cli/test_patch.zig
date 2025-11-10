const STD = @import("std");
const PATCH = @import("patch.zig");

const TEST_PATCHES_DIR = "tests/patches";

const ORIGINAL_FILES_DIR = TEST_PATCHES_DIR ++ "/" ++ "original";

const ORIGINAL_FILE_PATH = ORIGINAL_FILES_DIR ++ "/" ++ "original.txt";

const MODIFIED_FILES_DIR = TEST_PATCHES_DIR ++ "/" ++ "modified";

const MODIFIED_FILE_PATH = MODIFIED_FILES_DIR ++ "/" ++ "modified.txt";

fn initPatchTest() !void {
    STD.fs.cwd().access(ORIGINAL_FILES_DIR, .{}) catch {
        try STD.fs.cwd().makePath(ORIGINAL_FILES_DIR);
    };

    STD.fs.cwd().access(MODIFIED_FILES_DIR, .{}) catch {
        try STD.fs.cwd().makePath(MODIFIED_FILES_DIR);
    };

    STD.fs.cwd().access(ORIGINAL_FILE_PATH, .{}) catch {
        try STD.fs.cwd().writeFile(.{ .sub_path = ORIGINAL_FILE_PATH, .data = "hello", .flags = .{ .truncate = true } });
    };

    STD.fs.cwd().access(MODIFIED_FILE_PATH, .{}) catch {
        try STD.fs.cwd().writeFile(.{ .sub_path = MODIFIED_FILE_PATH, .data = "hey", .flags = .{ .truncate = true } });
    };
}

test "patch system" {
    var allocator: STD.mem.Allocator = STD.testing.allocator;

    try PATCH.run(allocator);
}

test "patch" {
    var allocator: STD.mem.Allocator = STD.testing.allocator;

    _ = allocator;
}

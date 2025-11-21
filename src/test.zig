const STD = @import("std");

const CLI = @import("cli.zig");
const PATCH = @import("patch.zig");
const UTIL = @import("util.zig");

test "run all tests" {
    STD.testing.refAllDecls(@This());
}

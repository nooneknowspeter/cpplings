const STD = @import("std");

test "run all tests" {
    STD.testing.refAllDecls(@This());

    comptime {
        _ = @import("cli.zig");
        _ = @import("patch.zig");
        _ = @import("util.zig");
        _ = @import("exercise_runner.zig");
        _ = @import("exercise_iterator.zig");
        _ = @import("file_watcher.zig");
    }
}

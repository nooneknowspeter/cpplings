const STD = @import("std");
const STYLES = @import("styles.zig");
const BUILTIN = @import("builtin");
const NATIVE_OS = BUILTIN.target.os.tag;

// FIX:
// TODO: doc comments
const PATH_BUILDER = struct {
    const Self = @This();

    path_iterator: STD.ArrayList([]const u8),

    pub fn add(self: *Self, allocator: STD.mem.Allocator, dir_or_file: []const u8) !void {
        const SEPERATOR = switch (NATIVE_OS) {
            .windows => "\\",
            else => "/",
        };

        try self.path_iterator.appendSlice(allocator, &[_][]const u8{ dir_or_file, SEPERATOR });
    }

    pub fn addSlice(self: *Self, allocator: STD.mem.Allocator, dir_or_file_slice: []const []const u8) !void {
        for (dir_or_file_slice) |dir_or_file| {
            try self.add(allocator, dir_or_file);
        }
    }

    pub fn build(self: *Self, allocator: STD.mem.Allocator) ![]u8 {
        return STD.mem.concat(allocator, u8, self.path_iterator);
    }

    pub fn denit(self: *Self, allocator: STD.mem.Allocator) void {
        allocator.destroy(self.path_iterator);
        allocator.destroy(self);
    }
};

test "build paths" {
    var allocator: STD.mem.Allocator = STD.testing.allocator;

    const PATH_BUILDER_INSTANCE = try allocator.create(PATH_BUILDER);
    defer PATH_BUILDER_INSTANCE.denit(allocator);

    try PATH_BUILDER_INSTANCE.add(allocator, "hello");
    try PATH_BUILDER_INSTANCE.addSlice(allocator, &[_][]const u8{ "hello", "world" });
}

/// iterate directory (
///    allocator: STD.mem.Allocator,
///    dir_path: []const u8,
///    extra_options: struct {
///        is_debug: bool = false,
///        include_filter: []const u8 = "", -> include contents containing filter in filename, leave as "" to disable
///        exclude_filter: []const u8 = "", -> exclude contents containing filter in filename, leave as "" to disable
///        is_print_list_of_contents: bool = true, -> print list of contents
///        allow_move_contents: bool = false, -> allow move semantics
///        move_contents_to: *STD.ArrayList([]const u8) = .empty, -> move or append contents to slice or ArrayList pointer
///    },
/// )
///
/// examples
/// --------
/// try iterateDirectory(STD.testing.allocator, "exercises/00_intro", .{}); -> default settings
///
/// try iterateDirectory(STD.testing.allocator, "exercises/00_intro", .{ .filter = ".cpp"}); -> filter cpp files
pub fn iterateDirectory(
    allocator: STD.mem.Allocator,
    dir_path: []const u8,
    extra_options: struct {
        is_debug: bool = false,
        include_filter: []const u8 = "",
        exclude_filter: []const u8 = "",
        is_print_list_of_contents: bool = false,
        allow_move_contents: bool = false,
        move_contents_to: *STD.ArrayList([]const u8) = undefined,
    },
) !void {
    var list_of_contents: STD.ArrayList([]const u8) = .empty;
    defer list_of_contents.deinit(allocator);

    var dir = try STD.fs.cwd().openDir(dir_path, .{ .iterate = true });
    defer dir.close();

    var contents_in_dir = dir.iterate();

    if (extra_options.is_debug) {
        STD.debug.print("\nDebug mode\n", .{});
    }

    while (try contents_in_dir.next()) |content| {
        // FIX: memory leak
        const CONTENT_FILE_PATH = try STD.fs.path.join(allocator, &[_][]const u8{ dir_path, content.name });
        // defer allocator.free(CONTENT_FILE_PATH);

        if (extra_options.is_debug) {
            STD.debug.print("{s}\n", .{CONTENT_FILE_PATH});
        }

        if (extra_options.exclude_filter.len > 0 and STD.mem.containsAtLeast(u8, content.name, 1, extra_options.exclude_filter)) {
            continue;
        }

        if (extra_options.include_filter.len < 0) {
            try list_of_contents.append(allocator, CONTENT_FILE_PATH);

            continue;
        }

        if (extra_options.include_filter.len > 0 and STD.mem.containsAtLeast(u8, content.name, 1, extra_options.include_filter)) {
            try list_of_contents.append(allocator, CONTENT_FILE_PATH);
        }
    }

    STD.sort.insertion(
        []const u8,
        list_of_contents.items,
        {},
        struct {
            fn lessThan(_: void, a: []const u8, b: []const u8) bool {
                return STD.mem.lessThan(u8, a, b);
            }
        }.lessThan,
    );

    if (extra_options.is_print_list_of_contents) {
        STD.debug.print("\nList of contents\n", .{});

        for (list_of_contents.items) |content_path| {
            STD.debug.print("{s}\n", .{content_path});
        }
    }

    if (extra_options.allow_move_contents) {
        try extra_options.move_contents_to.appendSlice(allocator, list_of_contents.items);
    }
}

// TODO: build tree/graph
// TODO: doc comments
const TREE_NODE = struct {
    const Self = @This();

    parent: *TREE_NODE,
    children: STD.ArrayList(*TREE_NODE),
    node_contents: []const u8,
};

pub const DIR_TREE = struct {
    const Self = @This();

    allocator: STD.mem.Allocator,
    root_node: *TREE_NODE,
    children_lookup: STD.ArrayList(*TREE_NODE),

    pub fn init(self: *Self, allocator: STD.mem.Allocator, dir_path: []const u8) !void {
        self.allocator = allocator;

        var root_children: STD.ArrayList([]const u8) = .empty;
        defer root_children.deinit(allocator);

        self.root_node = try allocator.create(TREE_NODE);

        try iterateDirectory(allocator, dir_path, .{ .allow_move_contents = true, .move_contents_to = &root_children, .is_print_list_of_contents = true });

        for (root_children.items) |child| {
            const CHILD_NODE: *TREE_NODE = try allocator.create(TREE_NODE);
            CHILD_NODE.parent = self.root_node;
            CHILD_NODE.node_contents = child;

            try self.children_lookup.append(allocator, CHILD_NODE);
        }
    }

    // pub fn iterateDirectoryTree(self: *Self, dir_path: []const u8, extra_options: struct { is_debug: bool = true, is_print_list_of_contents: bool = true, sub_dir_filter: []const u8 = "" }) !void {
    //     _ = extra_options;
    //
    //     iterateDirectory(self.allocator, dir_path, .{ .allow_move_contents = false, .filter = "", .is_debug = true, .is_print_list_of_contents = true, .move_contents_to = undefined });
    // }

    pub fn deinit(self: *Self, allocator: STD.mem.Allocator) void {
        allocator.destroy(self.root_node);
        allocator.destroy(self);
    }
};

test "directory tree" {
    var allocator: STD.mem.Allocator = STD.testing.allocator;

    const dir_tree: *DIR_TREE = try allocator.create(DIR_TREE);
    defer dir_tree.deinit(allocator);

    try dir_tree.init(allocator, ".patches/solutions");
}

// TODO: doc comments
pub fn runSubProcess(allocator: STD.mem.Allocator, process_output_buffer: *STD.ArrayList(u8), extra_options: struct {
    args: []const u8 = "echo this is a sub-process",
}) !void {
    const BUFFER_SIZE = comptime STD.math.pow(usize, 2, 16);

    var process_args: STD.ArrayList([]const u8) = .empty;
    defer process_args.deinit(allocator);

    var input_args_iterator = STD.mem.splitAny(u8, extra_options.args, " ");

    while (input_args_iterator.next()) |arg| {
        try process_args.append(allocator, arg);
    }

    var process = STD.process.Child.init(process_args.items, allocator);

    process.stderr_behavior = .Pipe;
    process.stdout_behavior = .Pipe;

    var process_stdout_buffer: STD.ArrayList(u8) = .empty;
    defer process_stdout_buffer.deinit(allocator);

    var process_stderr_buffer: STD.ArrayList(u8) = .empty;
    defer process_stderr_buffer.deinit(allocator);

    process.spawn() catch {
        STD.debug.print("{s}Failed spawning process, {s}{s}", .{
            STYLES.ASCII_STYLES.red,
            extra_options.args,
            STYLES.ASCII_STYLES.clear_style,
        });
    };

    try process.collectOutput(allocator, &process_stdout_buffer, &process_stderr_buffer, BUFFER_SIZE);

    const PROCESS_STATUS = try process.wait();

    if (PROCESS_STATUS.Exited != 0) {
        try process_output_buffer.appendSlice(allocator, process_stderr_buffer.items);
        return;
    }

    try process_output_buffer.appendSlice(allocator, process_stdout_buffer.items);
    return;
}

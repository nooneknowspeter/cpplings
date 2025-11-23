const STD = @import("std");

const STYLES = @import("styles.zig");
const BUILTIN = @import("builtin");

pub const NATIVE_OS = BUILTIN.target.os.tag;

/// const PATH_BUILDER_INSTANCE = try allocator.create(PATH_BUILDER); -> create an instance of the PATH_BUILDER using an allocator
///
/// PATH_BUILDER_INSTANCE.init(); -> initialize the instance
/// defer PATH_BUILDER_INSTANCE.denit(allocator); -> de-initialize the instance
///
/// try PATH_BUILDER_INSTANCE.add(allocator, "."); -> add a single string slice
/// try PATH_BUILDER_INSTANCE.addSlice(allocator, &[_][]const u8{ "hello", "world" }); -> add a array of string slices
///
/// var path: STD.ArrayList(u8) = .empty; -> create an array list for move semantics
/// defer path.deinit(allocator);
///
/// try PATH_BUILDER_INSTANCE.build(allocator, &path); -> build and move contents to created array list
pub const PATH_BUILDER = struct {
    const Self = @This();

    path_iterator: STD.ArrayList([]const u8),

    pub fn init(self: *Self) void {
        self.path_iterator = .empty;
    }

    /// add(allocator, "."); -> add a single string slice
    pub fn add(
        self: *Self,
        allocator: STD.mem.Allocator,
        dir_or_file: []const u8,
    ) !void {
        try self.path_iterator.appendSlice(allocator, &[_][]const u8{dir_or_file});
    }

    /// addSlice(allocator, &[_][]const u8{ "hello", "world" }); -> add a array of string slices
    pub fn addSlice(
        self: *Self,
        allocator: STD.mem.Allocator,
        dir_or_file_slice: []const []const u8,
    ) !void {
        for (dir_or_file_slice) |dir_or_file| {
            try self.add(allocator, dir_or_file);
        }
    }

    fn clear(self: *Self, allocator: STD.mem.Allocator) !void {
        self.path_iterator.clearAndFree(allocator);
    }

    /// build(allocator, &path); -> build and move contents to created array list
    pub fn build(
        self: *Self,
        allocator: STD.mem.Allocator,
        move_to: *STD.ArrayList(u8),
    ) !void {
        const FILE_PATH = try STD.fs.path.join(
            allocator,
            self.path_iterator.items,
        );
        defer allocator.free(FILE_PATH);

        try self.clear(allocator);

        try move_to.appendSlice(allocator, FILE_PATH);
    }

    pub fn denit(self: *Self, allocator: STD.mem.Allocator) void {
        self.path_iterator.deinit(allocator);
        allocator.destroy(self);
    }
};

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

    const PATH_BUILDER_INSTANCE = try allocator.create(PATH_BUILDER);
    PATH_BUILDER_INSTANCE.init();
    defer PATH_BUILDER_INSTANCE.denit(allocator);

    var dir = try STD.fs.cwd().openDir(dir_path, .{ .iterate = true });
    defer dir.close();

    var contents_in_dir = dir.iterate();

    if (extra_options.is_debug) {
        STD.debug.print("\nDebug mode\n", .{});
    }

    while (try contents_in_dir.next()) |content| {
        var content_file_path: STD.ArrayList(u8) = .empty;
        defer content_file_path.deinit(allocator);

        try PATH_BUILDER_INSTANCE.addSlice(
            allocator,
            &[_][]const u8{ dir_path, content.name },
        );
        try PATH_BUILDER_INSTANCE.build(
            allocator,
            &content_file_path,
        );

        if (extra_options.is_debug) {
            STD.debug.print("{s}\n", .{content_file_path.items});
        }

        if (extra_options.include_filter.len == 0 and extra_options.exclude_filter.len == 0) {
            const content_file_path_copy = try allocator.dupe(u8, content_file_path.items);

            try list_of_contents.append(allocator, content_file_path_copy);

            continue;
        }

        if (extra_options.exclude_filter.len > 0 and STD.mem.containsAtLeast(u8, content.name, 1, extra_options.exclude_filter)) {
            continue;
        }

        if (extra_options.include_filter.len > 0 and STD.mem.containsAtLeast(u8, content.name, 1, extra_options.include_filter)) {
            const content_file_path_copy = try allocator.dupe(u8, content_file_path.items);

            try list_of_contents.append(allocator, content_file_path_copy);
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
    } else {
        for (list_of_contents.items) |content_path| {
            allocator.free(content_path);
        }
    }
}

const TREE_NODE = struct {
    const Self = @This();

    parent: *TREE_NODE,
    children: STD.ArrayList(*TREE_NODE),
    node_contents: []const u8,
};

/// const dir_tree: *DIR_TREE = try allocator.create(UTIL.DIR_TREE); -> create an instance using an allocator
/// try dir_tree.init( -> initialize instance
///     allocator,
///     TEST_DIR, -> pass directory, initialize directory as the root node of the tree
/// );
/// defer dir_tree.deinit(allocator); -> de-initialize the instance
///
/// var filtered_dir_contents: STD.ArrayList([]const u8) = .empty;
/// defer {
///     for (filtered_dir_contents.items) |content| {
///         allocator.free(content);
///     }
///
///     filtered_dir_contents.deinit(allocator);
/// }
///
/// try dir_tree.buildandSearchFSTree(
///     allocator, -> dependency inject allocator
///     dir_tree.root_node, -> use self.root_node for recursion
///     extra_options: struct {
///         .is_debug_mode = false,
///         .include_filter = "", -> include contents containing filter in filename, leave as "" to disable
///         .allow_move_contents = true, -> allow move semantics
///         .move_contents_to = &filtered_dir_contents,  -> move or append contents to slice or ArrayList pointer
/// });
pub const DIR_TREE = struct {
    const Self = @This();

    root_node: *TREE_NODE,

    pub fn init(
        self: *Self,
        allocator: STD.mem.Allocator,
        dir_path: []const u8,
    ) !void {
        self.root_node = try allocator.create(TREE_NODE);
        self.root_node.node_contents = dir_path;
    }

    fn buildandSearchFSTree(
        self: *Self,
        allocator: STD.mem.Allocator,
        node: *TREE_NODE,
        extra_options: struct {
            is_debug_mode: bool = false,
            include_filter: []const u8 = "",
            allow_move_contents: bool = false,
            move_contents_to: *STD.ArrayList([]const u8) = undefined,
        },
    ) !void {
        if (extra_options.include_filter.len == 0) {
            if (extra_options.is_debug_mode) {
                STD.debug.print("{s}\n", .{node.node_contents});
            }

            if (extra_options.allow_move_contents) {
                const node_contents_copy = try allocator.dupe(u8, node.node_contents);

                try extra_options.move_contents_to.append(allocator, node_contents_copy);
            }
        }

        if (extra_options.include_filter.len > 0 and STD.mem.containsAtLeast(u8, node.node_contents, 1, extra_options.include_filter)) {
            if (extra_options.is_debug_mode) {
                STD.debug.print("{s}\n", .{node.node_contents});
            }

            if (extra_options.allow_move_contents) {
                const node_contents_copy = try allocator.dupe(u8, node.node_contents);

                try extra_options.move_contents_to.append(allocator, node_contents_copy);
            }
        }

        _ = STD.fs.cwd().openDir(node.node_contents, .{}) catch {
            return;
        };

        var children_node_contents: STD.ArrayList([]const u8) = .empty;
        defer {
            for (children_node_contents.items) |content| {
                allocator.free(content);
            }

            children_node_contents.deinit(allocator);
        }

        try iterateDirectory(allocator, node.node_contents, .{
            .allow_move_contents = true,
            .move_contents_to = &children_node_contents,
        });

        var children_nodes: STD.ArrayList(*TREE_NODE) = .empty;
        defer children_nodes.deinit(allocator);

        for (children_node_contents.items) |content| {
            const CHILD_NODE: *TREE_NODE = try allocator.create(TREE_NODE);
            defer allocator.destroy(CHILD_NODE);

            CHILD_NODE.parent = node;
            CHILD_NODE.node_contents = content;

            try children_nodes.append(allocator, CHILD_NODE);
        }

        for (children_nodes.items) |child_node| {
            try self.buildandSearchFSTree(allocator, child_node, extra_options);
        }
    }

    pub fn deinit(
        self: *Self,
        allocator: STD.mem.Allocator,
    ) void {
        allocator.destroy(self.root_node);
        allocator.destroy(self);
    }
};

/// run a program as a child process
///
/// runSubProcess(
///     allocator: STD.mem.Allocator,
///     process_output_buffer: *STD.ArrayList(u8), -> move process output; STDOUT and STDERR to arraylist argument
///     extra_options: struct {
///     args: []const u8 = "echo this is a sub-process", -> program and arguments
/// })
///
/// examples
/// --------
/// var process_output: STD.ArrayList(u8) = .empty;
/// defer process_output.deinit(STD.testing.allocator);
/// try runSubProcess(STD.testing.allocator, &process_output, .{ .args = "echo hello world" });
/// try STD.testing.expect(STD.mem.eql(u8, process_output.items, "hello world\n"));
pub fn runSubProcess(
    allocator: STD.mem.Allocator,
    process_output_buffer: *STD.ArrayList(u8),
    extra_options: struct {
        args: []const u8 = "echo this is a sub-process",
    },
) !void {
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

// tests

const TEST_DIR = "tests/test_dir";
const TEST_SUB_DIR = TEST_DIR ++ "/" ++ "chapter";
const TEST_FILE_1 = TEST_SUB_DIR ++ "/" ++ "test.cpp";
const TEST_FILE_2 = TEST_SUB_DIR ++ "/" ++ "test.txt";

fn initTests() !void {
    STD.fs.cwd().access(TEST_SUB_DIR, .{}) catch {
        try STD.fs.cwd().makePath(TEST_SUB_DIR);
    };

    STD.fs.cwd().access(TEST_FILE_1, .{}) catch {
        var file = try STD.fs.cwd().createFile(TEST_FILE_1, .{});
        defer file.close();
    };

    STD.fs.cwd().access(TEST_FILE_2, .{}) catch {
        var file = try STD.fs.cwd().createFile(TEST_FILE_2, .{});
        defer file.close();
    };
}

test "build paths" {
    var mem_arena: STD.heap.ArenaAllocator = STD.heap.ArenaAllocator.init(STD.testing.allocator);
    defer mem_arena.deinit();

    const ALLOCATOR: STD.mem.Allocator = mem_arena.allocator();

    const PATH_BUILDER_INSTANCE = try ALLOCATOR.create(PATH_BUILDER);
    PATH_BUILDER_INSTANCE.init();
    defer PATH_BUILDER_INSTANCE.denit(ALLOCATOR);

    try PATH_BUILDER_INSTANCE.add(ALLOCATOR, ".");
    try PATH_BUILDER_INSTANCE.addSlice(ALLOCATOR, &[_][]const u8{ "hello", "world" });

    var path: STD.ArrayList(u8) = .empty;
    defer path.deinit(ALLOCATOR);
    try PATH_BUILDER_INSTANCE.build(ALLOCATOR, &path);

    const EXPECTED_OUTPUT: []const u8 = switch (NATIVE_OS) {
        .windows => ".\\hello\\world",
        else => "./hello/world",
    };

    try STD.testing.expect(STD.mem.eql(u8, path.items, EXPECTED_OUTPUT));
}

test "iterate test directory and filter files" {
    try initTests();

    var mem_arena: STD.heap.ArenaAllocator = STD.heap.ArenaAllocator.init(STD.testing.allocator);
    defer mem_arena.deinit();

    const ALLOCATOR: STD.mem.Allocator = mem_arena.allocator();

    var list_of_contents: STD.ArrayList([]const u8) = .empty;
    defer {
        for (list_of_contents.items) |path| {
            ALLOCATOR.free(path);
        }

        list_of_contents.deinit(ALLOCATOR);
    }

    try iterateDirectory(
        ALLOCATOR,
        TEST_SUB_DIR,
        .{
            .allow_move_contents = true,
            .include_filter = ".cpp",
            .is_print_list_of_contents = false,
            .move_contents_to = &list_of_contents,
            .is_debug = false,
        },
    );

    try STD.testing.expect(STD.mem.eql(u8, list_of_contents.items[0], "tests/test_dir/chapter/test.cpp"));
}

test "build and search directory tree" {
    var mem_arena: STD.heap.ArenaAllocator = STD.heap.ArenaAllocator.init(STD.testing.allocator);
    defer mem_arena.deinit();

    const ALLOCATOR: STD.mem.Allocator = mem_arena.allocator();

    const dir_tree: *DIR_TREE = try ALLOCATOR.create(DIR_TREE);
    try dir_tree.init(ALLOCATOR, TEST_DIR);
    defer dir_tree.deinit(ALLOCATOR);

    var filtered_dir_contents: STD.ArrayList([]const u8) = .empty;
    defer {
        for (filtered_dir_contents.items) |content| {
            ALLOCATOR.free(content);
        }

        filtered_dir_contents.deinit(ALLOCATOR);
    }

    try dir_tree.buildandSearchFSTree(ALLOCATOR, dir_tree.root_node, .{
        .is_debug_mode = false,
        .include_filter = ".txt",
        .allow_move_contents = true,
        .move_contents_to = &filtered_dir_contents,
    });

    try STD.testing.expect(
        STD.mem.eql(u8, filtered_dir_contents.items[0], "tests/test_dir/chapter/test.txt"),
    );
}

test "run sub process with default arguments" {
    var mem_arena: STD.heap.ArenaAllocator = STD.heap.ArenaAllocator.init(STD.testing.allocator);
    defer mem_arena.deinit();

    const ALLOCATOR: STD.mem.Allocator = mem_arena.allocator();

    var process_output: STD.ArrayList(u8) = .empty;
    defer process_output.deinit(ALLOCATOR);

    try runSubProcess(
        ALLOCATOR,
        &process_output,
        .{},
    );

    try STD.testing.expect(
        STD.mem.eql(u8, process_output.items, "this is a sub-process\n"),
    );
}

test "run hello world sub process" {
    var mem_arena: STD.heap.ArenaAllocator = STD.heap.ArenaAllocator.init(STD.testing.allocator);
    defer mem_arena.deinit();

    const ALLOCATOR: STD.mem.Allocator = mem_arena.allocator();

    var process_output: STD.ArrayList(u8) = .empty;
    defer process_output.deinit(ALLOCATOR);

    try runSubProcess(
        ALLOCATOR,
        &process_output,
        .{ .args = "echo hello world" },
    );

    try STD.testing.expect(
        STD.mem.eql(u8, process_output.items, "hello world\n"),
    );
}

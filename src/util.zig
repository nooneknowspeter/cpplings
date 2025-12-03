const STD = @import("std");

const STYLES = @import("styles.zig");
const BUILTIN = @import("builtin");

pub const NATIVE_OS = BUILTIN.target.os.tag;

const TREE_NODE = struct {
    const Self = @This();

    parent: *TREE_NODE,
    children: STD.ArrayList(*TREE_NODE),
    node_contents: []const u8,

    pub fn init(
        self: *Self,
        allocator: STD.mem.Allocator,
        node_options: struct {
            parent: *TREE_NODE = undefined,
            node_contents: []const u8 = "empty",
        },
    ) !void {
        const DUPLICATED_CHILD_CONTENT = try allocator.dupe(u8, node_options.node_contents);
        self.node_contents = DUPLICATED_CHILD_CONTENT;

        self.parent = node_options.parent;
        self.children = .empty;
    }

    pub fn deinit(self: *Self, allocator: STD.mem.Allocator) void {
        self.children.deinit(allocator);

        allocator.free(self.node_contents);

        allocator.destroy(self);
    }
};

pub const DIR_TREE = struct {
    const Self = @This();

    root_node: *TREE_NODE,
    tree_nodes: STD.ArrayList(*TREE_NODE),

    // NOTE: https://ziggit.dev/t/cant-work-around-the-error-unable-to-resolve-inferred-error-set/2239/6
    pub fn init(
        self: *Self,
        allocator: STD.mem.Allocator,
        dir_path: []const u8,
    ) anyerror!void {
        self.root_node = try allocator.create(TREE_NODE);

        try self.root_node.init(
            allocator,
            .{ .node_contents = dir_path },
        );

        self.tree_nodes = .empty;

        self.populateTree(allocator, self.root_node) catch {
            STD.debug.print("{s}{s}Failed to populate tree instance{s}", .{
                STYLES.ASCII_STYLES.red,
                STYLES.ASCII_STYLES.underline,
                STYLES.ASCII_STYLES.clear_style,
            });
        };
    }

    fn populateTree(
        self: *Self,
        allocator: STD.mem.Allocator,
        parent_node: *TREE_NODE,
    ) anyerror!void {
        try self.tree_nodes.append(allocator, parent_node);

        _ = STD.fs.cwd().openDir(parent_node.node_contents, .{}) catch {
            return;
        };

        var children_node_contents: STD.ArrayList([]const u8) = .empty;
        defer children_node_contents.deinit(allocator);

        var dir = try STD.fs.cwd().openDir(parent_node.node_contents, .{ .iterate = true });
        defer dir.close();

        var contents_in_dir_iterator = dir.iterate();

        while (try contents_in_dir_iterator.next()) |content| {
            const CONTENT_FILE_PATH: []u8 = try STD.fs.path.join(
                allocator,
                &[_][]const u8{ parent_node.node_contents, content.name },
            );

            try children_node_contents.append(allocator, CONTENT_FILE_PATH);
        }

        STD.sort.insertion(
            []const u8,
            children_node_contents.items,
            {},
            struct {
                fn lessThan(_: void, a: []const u8, b: []const u8) bool {
                    return STD.mem.lessThan(u8, a, b);
                }
            }.lessThan,
        );

        for (children_node_contents.items) |child_content| {
            const CHILD_NODE_INSTANCE: *TREE_NODE = try allocator.create(TREE_NODE);

            try CHILD_NODE_INSTANCE.init(
                allocator,
                .{
                    .parent = parent_node,
                    .node_contents = child_content,
                },
            );

            try parent_node.children.append(allocator, CHILD_NODE_INSTANCE);

            try self.populateTree(allocator, CHILD_NODE_INSTANCE);
        }
    }

    pub fn iterateAndFilterTree(
        self: *Self,
        allocator: STD.mem.Allocator,
        extra_options: struct {
            is_debug_enabled: bool = false,
            include_filter: []const u8 = "",
            exclude_filter: []const u8 = "",
            is_move_semantics_enabled: bool = false,
            external_list: *STD.ArrayList([]const u8) = undefined,
        },
    ) !void {
        var debug_nodes_list: STD.ArrayList([]const u8) = .empty;
        defer debug_nodes_list.deinit(allocator);

        for (self.tree_nodes.items) |node| {
            if (extra_options.include_filter.len == 0 and extra_options.exclude_filter.len == 0) {
                if (extra_options.is_debug_enabled) {
                    STD.debug.print("no filter included -> {s}\n", .{node.node_contents});
                }

                if (extra_options.is_debug_enabled) {
                    try debug_nodes_list.append(allocator, node.node_contents);
                }

                if (extra_options.is_move_semantics_enabled) {
                    try extra_options.external_list.append(allocator, node.node_contents);
                }

                continue;
            }

            if (extra_options.exclude_filter.len > 0 and STD.mem.containsAtLeast(u8, node.node_contents, 1, extra_options.exclude_filter)) {
                if (extra_options.is_debug_enabled) {
                    STD.debug.print("excluding -> {s}\n", .{node.node_contents});
                }

                continue;
            }

            if (extra_options.include_filter.len > 0 and STD.mem.containsAtLeast(u8, node.node_contents, 1, extra_options.include_filter)) {
                if (extra_options.is_debug_enabled) {
                    STD.debug.print("including -> {s}\n", .{node.node_contents});
                }

                if (extra_options.is_debug_enabled) {
                    try debug_nodes_list.append(allocator, node.node_contents);
                }

                if (extra_options.is_move_semantics_enabled) {
                    try extra_options.external_list.append(allocator, node.node_contents);
                }
            }
        }

        if (extra_options.is_debug_enabled) {
            STD.debug.print("\n[\n", .{});

            for (debug_nodes_list.items) |item| {
                STD.debug.print("{s},", .{item});
            }

            STD.debug.print("\n]\n", .{});
        }
    }

    pub fn deinit(
        self: *Self,
        allocator: STD.mem.Allocator,
    ) void {
        for (self.tree_nodes.items) |node| {
            node.deinit(allocator);
        }

        self.tree_nodes.deinit(allocator);
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
    extra_options: struct {
        allow_move_semantics: bool = false,
        args: []const u8 = "echo this is a sub-process",
        is_debug_mode: bool = false,
        move_process_output_to: *STD.ArrayList(u8),
    },
) !u8 {
    const BUFFER_SIZE = comptime 1 << 16;

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
        if (extra_options.is_debug_mode) {
            STD.debug.print("{s}\n", .{process_stderr_buffer.items});
        }

        if (extra_options.allow_move_semantics) {
            try extra_options.move_process_output_to.appendSlice(allocator, process_stderr_buffer.items);
        }

        return PROCESS_STATUS.Exited;
    }

    if (extra_options.is_debug_mode) {
        STD.debug.print("{s}\n", .{process_stdout_buffer.items});
    }

    if (extra_options.allow_move_semantics) {
        try extra_options.move_process_output_to.appendSlice(allocator, process_stdout_buffer.items);
    }

    return PROCESS_STATUS.Exited;
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

test "build and search directory tree" {
    var mem_arena: STD.heap.ArenaAllocator = STD.heap.ArenaAllocator.init(STD.testing.allocator);
    defer mem_arena.deinit();

    const allocator: STD.mem.Allocator = mem_arena.allocator();

    const DIR_TREE_INSTANCE: *DIR_TREE = try allocator.create(DIR_TREE);
    defer DIR_TREE_INSTANCE.deinit(allocator);
    try DIR_TREE_INSTANCE.init(allocator, TEST_DIR);

    var returned_dir_contents: STD.ArrayList([]const u8) = .empty;
    defer returned_dir_contents.deinit(allocator);

    try DIR_TREE_INSTANCE.iterateAndFilterTree(allocator, .{
        .is_debug_enabled = false,
        .include_filter = ".txt",
        .is_move_semantics_enabled = true,
        .external_list = &returned_dir_contents,
    });

    try STD.testing.expect(
        STD.mem.eql(u8, returned_dir_contents.items[0], "tests/test_dir/chapter/test.txt"),
    );
}

test "build and search solutions directory tree" {
    var mem_arena: STD.heap.ArenaAllocator = STD.heap.ArenaAllocator.init(STD.testing.allocator);
    defer mem_arena.deinit();

    var allocator: STD.mem.Allocator = mem_arena.allocator();

    const DIR_TREE_INSTANCE: *DIR_TREE = try allocator.create(DIR_TREE);
    defer DIR_TREE_INSTANCE.deinit(allocator);
    try DIR_TREE_INSTANCE.init(allocator, ".patches/solutions");

    try DIR_TREE_INSTANCE.iterateAndFilterTree(allocator, .{
        .is_debug_enabled = false,
        .include_filter = "src",
    });
}

test "run sub process with default arguments" {
    var mem_arena: STD.heap.ArenaAllocator = STD.heap.ArenaAllocator.init(STD.testing.allocator);
    defer mem_arena.deinit();

    const ALLOCATOR: STD.mem.Allocator = mem_arena.allocator();

    var process_output: STD.ArrayList(u8) = .empty;
    defer process_output.deinit(ALLOCATOR);

    const PROCESS_EXIT_STATUS = try runSubProcess(
        ALLOCATOR,
        .{
            .allow_move_semantics = true,
            .move_process_output_to = &process_output,
        },
    );

    try STD.testing.expect(PROCESS_EXIT_STATUS == 0);

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

    const PROCESS_EXIT_STATUS = try runSubProcess(
        ALLOCATOR,
        .{
            .args = "echo hello world",
            .allow_move_semantics = true,
            .move_process_output_to = &process_output,
        },
    );

    try STD.testing.expect(PROCESS_EXIT_STATUS == 0);

    try STD.testing.expect(
        STD.mem.eql(u8, process_output.items, "hello world\n"),
    );
}


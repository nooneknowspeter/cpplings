const STD = @import("std");

const CLI = @import("cli.zig");
const UTIL = @import("util.zig");
const STYLES = @import("styles.zig");

pub const DEFAULT_ORIGINAL_DIR = "patches";
pub const DEFAULT_MODIFIED_DIR: []const u8 = CLI.DEFAULT_EXERCISES_DIR;
pub const DEFAULT_EXTENSION_FILTER = ".cpp";

pub const PatchSystem = struct {
    const Self = @This();

    list_of_solution_file_paths: STD.ArrayList([]const u8),
    list_of_exercise_file_paths: STD.ArrayList([]const u8),

    pub fn init(allocator: STD.mem.Allocator, extra_options: struct {
        original_files_dir: []const u8 = DEFAULT_ORIGINAL_DIR,
        modified_dir: []const u8 = DEFAULT_MODIFIED_DIR,
        extension_filter: []const u8 = DEFAULT_EXTENSION_FILTER,
        is_debug_enabled: bool = false,
    }) !*PatchSystem {
        STD.debug.print("{s}\n\n", .{CLI.ASCII_ART});

        STD.debug.print("initializing patch system\n", .{});
        const PATCH_SYSTEM: *PatchSystem = try allocator.create(PatchSystem);

        STD.debug.print("populating dirs\n", .{});
        PATCH_SYSTEM.list_of_solution_file_paths = .empty;
        PATCH_SYSTEM.list_of_exercise_file_paths = .empty;

        var ORIGINAL_FILES_TREE: *UTIL.DIR_TREE = try UTIL.DIR_TREE.init(
            allocator,
            extra_options.original_files_dir,
        );
        defer ORIGINAL_FILES_TREE.deinit(allocator);

        try ORIGINAL_FILES_TREE.iterateAndFilterTree(allocator, .{
            .is_debug_enabled = extra_options.is_debug_enabled,
            .include_filter = extra_options.extension_filter,
            .is_move_semantics_enabled = true,
            .external_list = &PATCH_SYSTEM.list_of_solution_file_paths,
        });

        var MODIFIED_FILES_TREE: *UTIL.DIR_TREE = try UTIL.DIR_TREE.init(
            allocator,
            extra_options.modified_dir,
        );
        defer MODIFIED_FILES_TREE.deinit(allocator);

        try MODIFIED_FILES_TREE.iterateAndFilterTree(allocator, .{
            .is_debug_enabled = extra_options.is_debug_enabled,
            .include_filter = extra_options.extension_filter,
            .is_move_semantics_enabled = true,
            .external_list = &PATCH_SYSTEM.list_of_exercise_file_paths,
        });

        STD.debug.print("generating patches\n", .{});
        try PATCH_SYSTEM.generatePatches(allocator, .{
            .extension_filter = extra_options.extension_filter,
            .is_debug_enabled = extra_options.is_debug_enabled,
        });

        return PATCH_SYSTEM;
    }

    fn generatePatches(
        self: *Self,
        allocator: STD.mem.Allocator,
        extra_options: struct {
            extension_filter: []const u8 = DEFAULT_EXTENSION_FILTER,
            is_debug_enabled: bool = false,
        },
    ) !void {
        for (self.list_of_solution_file_paths.items) |solution| {
            const basename = STD.fs.path.basename(solution);

            var matched_exercise: ?[]const u8 = null;
            for (self.list_of_exercise_file_paths.items) |exercise| {
                if (STD.mem.eql(u8, STD.fs.path.basename(exercise), basename)) {
                    matched_exercise = exercise;
                    break;
                }
            }

            const exercise = matched_exercise orelse {
                STD.debug.print("No matching exercise found for {s}\n", .{basename});
                continue;
            };

            const PATCH_FILE_PATH = try STD.mem.replaceOwned(
                u8,
                allocator,
                solution,
                extra_options.extension_filter,
                ".patch",
            );

            try PatchSystem.generatePatch(allocator, solution, exercise, PATCH_FILE_PATH, .{
                .is_debug_enabled = extra_options.is_debug_enabled,
            });
        }
    }

    fn generatePatch(
        allocator: STD.mem.Allocator,
        original_file_path: []const u8,
        modified_file_path: []const u8,
        patch_file_path: []const u8,
        extra_options: struct {
            is_debug_enabled: bool = false,
        },
    ) !void {
        var splitted_string_iterator = STD.mem.splitSequence(u8, patch_file_path, "/");

        var splitted_string_array_list: STD.ArrayList([]const u8) = .empty;
        defer splitted_string_array_list.deinit(allocator);

        while (splitted_string_iterator.next()) |string_component| {
            try splitted_string_array_list.append(allocator, string_component);
        }

        _ = splitted_string_array_list.pop();

        const PATCH_FILE_DIR = try STD.fs.path.join(allocator, splitted_string_array_list.items);

        STD.debug.print("diff -u {s} {s} --> {s}{s}{s}\n", .{
            original_file_path,
            modified_file_path,
            STYLES.ASCII_STYLES.underline,
            patch_file_path,
            STYLES.ASCII_STYLES.clear_style,
        });

        const PROCESS_ARGS = try STD.fmt.allocPrint(allocator, "diff -u {s} {s}", .{
            original_file_path,
            modified_file_path,
        });

        var diff_contents: STD.ArrayList(u8) = .empty;
        defer diff_contents.deinit(allocator);

        try UTIL.runSubProcess(allocator, .{
            .args = PROCESS_ARGS,
            .allow_move_semantics = true,
            .move_process_output_to = &diff_contents,
            .is_debug_enabled = extra_options.is_debug_enabled,
        });

        STD.fs.cwd().access(patch_file_path, .{}) catch {
            try STD.fs.cwd().makePath(PATCH_FILE_DIR);
        };

        try STD.fs.cwd().writeFile(.{
            .sub_path = patch_file_path,
            .data = diff_contents.items,
        });

        STD.debug.print("\n{s}Generated patch -> {s}{s}{s}\n\n", .{
            STYLES.ASCII_STYLES.bold,
            STYLES.ASCII_STYLES.underline,
            patch_file_path,
            STYLES.ASCII_STYLES.clear_style,
        });
    }

    fn patch(
        allocator: STD.mem.Allocator,
        file: []const u8,
        patch_file: []const u8,
        extra_options: struct {
            reverse: bool = true,
            is_debug_enabled: bool = false,
        },
    ) !void {
        var patch_options: []const u8 = "-u";

        if (extra_options.reverse) {
            patch_options = "-u -R";
        }

        const PROCESS_ARGS = try STD.fmt.allocPrint(allocator, "patch {s} {s} {s}", .{
            patch_options,
            file,
            patch_file,
        });

        var process_output: STD.ArrayList(u8) = .empty;
        defer process_output.deinit(allocator);

        try UTIL.runSubProcess(allocator, .{
            .is_debug_enabled = extra_options.is_debug_enabled,
            .args = PROCESS_ARGS,
            .allow_move_semantics = false,
            .move_process_output_to = &process_output,
        });
    }

    pub fn deinit(self: *Self, allocator: STD.mem.Allocator) void {
        self.list_of_exercise_file_paths.deinit(allocator);
        self.list_of_solution_file_paths.deinit(allocator);

        allocator.destroy(self);
    }
};

// tests

const TEST_PATCHES_DIR = "tests/patches";
const ORIGINAL_TESTS_FILES_DIR = TEST_PATCHES_DIR ++ "/" ++ "original";
const ORIGINAL_TEST_FILE_PATH = ORIGINAL_TESTS_FILES_DIR ++ "/" ++ "original.txt";
const MODIFIED_TEST_FILES_DIR = TEST_PATCHES_DIR ++ "/" ++ "modified";
const MODIFIED_TEST_FILE_PATH = MODIFIED_TEST_FILES_DIR ++ "/" ++ "modified.txt";
const PATCH_TEST_FILES_DIR = TEST_PATCHES_DIR ++ "/" ++ "patch";
const PATCH_TEST_FILE_PATH = PATCH_TEST_FILES_DIR ++ "/" ++ "patch.patch";

fn initTest() !void {
    STD.fs.cwd().access(ORIGINAL_TESTS_FILES_DIR, .{}) catch {
        try STD.fs.cwd().makePath(ORIGINAL_TESTS_FILES_DIR);
    };

    STD.fs.cwd().access(MODIFIED_TEST_FILES_DIR, .{}) catch {
        try STD.fs.cwd().makePath(MODIFIED_TEST_FILES_DIR);
    };

    STD.fs.cwd().access(ORIGINAL_TEST_FILE_PATH, .{}) catch {
        try STD.fs.cwd().writeFile(.{ .sub_path = ORIGINAL_TEST_FILE_PATH, .data = "hello", .flags = .{ .truncate = true } });
    };

    STD.fs.cwd().access(MODIFIED_TEST_FILE_PATH, .{}) catch {
        try STD.fs.cwd().writeFile(.{ .sub_path = MODIFIED_TEST_FILE_PATH, .data = "hey", .flags = .{ .truncate = true } });
    };
}

test "generate patch" {
    try initTest();

    var mem_arena: STD.heap.ArenaAllocator = STD.heap.ArenaAllocator.init(STD.testing.allocator);
    defer mem_arena.deinit();

    const ALLOCATOR: STD.mem.Allocator = mem_arena.allocator();

    try PatchSystem.generatePatch(
        ALLOCATOR,
        ORIGINAL_TEST_FILE_PATH,
        MODIFIED_TEST_FILE_PATH,
        PATCH_TEST_FILE_PATH,
        .{},
    );
}

test "patch" {
    try initTest();

    var mem_arena: STD.heap.ArenaAllocator = STD.heap.ArenaAllocator.init(STD.testing.allocator);
    defer mem_arena.deinit();

    const ALLOCATOR = mem_arena.allocator();

    try PatchSystem.patch(ALLOCATOR, MODIFIED_TEST_FILE_PATH, PATCH_TEST_FILE_PATH, .{
        .reverse = true,
    });

    const FILE_CONTENTS = try STD.fs.cwd().readFileAlloc(
        ALLOCATOR,
        MODIFIED_TEST_FILE_PATH,
        STD.math.maxInt(usize),
    );

    try STD.testing.expectEqualSlices(u8, FILE_CONTENTS, "hello");
}

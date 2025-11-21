const STD = @import("std");

const CLI = @import("cli.zig");
const UTIL = @import("util.zig");
const STYLES = @import("styles.zig");

const PatchSystem = struct {
    list_of_original_file_paths: STD.ArrayList([]const u8) = .empty,
    list_of_modified_file_paths: STD.ArrayList([]const u8) = .empty,
};

fn iterateDirs(allocator: STD.mem.Allocator, patch_system: *PatchSystem) !void {
    _ = patch_system;

    try UTIL.iterateDirectory(allocator, ".patches/solutions", .{ .is_print_list_of_contents = true });

}

// TODO: clean up
fn generatePatches(allocator: STD.mem.Allocator, patch_system: *PatchSystem) !void {
    for (patch_system.list_of_exercises.items, patch_system.list_of_solutions.items) |exercise, solution| {
        const BUFFER_SIZE = comptime STD.math.pow(usize, 2, 16);

        // TODO: DRY
        var current_solution_filepath_iterator = STD.mem.splitAny(u8, solution, "/");
        var current_solution_filepath_list: STD.ArrayList([]const u8) = .empty;

        while (current_solution_filepath_iterator.next()) |current_solution_filepath_slice_element| {
            try current_solution_filepath_list.append(allocator, current_solution_filepath_slice_element);
        }

        const TOP_LEVEL_DIR = current_solution_filepath_list.items[0];
        const CHAPTER_DIR = current_solution_filepath_list.items[2];
        const SOLUTION_FILENAME = current_solution_filepath_list.items[3];

        const OUTPUT_FILENAME = try STD.mem.replaceOwned(u8, allocator, SOLUTION_FILENAME, ".cpp", ".patch");

        const OUTPUT_FILEPATH = try STD.fs.path.join(allocator, &[_][]const u8{ TOP_LEVEL_DIR, "patches", CHAPTER_DIR, OUTPUT_FILENAME });

        STD.debug.print("diff {s} {s} --> {s}{s}{s}\n", .{
            exercise,
            solution,
            STYLES.ASCII_STYLES.underline,
            OUTPUT_FILEPATH,
            STYLES.ASCII_STYLES.clear_style,
        });

        // TODO: DRY
        var process_args: STD.ArrayList([]const u8) = .empty;

        try process_args.appendSlice(allocator, &[_][]const u8{ "diff", "-u", exercise, solution });
        var process = STD.process.Child.init(process_args.items, allocator);

        process.stderr_behavior = .Pipe;
        process.stdout_behavior = .Pipe;

        var process_stdout_buffer: STD.ArrayList(u8) = .empty;
        var process_stderr_buffer: STD.ArrayList(u8) = .empty;

        process.spawn() catch {
            STD.debug.print("{s}Failed to generate patches!\n{s}", .{
                STYLES.ASCII_STYLES.red,
                STYLES.ASCII_STYLES.clear_style,
            });
            return;
        };

        try process.collectOutput(allocator, &process_stdout_buffer, &process_stderr_buffer, BUFFER_SIZE);

        const PATCHES_DIR_PATH = try STD.fs.path.join(allocator, &[_][]const u8{ TOP_LEVEL_DIR, "patches" });
        const PATCHES_CHAPTER_DIR_PATH = try STD.fs.path.join(allocator, &[_][]const u8{ TOP_LEVEL_DIR, "patches", CHAPTER_DIR });

        STD.fs.cwd().access(PATCHES_DIR_PATH, .{}) catch {
            try STD.fs.cwd().makeDir(PATCHES_DIR_PATH);
        };

        STD.fs.cwd().access(PATCHES_CHAPTER_DIR_PATH, .{}) catch {
            try STD.fs.cwd().makeDir(PATCHES_CHAPTER_DIR_PATH);
        };

        try STD.fs.cwd().writeFile(.{ .sub_path = OUTPUT_FILEPATH, .data = process_stdout_buffer.items });
    }

    STD.debug.print("\n{s}Generated patches -> {s}./patches/patches{s}\n", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.underline, STYLES.ASCII_STYLES.clear_style });
}

// TODO: implement patch function
fn patch(
    allocator: STD.mem.Allocator,
    file: []const u8,
    patch_file: []const u8,
    extra_options: struct { reverse: bool = true },
) !void {
    _ = extra_options;

    // TODO: DRY
    const BUFFER_SIZE = comptime STD.math.pow(usize, 2, 16);

    var process_args: STD.ArrayList([]const u8) = .empty;

    try process_args.appendSlice(allocator, &[_][]const u8{ "patch", "-u", file, patch_file });
    var process = STD.process.Child.init(process_args.items, allocator);

    process.stderr_behavior = .Pipe;
    process.stdout_behavior = .Pipe;

    var process_stdout_buffer: STD.ArrayList(u8) = .empty;
    var process_stderr_buffer: STD.ArrayList(u8) = .empty;

    process.spawn() catch {
        STD.debug.print("{s}Failed to patch {s}file!\n{s}", .{
            STYLES.ASCII_STYLES.red,
            file,
            STYLES.ASCII_STYLES.clear_style,
        });
        return;
    };

    try process.collectOutput(allocator, &process_stdout_buffer, &process_stderr_buffer, BUFFER_SIZE);
}

pub fn run(allocator: STD.mem.Allocator) !void {
    STD.debug.print("{s}\n\n", .{CLI.ASCII_ART});

    STD.debug.print("Creating patch system...\n", .{});
    const patch_system: *PatchSystem = try allocator.create(PatchSystem);
    defer allocator.destroy(patch_system);

    STD.debug.print("Iterating exercises...\n", .{});
    try iterateDirs(allocator, patch_system);

    // STD.debug.print("Generating patches...\n", .{});
    // try generatePatches(allocator, patch_system);
}

// tests

const TEST_PATCHES_DIR = "tests/patches";
const ORIGINAL_FILES_DIR = TEST_PATCHES_DIR ++ "/" ++ "original";
const ORIGINAL_FILE_PATH = ORIGINAL_FILES_DIR ++ "/" ++ "original.txt";
const MODIFIED_FILES_DIR = TEST_PATCHES_DIR ++ "/" ++ "modified";
const MODIFIED_FILE_PATH = MODIFIED_FILES_DIR ++ "/" ++ "modified.txt";

fn initTest() !void {
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

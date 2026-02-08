const STD = @import("std");

const PATCH_SYSTEM = @import("patch.zig");
const STYLES = @import("styles.zig");
const UTIL = @import("util.zig");

var stdin_buffer: [1024]u8 = undefined;
var stdin_reader = STD.fs.File.stdin().reader(&stdin_buffer);
const STDIN = &stdin_reader.interface;

pub const ASCII_ART =
    \\                   _ _
    \\                  | (_)
    \\   ___ _ __  _ __ | |_ _ __   __ _ ___
    \\  / __| '_ \| '_ \| | | '_ \ / _` / __|
    \\ | (__| |_) | |_) | | | | | | (_| \__ \
    \\  \___| .__/| .__/|_|_|_| |_|\__, |___/
    \\      | |   | |               __/ |
    \\      |_|   |_|              |___/
    \\
;

// FIX:
const CLI = struct {
    const Self = @This();

    did_current_exercise_compile: bool,

    current_exercise: []const u8,
    current_exercise_index: u8,
    current_chapter: []const u8,
    current_patch: []const u8,

    current_exercise_prev_mod_time: i128,

    completed_exercises: STD.ArrayList([]const u8),

    current_exercise_stdout: STD.ArrayList(u8),
    current_exercise_stderr: STD.ArrayList(u8),

    list_of_exercises: STD.ArrayList([]const u8),
    list_of_chapter_support_files: STD.ArrayList([]const u8),
    list_of_patches: STD.ArrayList([]const u8),

    fn init(
        allocator: STD.mem.Allocator,
        extra_options: struct {
            exercises_dir_path: []const u8 = "exercises",
        },
    ) !*CLI {
        const CLI_INSTANCE: *CLI = try allocator.create(CLI);

        CLI_INSTANCE.current_exercise_prev_mod_time = 0;

        CLI_INSTANCE.completed_exercises = .empty;

        CLI_INSTANCE.current_exercise_stderr = .empty;
        CLI_INSTANCE.current_exercise_stdout = .empty;

        CLI_INSTANCE.list_of_exercises = .empty;
        CLI_INSTANCE.list_of_chapter_support_files = .empty;
        CLI_INSTANCE.list_of_patches = .empty;

        try CLI_INSTANCE.iterate(
            allocator,
            .{
                .exercises_dir_path = extra_options.exercises_dir_path,
            },
        );

        return CLI_INSTANCE;
    }

    fn iterate(
        self: *Self,
        allocator: STD.mem.Allocator,
        extra_options: struct {
            exercises_dir_path: []const u8 = "exercises",
        },
    ) !void {
        self.completed_exercises.clearAndFree(allocator);
        self.list_of_exercises.clearAndFree(allocator);
        self.list_of_patches.clearAndFree(allocator);

        const EXERCISES_DIR_TREE: *UTIL.DIR_TREE = try UTIL.DIR_TREE.init(
            allocator,
            extra_options.exercises_dir_path,
        );
        // defer EXERCISES_DIR_TREE.deinit(allocator);

        try EXERCISES_DIR_TREE.iterateAndFilterTree(allocator, .{
            .is_debug_enabled = true,
            .include_filter = ".cpp",
            .exclude_filter = "src",
            .is_move_semantics_enabled = true,
            .external_list = &self.list_of_exercises,
        });

        const PATCHES_DIR_TREE: *UTIL.DIR_TREE = try UTIL.DIR_TREE.init(
            allocator,
            "patches",
        );
        // defer PATCHES_DIR_TREE.deinit(allocator);

        try PATCHES_DIR_TREE.iterateAndFilterTree(allocator, .{
            .is_debug_enabled = true,
            .include_filter = ".patch",
            .exclude_filter = ".cpp",
            .is_move_semantics_enabled = true,
            .external_list = &self.list_of_patches,
        });

        for (0.., self.list_of_exercises.items) |index, exercise| {
            self.current_exercise = exercise;
            self.current_exercise_index = @intCast(index);

            self.current_patch = self.list_of_patches.items[index];

            try self.compileCurrentExercise(allocator);

            if (!(self.did_current_exercise_compile)) {
                break;
            }

            try self.completed_exercises.append(allocator, exercise);
        }
    }

    fn iteratePreviousExercise(
        self: *Self,
        allocator: STD.mem.Allocator,
    ) !void {
        if (self.current_exercise_index <= 0) {
            return;
        }

        _ = self.completed_exercises.pop();

        const PREVIOUS_EXERCISE_INDEX = self.current_exercise_index - 1;

        self.current_exercise = self.list_of_exercises.items[PREVIOUS_EXERCISE_INDEX];
        self.current_exercise_index = @intCast(PREVIOUS_EXERCISE_INDEX);

        self.current_patch = self.list_of_patches.items[PREVIOUS_EXERCISE_INDEX];

        try self.compileCurrentExercise(allocator);
    }

    fn iterateNextExercise(
        self: *Self,
        allocator: STD.mem.Allocator,
    ) !void {
        try self.compileCurrentExercise(allocator);

        if (!(self.did_current_exercise_compile)) {
            return;
        }

        try self.completed_exercises.append(allocator, self.current_exercise);

        const NEXT_EXERCISE_INDEX = self.current_exercise_index + 1;

        self.current_exercise = self.list_of_exercises.items[NEXT_EXERCISE_INDEX];
        self.current_exercise_index = @intCast(NEXT_EXERCISE_INDEX);

        self.current_patch = self.list_of_patches.items[NEXT_EXERCISE_INDEX];

        try self.compileCurrentExercise(allocator);
    }

    fn compileCurrentExercise(
        self: *Self,
        allocator: STD.mem.Allocator,
    ) !void {
        self.current_exercise_stdout.clearAndFree(allocator);
        self.current_exercise_stderr.clearAndFree(allocator);

        if (self.list_of_exercises.items.len == 0) {
            try self.current_exercise_stderr.appendSlice(
                allocator,
                "No exercises found",
            );

            return error.NoExercisesFound;
        }

        try self.current_exercise_stdout.appendSlice(
            allocator,
            "Wait, compiling...",
        );

        try self.clear();
        try self.draw(allocator);

        var process_args: STD.ArrayList([]const u8) = .empty;
        defer process_args.deinit(allocator);

        try process_args.appendSlice(
            allocator,
            &[_][]const u8{
                "zig",
                "build",
                "exercises",
                "--",
                self.current_exercise,
            },
        );

        // self.list_of_chapter_support_files.clearAndFree(allocator);
        //
        // var current_exercise_filepath_iterator = STD.mem.splitAny(u8, self.current_exercise, "/");
        // var current_exercise_filepath_slice: STD.ArrayList([]const u8) = .empty;
        // defer current_exercise_filepath_slice.deinit(allocator);
        //
        // while (current_exercise_filepath_iterator.next()) |current_exercise_filepath_slice_element| {
        //     if (STD.mem.containsAtLeast(u8, current_exercise_filepath_slice_element, 1, ".cpp")) {
        //         break;
        //     }
        //
        //     try current_exercise_filepath_slice.append(
        //         allocator,
        //         current_exercise_filepath_slice_element,
        //     );
        // }
        //
        // self.current_chapter = try STD.fs.path.join(
        //     allocator,
        //     current_exercise_filepath_slice.items,
        // );
        //
        // const CHAPTER_SUPPORT_FILES_DIR_PATH = try STD.fs.path.join(allocator, &[_][]const u8{ self.current_chapter, "src" });
        //
        // const CHAPTER_SUPPORT_FILES_DIR_TREE: *UTIL.DIR_TREE = try UTIL.DIR_TREE.init(allocator, CHAPTER_SUPPORT_FILES_DIR_PATH,);
        // defer CHAPTER_SUPPORT_FILES_DIR_TREE.deinit(allocator);
        //
        // try CHAPTER_SUPPORT_FILES_DIR_TREE.iterateAndFilterTree(allocator, .{
        //     .is_debug_enabled = false,
        //     .include_filter = ".cpp",
        //     .is_move_semantics_enabled = true,
        //     .external_list = &self.list_of_chapter_support_files,
        // });
        //
        // try process_args.appendSlice(allocator, self.list_of_chapter_support_files.items);

        self.current_exercise_stdout.clearAndFree(allocator);
        self.current_exercise_stderr.clearAndFree(allocator);

        UTIL.runSubProcess(
            allocator,
            .{
                .is_debug_enabled = true,
                .args = try STD.mem.concat(
                    allocator,
                    u8,
                    process_args.items,
                ),
                .allow_move_semantics = true,
                .move_process_stdout_to = &self.current_exercise_stdout,
                .move_process_stderr_to = &self.current_exercise_stderr,
            },
        ) catch
            {
                STD.debug.print("\n{s}Error compiling exercise...{s}\n", .{
                    STYLES.ASCII_STYLES.red,
                    STYLES.ASCII_STYLES.clear_style,
                });

                self.did_current_exercise_compile = false;

                return;
            };

        self.did_current_exercise_compile = true;
    }

    fn progressBar(
        self: *Self,
        allocator: STD.mem.Allocator,
    ) !void {
        var ascii_bar: STD.ArrayList(u8) = .empty;

        for (self.completed_exercises.items) |exercise| {
            _ = exercise;

            try ascii_bar.append(allocator, '#');
        }

        try ascii_bar.append(allocator, '>');

        for (self.list_of_exercises.items[self.completed_exercises.items.len..]) |exercise| {
            _ = exercise;

            try ascii_bar.append(allocator, '-');
        }

        STD.debug.print(
            "\nProgress: [{s}] {d}/{d}\n",
            .{
                ascii_bar.items,
                self.current_exercise_index + 1,
                self.list_of_exercises.items.len,
            },
        );

        ascii_bar.clearAndFree(allocator);
    }

    fn draw(
        self: *Self,
        allocator: STD.mem.Allocator,
    ) !void {
        STD.debug.print("{s}\n", .{ASCII_ART});

        STD.debug.print("{s}\n", .{self.current_exercise_stdout.items});
        if (!(self.did_current_exercise_compile)) {
            STD.debug.print("\n{s}", .{self.current_exercise_stderr.items});
        }

        STD.debug.print("{s}", .{STYLES.ASCII_STYLES.bold});
        if (self.did_current_exercise_compile) {
            STD.debug.print(
                "\nExercise completed , move on to the next...\n",
                .{},
            );
        } else {
            STD.debug.print(
                "\nExercise failed to compile . Keep trying, we believe in you.\n",
                .{},
            );
        }
        STD.debug.print("{s}", .{STYLES.ASCII_STYLES.clear_style});

        try self.progressBar(allocator);

        STD.debug.print(
            "Current exercise: {s}{s}{s}{s}\n",
            .{
                STYLES.ASCII_STYLES.bold,
                STYLES.ASCII_STYLES.underline,
                self.current_exercise,
                STYLES.ASCII_STYLES.clear_style,
            },
        );

        STD.debug.print("\n", .{});
        STD.debug.print(
            "{s}n{s}: next / ",
            .{
                STYLES.ASCII_STYLES.bold,
                STYLES.ASCII_STYLES.clear_style,
            },
        );
        STD.debug.print(
            "{s}p{s}: previous / ",
            .{
                STYLES.ASCII_STYLES.bold,
                STYLES.ASCII_STYLES.clear_style,
            },
        );
        STD.debug.print(
            "{s}c{s}: check all / ",
            .{
                STYLES.ASCII_STYLES.bold,
                STYLES.ASCII_STYLES.clear_style,
            },
        );
        STD.debug.print(
            "{s}r{s}: refresh / ",
            .{
                STYLES.ASCII_STYLES.bold,
                STYLES.ASCII_STYLES.clear_style,
            },
        );
        STD.debug.print(
            "{s}x{s}: reset / ",
            .{
                STYLES.ASCII_STYLES.bold,
                STYLES.ASCII_STYLES.clear_style,
            },
        );
        STD.debug.print(
            "{s}q{s}: quit ",
            .{
                STYLES.ASCII_STYLES.bold,
                STYLES.ASCII_STYLES.clear_style,
            },
        );
        STD.debug.print("-> ", .{});
    }

    fn clear(self: *Self) !void {
        _ = self;
        STD.debug.print("{s}", .{STYLES.ASCII_STYLES.clear_prompt});
    }

    fn userInput(
        self: *Self,
        allocator: STD.mem.Allocator,
    ) !void {
        while (true) {
            const INPUT = try STDIN.takeDelimiterExclusive('\n');

            if (STD.mem.eql(u8, INPUT, "n")) {
                try self.iterateNextExercise(allocator);

                try self.clear();
                try self.draw(allocator);
            }

            if (STD.mem.eql(u8, INPUT, "p")) {
                try self.iteratePreviousExercise(allocator);

                try self.clear();
                try self.draw(allocator);
            }

            if (STD.mem.eql(u8, INPUT, "c")) {
                try self.iterate(allocator, .{});

                try self.clear();
                try self.draw(allocator);
            }

            if (STD.mem.eql(u8, INPUT, "r")) {
                try self.compileCurrentExercise(allocator);

                try self.clear();
                try self.draw(allocator);
            }

            if (STD.mem.eql(u8, INPUT, "x")) {
                PATCH_SYSTEM.PatchSystem.patch(
                    allocator,
                    self.current_exercise,
                    self.current_patch,
                    .{},
                ) catch {
                    STD.debug.print("\n{s}Error applying patch to exercise...{s}\n", .{
                        STYLES.ASCII_STYLES.red,
                        STYLES.ASCII_STYLES.clear_style,
                    });
                };

                try self.clear();
                try self.draw(allocator);
            }

            if (STD.mem.eql(u8, INPUT, "q")) {
                try self.clear();

                break;
            }
        }
    }

    fn deinit(self: *Self, allocator: STD.mem.Allocator) void {
        self.completed_exercises.deinit(allocator);

        self.current_exercise_stderr.deinit(allocator);
        self.current_exercise_stdout.deinit(allocator);

        self.list_of_exercises.deinit(allocator);
        self.list_of_chapter_support_files.deinit(allocator);
        self.list_of_patches.deinit(allocator);

        allocator.destroy(self);
    }
};

fn watchFileChangesDaemon(
    allocator: STD.mem.Allocator,
    cli_instance: *CLI,
    polling_rate_ms: u64,
) !void {
    while (true) {
        const CURRENT_EXERCISE_METADATA = STD.fs.cwd().statFile(cli_instance.current_exercise) catch |err| {
            if (err == error.FileNotFound) {
                STD.Thread.sleep(@intCast(polling_rate_ms * 1_000_000));

                continue;
            }

            return err;
        };
        const CURRENT_EXERCISE_MODIFIED_TIME = CURRENT_EXERCISE_METADATA.mtime;

        if (CURRENT_EXERCISE_MODIFIED_TIME != cli_instance.current_exercise_prev_mod_time) {
            cli_instance.current_exercise_prev_mod_time = CURRENT_EXERCISE_MODIFIED_TIME;

            try cli_instance.compileCurrentExercise(allocator);

            try cli_instance.clear();
            try cli_instance.draw(allocator);
        }

        STD.Thread.sleep(@intCast(polling_rate_ms * 1_000_000));
    }
}

pub fn run(
    allocator: STD.mem.Allocator,
    extra_options: struct {
        exercises_dir_path: []const u8 = "exercises",
    },
) !void {
    const CLI_INSTANCE: *CLI = try CLI.init(
        allocator,
        .{
            .exercises_dir_path = extra_options.exercises_dir_path,
        },
    );
    defer CLI_INSTANCE.deinit(allocator);

    try CLI_INSTANCE.clear();
    try CLI_INSTANCE.draw(allocator);

    // // TODO: implement external mutex
    // const watch_daemon_polling_rate: u64 = 500; // ms
    // var watch_thread: STD.Thread = try STD.Thread.spawn(
    //     .{},
    //     watchFileChangesDaemon,
    //     .{
    //         allocator,
    //         CLI_INSTANCE,
    //         watch_daemon_polling_rate,
    //     },
    // );
    // watch_thread.detach();

    try CLI_INSTANCE.userInput(allocator);
}


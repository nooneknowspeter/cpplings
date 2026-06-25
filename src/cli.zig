const STD = @import("std");

const STYLES = @import("styles.zig");
const EXERCISE_RUNNER = @import("exercise_runner.zig");
const EXERCISE_ITERATOR = @import("exercise_iterator.zig");
const FILE_WATCHER = @import("file_watcher.zig");
const PATCH_SYSTEM = @import("patch.zig");
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

pub const WATCH_POLLING_RATE_MS: u64 = 500;
pub const PROCESS_BUFFER_SIZE: usize = 1 << 16;
pub const DEFAULT_EXERCISES_DIR = "exercises";

pub fn help_documentation() void {
    STD.debug.print("{s}\n", .{ASCII_ART});
    STD.debug.print("cpplings [OPTIONS]\n\n", .{});
    STD.debug.print("Options:\n", .{});
    STD.debug.print("-h | --help        -> output this help documentation\n", .{});
    STD.debug.print("-l | --list        -> list all exercises\n", .{});
    STD.debug.print("-p | --patch       -> generate patch files\n", .{});
    STD.debug.print("-s | --solutions   -> run and test cpplings against solutions dir\n", .{});
    STD.debug.print("-c | --compiler    -> compiler to use: zig (default), g++, clang++\n", .{});
    STD.debug.print("-d | --debug       -> enable debug output\n", .{});
    STD.debug.print("-H | --hint        -> show hint for an exercise\n", .{});
    STD.debug.print("-r | --reset       -> reset an exercise to its original state\n", .{});
    STD.debug.print("--manual-run      -> disable auto-recompile on file change\n", .{});
}

const CLI = struct {
    allocator: STD.mem.Allocator,
    exercise_iterator: *EXERCISE_ITERATOR.ExerciseIterator,
    exercise_runner: EXERCISE_RUNNER.ExerciseRunner,
    debug_enabled: bool,
    manual_run: bool,
};

fn progressBar(self: *CLI) !void {
    var ascii_bar: STD.ArrayList(u8) = .empty;
    defer ascii_bar.deinit(self.allocator);

    for (self.exercise_iterator.completed_exercises.items) |_| {
        try ascii_bar.append(self.allocator, '#');
    }

    try ascii_bar.append(self.allocator, '>');

    const REMAINING = self.exercise_iterator.total() - self.exercise_iterator.progress();

    for (0..REMAINING) |_| {
        try ascii_bar.append(self.allocator, '-');
    }

    STD.debug.print("\nProgress: [{s}] {d}/{d}\n", .{
        ascii_bar.items,
        self.exercise_iterator.progress() + 1,
        self.exercise_iterator.total(),
    });
}

fn draw(self: *CLI) !void {
    STD.debug.print("{s}\n", .{ASCII_ART});

    STD.debug.print("{s}\n", .{self.exercise_runner.current_exercise_stdout.items});
    if (!self.exercise_runner.did_current_exercise_compile) {
        STD.debug.print("\n{s}", .{self.exercise_runner.current_exercise_stderr.items});
    }

    STD.debug.print("{s}", .{STYLES.ASCII_STYLES.bold});
    if (self.exercise_runner.did_current_exercise_compile) {
        STD.debug.print("\nExercise completed, move on to the next...\n", .{});
    } else {
        STD.debug.print("\nExercise failed to compile. Keep trying, we believe in you.\n", .{});
    }
    STD.debug.print("{s}", .{STYLES.ASCII_STYLES.clear_style});

    try progressBar(self);

    if (self.exercise_iterator.current()) |current| {
        STD.debug.print("Current exercise: {s}{s}{s}{s}\n", .{
            STYLES.ASCII_STYLES.bold,
            STYLES.ASCII_STYLES.underline,
            current,
            STYLES.ASCII_STYLES.clear_style,
        });
    }

    STD.debug.print("\n", .{});
    STD.debug.print("{s}n{s}: next / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}p{s}: previous / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}c{s}: check all / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}r{s}: refresh / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}x{s}: reset / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}h{s}: hint / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}l{s}: list / ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("{s}q{s}: quit ", .{ STYLES.ASCII_STYLES.bold, STYLES.ASCII_STYLES.clear_style });
    STD.debug.print("-> ", .{});
}

fn clear() void {
    STD.debug.print("{s}", .{STYLES.ASCII_STYLES.clear_prompt});
}

fn compileCurrentExercise(self: *CLI) !void {
    const current_exercise = self.exercise_iterator.current() orelse return;

    try self.exercise_runner.current_exercise_stdout.appendSlice(self.allocator, "Wait, compiling...");

    clear();
    try draw(self);

    var support_files = try self.exercise_runner.getChapterSupportFiles(self.allocator, current_exercise);
    defer support_files.deinit(self.allocator);

    try self.exercise_runner.compile(self.allocator, current_exercise, support_files.items);
}

fn userInput(self: *CLI) !void {
    while (true) {
        const INPUT = try STDIN.takeDelimiterExclusive('\n');

        if (STD.mem.eql(u8, INPUT, "n")) {
            try self.exercise_iterator.next();
            try compileCurrentExercise(self);

            clear();
            try draw(self);
        }

        if (STD.mem.eql(u8, INPUT, "p")) {
            self.exercise_iterator.prev();
            try compileCurrentExercise(self);

            clear();
            try draw(self);
        }

        if (STD.mem.eql(u8, INPUT, "c")) {
            self.exercise_iterator.reset();
            while (self.exercise_iterator.current()) |_| {
                try compileCurrentExercise(self);
                if (!self.exercise_runner.did_current_exercise_compile) {
                    break;
                }
                try self.exercise_iterator.next();
            }

            clear();
            try draw(self);
        }

        if (STD.mem.eql(u8, INPUT, "r")) {
            try compileCurrentExercise(self);

            clear();
            try draw(self);
        }

        if (STD.mem.eql(u8, INPUT, "x")) {
            self.exercise_iterator.reset();
            clear();
            try draw(self);
        }

        if (STD.mem.eql(u8, INPUT, "h")) {
            if (self.exercise_iterator.current()) |current| {
                try showHint(self.allocator, current);
            }
        }

        if (STD.mem.eql(u8, INPUT, "l")) {
            try list(self.allocator);
        }

        if (STD.mem.eql(u8, INPUT, "q")) {
            clear();
            break;
        }
    }
}

fn watchFileChanges(self: *CLI) !void {
    while (true) {
        const current_exercise = self.exercise_iterator.current() orelse break;

        var watcher = FILE_WATCHER.FileWatcher.init(current_exercise);
        const has_changes = watcher.checkForChanges() catch break;

        if (has_changes) {
            try compileCurrentExercise(self);
            clear();
            try draw(self);
        }

        STD.Thread.sleep(@intCast(WATCH_POLLING_RATE_MS * 1_000_000));
    }
}

pub fn run(
    allocator: STD.mem.Allocator,
    options: struct {
        exercises_dir_path: []const u8 = DEFAULT_EXERCISES_DIR,
        debug_enabled: bool = false,
        manual_run: bool = false,
        exercise_runner: ?EXERCISE_RUNNER.ExerciseRunner = null,
    },
) !void {
    const EXERCISE_ITERATOR_INSTANCE = try EXERCISE_ITERATOR.ExerciseIterator.init(
        allocator,
        options.exercises_dir_path,
    );
    errdefer EXERCISE_ITERATOR_INSTANCE.deinit();

    var exercise_runner = options.exercise_runner orelse EXERCISE_RUNNER.ExerciseRunner.init();
    if (options.exercise_runner == null) {
        errdefer exercise_runner.deinit(allocator);
    }

    var cli = CLI{
        .allocator = allocator,
        .exercise_iterator = EXERCISE_ITERATOR_INSTANCE,
        .exercise_runner = exercise_runner,
        .debug_enabled = options.debug_enabled,
        .manual_run = options.manual_run,
    };

    try compileCurrentExercise(&cli);

    clear();
    try draw(&cli);

    if (!options.manual_run) {
        var watch_thread: STD.Thread = try STD.Thread.spawn(.{}, watchFileChanges, .{&cli});
        watch_thread.detach();
    }

    try userInput(&cli);
}

pub fn list(allocator: STD.mem.Allocator) !void {
    STD.debug.print("{s}\n", .{ASCII_ART});
    STD.debug.print("All exercises:\n\n", .{});

    const EXERCISE_ITERATOR_INSTANCE = try EXERCISE_ITERATOR.ExerciseIterator.init(
        allocator,
        DEFAULT_EXERCISES_DIR,
    );
    defer EXERCISE_ITERATOR_INSTANCE.deinit();

    for (EXERCISE_ITERATOR_INSTANCE.all_exercises.items, 0..) |exercise, i| {
        STD.debug.print("{s}{d}{s}. {s}\n", .{
            STYLES.ASCII_STYLES.bold,
            i + 1,
            STYLES.ASCII_STYLES.clear_style,
            exercise,
        });
    }

    STD.debug.print("\nTotal: {d} exercises\n", .{EXERCISE_ITERATOR_INSTANCE.total()});
}

pub fn showHint(allocator: STD.mem.Allocator, exercise_path: []const u8) !void {
    _ = allocator;
    STD.debug.print("{s}\n", .{ASCII_ART});
    STD.debug.print("{s}Hint for: {s}{s}\n\n", .{
        STYLES.ASCII_STYLES.bold,
        exercise_path,
        STYLES.ASCII_STYLES.clear_style,
    });

    STD.debug.print("Look at the TODO comments in the exercise file.\n", .{});
    STD.debug.print("Check the learncpp.com references for more information.\n", .{});
    STD.debug.print("If stuck, compare with the solution file.\n\n", .{});
}

pub fn reset(allocator: STD.mem.Allocator, exercise_path: []const u8) !void {
    STD.debug.print("{s}\n", .{ASCII_ART});
    STD.debug.print("{s}Resetting: {s}{s}\n\n", .{
        STYLES.ASCII_STYLES.bold,
        exercise_path,
        STYLES.ASCII_STYLES.clear_style,
    });

    const BASENAME = STD.fs.path.basename(exercise_path);
    const SOLUTIONS_PATH = try STD.fmt.allocPrint(allocator, "patches/solutions/{s}", .{BASENAME});
    defer allocator.free(SOLUTIONS_PATH);

    STD.debug.print("Would reset {s} to original state.\n", .{BASENAME});
    STD.debug.print("This feature requires the patch utility.\n", .{});
}


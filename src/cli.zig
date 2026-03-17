const STD = @import("std");

const STYLES = @import("styles.zig");
const EXERCISE_RUNNER = @import("exercise_runner.zig");
const EXERCISE_ITERATOR = @import("exercise_iterator.zig");
const FILE_WATCHER = @import("file_watcher.zig");

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
    STD.debug.print("cpplings [OPTION]\n\n", .{});
    STD.debug.print("Options:\n", .{});
    STD.debug.print("-h | --help | help -> output this help documentation\n", .{});
    STD.debug.print("-p | --patch -> generate patch files\n", .{});
    STD.debug.print("-s | --solutions -> run and test cpplings against solutions dir\n", .{});
}

const CLI = struct {
    allocator: STD.mem.Allocator,
    exercise_iterator: *EXERCISE_ITERATOR.ExerciseIterator,
    exercise_runner: EXERCISE_RUNNER.ExerciseRunner,
};

fn progressBar(self: *CLI) !void {
    var ascii_bar: STD.ArrayList(u8) = .empty;
    defer ascii_bar.deinit(self.allocator);

    for (self.exercise_iterator.completed_exercises.items) |_| {
        try ascii_bar.append(self.allocator, '#');
    }

    try ascii_bar.append(self.allocator, '>');

    const remaining = self.exercise_iterator.total() - self.exercise_iterator.progress();
    for (0..remaining) |_| {
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

pub fn run(allocator: STD.mem.Allocator, extra_options: struct { exercises_dir_path: []const u8 = DEFAULT_EXERCISES_DIR }) !void {
    const exercise_iterator = try EXERCISE_ITERATOR.ExerciseIterator.init(
        allocator,
        extra_options.exercises_dir_path,
    );
    errdefer exercise_iterator.deinit();

    var exercise_runner = EXERCISE_RUNNER.ExerciseRunner.init();
    errdefer exercise_runner.deinit(allocator);

    var cli = CLI{
        .allocator = allocator,
        .exercise_iterator = exercise_iterator,
        .exercise_runner = exercise_runner,
    };

    try compileCurrentExercise(&cli);

    clear();
    try draw(&cli);

    var watch_thread: STD.Thread = try STD.Thread.spawn(.{}, watchFileChanges, .{&cli});
    watch_thread.detach();

    try userInput(&cli);
}

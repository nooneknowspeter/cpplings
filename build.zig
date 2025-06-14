const std = @import("std");

pub fn build(b: *std.Build) !void {
    // const splash_screen =
    //     \\                   _ _
    //     \\                  | (_)
    //     \\   ___ _ __  _ __ | |_ _ __   __ _ ___
    //     \\  / __| '_ \| '_ \| | | '_ \ / _` / __|
    //     \\ | (__| |_) | |_) | | | | | | (_| \__ \
    //     \\  \___| .__/| .__/|_|_|_| |_|\__, |___/
    //     \\      | |   | |               __/ |
    //     \\      |_|   |_|              |___/
    //     \\
    //     \\               compiling...
    // ;

    // std.debug.print("{s}", .{splash_screen});

    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const allocator = std.heap.page_allocator;

    const exe = b.addExecutable(.{ .name = "cpplings", .target = target, .optimize = optimize, .link_libc = true });

    // main.cpp
    exe.addCSourceFile(.{ .file = b.path("src/main.cpp") });

    // link C/C++ std libraries
    exe.linkLibC();
    exe.linkLibCpp();

    // add include path
    exe.addIncludePath(b.path("include"));

    // add source files
    // chapters
    const chapter_dirs = try findSourceFiles(allocator, "exercises/", "", false);

    // find exercises in each chapter folder
    for (chapter_dirs) |chapter_dir| {
        // std.debug.print("\n{s}\n-------------\n", .{chapter_dir});

        const exercises = try findSourceFiles(allocator, chapter_dir, "cpp", true);

        // add exercise source file
        for (exercises) |exercise| {
            // std.debug.print("{s}\n", .{exercise});
            exe.addCSourceFile(.{ .file = b.path(exercise) });
        }
    }

    // linking
    b.installArtifact(exe);

    // run command
    const run_exe = b.addRunArtifact(exe);
    const run_step = b.step("run", "run cpplings");
    run_step.dependOn(&run_exe.step);
}

/// find source files in directory
pub fn findSourceFiles(
    /// allocator
    allocator: std.mem.Allocator,
    /// string literal of directory containing source files
    directory: anytype,
    /// extension of file
    extension: anytype,
    /// toggle filtering by extension
    toggle_filtering_files: bool,
) ![][]const u8 {
    // _ = toggle_filtering_files;

    var files = std.ArrayList([]const u8).init(allocator);

    const dir = try std.fs.cwd().openDir(directory, .{ .iterate = true });

    var iter = dir.iterate();

    while (try iter.next()) |entry| {
        if (!toggle_filtering_files) {
            try files.append(try std.mem.concat(allocator, u8, &[_][]const u8{ directory, entry.name, "/" }));
        } else {
            if (std.mem.containsAtLeast(u8, entry.name, 1, extension)) {
                // try files.append(entry.name);
                try files.append(try std.mem.concat(allocator, u8, &[_][]const u8{ directory, entry.name }));
            }
        }
    }

    return files.items;
}

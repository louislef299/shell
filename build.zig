const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const lsh = b.addExecutable(.{
        .name = "lsh",
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    lsh.addCSourceFiles(.{ .files = &.{
        "src/disk.c",
        "src/fs.c",
        "src/main.c",
        "src/shell.c",
    }, .flags = &.{
        "-Wall",
        "-c",
        "-g",
    } });
    b.installArtifact(lsh);
}

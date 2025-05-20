const std = @import("std");

pub fn build(b: *std.Build) void {
    const make_step = b.addSystemCommand(&[_][]const u8{
        "make", "default",
    });
    const default = b.step("default", "Run the default make step");
    default.dependOn(&make_step.step);
}

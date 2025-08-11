// swift-tools-version: 5.9
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "SwiftKFD",
    products: [
        .library(
            name: "SwiftKFD",
            targets: ["SwiftKFD"]),
    ],
    targets: [
        .target(
            name: "SwiftKFD",
            dependencies: ["SwiftKFD_objc"],
            path: "Sources/SwiftKFD"
        ),
        .target(
            name: "SwiftKFD_objc",
            dependencies: ["libjailbreak_objc"],
            path: "Sources/SwiftKFD_objc",
            publicHeadersPath: "."
        ),
        .target(
            name: "libjailbreak_objc",
            dependencies: ["libarchive", "lz4", "xz", "zlib", "bz2", "iconv"],
            path: "Sources/libjailbreak_objc",
            publicHeadersPath: "."
        ),
        .binaryTarget(
            name: "libarchive",
            path: "ios/xcframeworks/libarchive.xcframework"
        ),
        .binaryTarget(
            name: "lz4",
            path: "ios/xcframeworks/lz4.xcframework"
        ),
        .binaryTarget(
            name: "xz",
            path: "ios/xcframeworks/xz.xcframework"
        ),
        .systemLibrary(
            name: "zlib",
            path: "Sources/zlib",
            pkgConfig: "zlib"
        ),
        .systemLibrary(
            name: "bz2",
            pkgConfig: "bzip2"
        ),
        .systemLibrary(
            name: "iconv"
        ),
    ]
)

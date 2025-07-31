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
            path: "Sources/SwiftKFD"),
        .target(
            name: "SwiftKFD_objc",
            dependencies: ["libjailbreak_objc"],
            path: "Sources/SwiftKFD_objc",
            publicHeadersPath: "."),
        .target(
            name: "libjailbreak_objc",
            dependencies: [],
            path: "Sources/libjailbreak_objc",
            publicHeadersPath: "."),
    ]
)

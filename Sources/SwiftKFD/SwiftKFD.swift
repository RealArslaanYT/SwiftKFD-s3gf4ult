import SwiftKFD_objc
#if os(iOS) || os(iPadOS)
import UIKit
#endif

public class deviceInfo {
    enum DeviceFamily: Int {
        case unknown
        case iOS
        case iPadOS
        case macOS
        case watchOS
        case tvOS
    }
    
    public enum KFDType: Int {
        case landa
        case smith
        case incompatible
    }
    
    static func getVersionArray() -> [Int] {
        #if os(iOS) || os(iPadOS)
        let systemVersion = UIDevice.current.systemVersion
        let versionComponents = systemVersion.split(separator: ".").compactMap { Int($0) }
        if versionComponents.count >= 2 {
            let major = versionComponents[0]
            let sub = versionComponents[1]
            let minor = versionComponents.count >= 3 ? versionComponents[2] : 0
            return [major, sub, minor]
        } else {
            return [99, 0, 0]
        }
        #else
        let processInfo = ProcessInfo.processInfo
        let systemVersion = processInfo.operatingSystemVersionString
        let versionComponents = systemVersion.split(separator: ".").compactMap { Int($0) }
        var version = [99, 0, 0] // 99 to prevent accidental exploit lol
        if versionComponents.count >= 1 {
            version[0] = versionComponents[0]
        }
        if versionComponents.count >= 2 {
            version[1] = versionComponents[1]
        }
        if versionComponents.count >= 3 {
            version[2] = versionComponents[2]
        }
        return version
        #endif
    }
    
    static func getDeviceType() -> DeviceFamily {
        #if os(iOS)
        return .iOS
        #elseif os(iPadOS)
        return .iPadOS
        #elseif os(macOS)
        return .macOS
        #elseif os(watchOS)
        return .watchOS
        #elseif os(tvOS)
        return .tvOS
        #else
        return .unknown
        #endif
    }
    
    static func getKFDType() -> KFDType {
        let deviceType = self.getDeviceType()
        let versionArray = self.getVersionArray()
        NSLog("\(deviceType)")
        NSLog("\(versionArray)")
        if deviceType == .iOS || deviceType == .iPadOS {
            if versionArray.lexicographicallyPrecedes([16, 7, 0]) {
                return .landa
            } else {
                return .incompatible
            }
        } else if deviceType == .macOS {
            if versionArray.lexicographicallyPrecedes([13, 5, 2]) {
                return .smith
            } else if versionArray.lexicographicallyPrecedes([14, 0, 0]) {
                return .landa
            } else {
                return .incompatible
            }
        } else if deviceType == .watchOS {
            if versionArray.lexicographicallyPrecedes([8, 8, 1]) || (versionArray.lexicographicallyPrecedes([9, 5, 2]) &&
                                                                     versionArray.lexicographicallyPrecedes([9, 5, 2]) &&
                                                                     versionArray.lexicographicallyPrecedes([9, 0, 0])) {
                return .smith
            } else if versionArray.lexicographicallyPrecedes([10, 0, 0]) {
                return .landa
            } else {
                return .incompatible
            }
        } else if deviceType == .tvOS {
            if versionArray.lexicographicallyPrecedes([17, 3, 0]) {
                return .landa
            } else {
                return .incompatible
            }
        } else {
            return .incompatible
        }
    }
    
    static func getPuafPages() -> UInt64 {
        let deviceType = self.getDeviceType()
        if deviceType == .iOS {
            return 3072
        } else if deviceType == .iPadOS || deviceType == .macOS {
            return 4096
        } else {
            return 2048
        }
    }
}

extension String: LocalizedError {
    public var errorDescription: String? { return self }
}

var kopened = false

public func SmartKopen(_ puaf_pages: UInt64? = nil, _ puaf_method: UInt64? = nil, _ headroom: Int = -1, _ forcekfd: Bool = false) throws {
    let kfdtype = deviceInfo.getKFDType()
    if kfdtype != .incompatible || forcekfd {
        do_kopen(puaf_pages ?? deviceInfo.getPuafPages(), puaf_method ?? 2, 1, 1, headroom)
        kopened = true
    } else {
        throw "Unsupported Version"
    }
}

public func SmartKclose() {
    // just a proxy to kclose lol
    if kopened {
        do_kclose()
        kopened = false
    }
}

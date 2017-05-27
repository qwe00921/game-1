//
//  FileUtlis.swift
//  AppResignerTRGAME
//
//  Created by apache on 15/2/28.
//  Copyright (c) 2015年 shenliming. All rights reserved.
//

import Foundation

func getChannelDict() -> NSDictionary? {
    let filepath = Bundle.main.resourcePath! + "/channels_config.plist"
    let dict = NSDictionary(contentsOfFile: filepath)
    return dict
}

func getBundleIDs() -> NSDictionary? {
    let filepath = Bundle.main.resourcePath! + "/bundleIdList.plist"
    let dict = NSDictionary(contentsOfFile: filepath)
    return dict
}

func getGamesConfig() -> NSDictionary? {
    let filepath = Bundle.main.resourcePath! + "/games_config.plist"
    let dict = NSDictionary(contentsOfFile: filepath)
    return dict
}

func getGamesHaimaConfig() -> NSDictionary? {
    let filepath = Bundle.main.resourcePath! + "/games_config_haima.plist"
    let dict = NSDictionary(contentsOfFile: filepath)
    return dict
}


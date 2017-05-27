//
//  ChannelListTableViewCell.swift
//  AppResignerTRGAME
//
//  Created by apache on 15/2/27.
//  Copyright (c) 2015年 shenliming. All rights reserved.
//

import Cocoa

class ChannelListTableViewCell: NSTableCellView {
   // @IBOutlet var view:NSView!
    @IBOutlet var button:NSButton!
    @IBOutlet var detailLabel:NSTextField!


//    override func drawRect(dirtyRect: NSRect) {
//        super.drawRect(dirtyRect)
//
//        // Drawing code here.
//    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
//        NSBundle.mainBundle().loadNibNamed("ChannelListTableViewCell", owner: self, topLevelObjects: nil)
//        self.view.frame = self.bounds
//        self.addSubview(view)
        
    }
    
    override init(frame frameRect: NSRect) {
        super.init(frame: frameRect)
//        NSBundle.mainBundle().loadNibNamed("ChannelListTableViewCell", owner: self, topLevelObjects: nil)
//        self.view.frame = self.bounds
//        self.addSubview(view)
    }
    
}

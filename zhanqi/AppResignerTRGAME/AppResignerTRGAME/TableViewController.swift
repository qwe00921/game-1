//
//  TableViewController.swift
//  AppResignerTRGAME
//
//  Created by shenliming on 15/2/11.
//  Copyright (c) 2015年 shenliming. All rights reserved.
//

import Cocoa


extension ViewController: NSTableViewDataSource, NSTableViewDelegate {
    
    func numberOfRows(in tableView: NSTableView) -> Int {
        if channel_config == nil {
            return 0;
        }
        return (channel_config["channels"] as! NSArray).count
    }
    
    
    func tableView(_ tableView: NSTableView, shouldSelectRow row: Int) -> Bool {
        return false
    }
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {

//        let button   = NSButton();
//        button.setButtonType(NSButtonType.SwitchButton)//.buttonWithType(NSButtonType.System) as NSButton
//        button.frame = CGRectMake(100, 100, 100, 200)
//    
//       // button. = NSColor.greenColor()
//        button.title = "aaa" + String(row)
//        button.action = Selector("pushed:");
//        button.target = self
//        //button.addTarget(self, action: "buttonAction:", forControlEvents: NSControlEvents.TouchUpInside)
//        return button;
        
//        let cell = ChannelListTableViewCell(frame: NSMakeRect(0, 0, 200, 25))
//        return cell
        
        let view =  tableView.make(withIdentifier: "abc", owner: self) as! ChannelListTableViewCell
        let channel = (channel_config["channels"] as! NSArray)[row] as! NSDictionary
        view.button.title = channel["channel_name"] as! String
        view.button.tag = row
        view.button.action = #selector(ViewController.pushed(_:))
        view.button.target = self
        view.detailLabel.stringValue = channel["channel_id"] as! String
        return view
    }
    
    func pushed(_ button:NSButton) -> Void{
        print("in pushed" + button.title)
        if button.state == NSOnState {
            active_channels.add((channel_config["channels"] as! NSArray)[button.tag])
        } else {
            active_channels.remove((channel_config["channels"] as! NSArray)[button.tag])
        }
    }
}

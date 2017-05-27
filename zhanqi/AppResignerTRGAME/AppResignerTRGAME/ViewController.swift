//
//  ViewController.swift
//  AppResignerTRGAME
//
//  Created by shenliming on 15/2/10.
//  Copyright (c) 2015年 shenliming. All rights reserved.
//

import Cocoa

var channel_config:NSDictionary!
var games_config:NSDictionary!
var bundleIdList:NSDictionary!
var active_channels:NSMutableSet = NSMutableSet()
let logBuffer:NSMutableData = NSMutableData()
var appdir:String! = nil
var oriInfoPlist:NSDictionary! = nil
var bundle_id:String! = nil
var profileName:String = ""
var newExeName:String = ""

class ViewController: NSViewController {
    
    @IBOutlet weak var startRepackBtn: NSButton!
    @IBOutlet var resNeedDeletePaths: NSTextView!
    @IBOutlet var resNeedEncPaths: NSTextView!
    @IBOutlet var resNormalPaths: NSTextView!
    @IBOutlet weak var workingSpinner: NSProgressIndicator!
    
    @IBOutlet weak var packageNameBox: NSPopUpButton!
    @IBOutlet weak var versionInfoTextView: NSTextField!
    @IBOutlet weak var versionInfoEnableBtn: NSButton!
    @IBOutlet weak var buildNumberTextView: NSTextField!
    @IBOutlet weak var buildNumberEnableBtn: NSButton!
    @IBOutlet var logWindow: NSTextView!
    @IBOutlet weak var channels_tableview: NSTableView!
    @IBOutlet weak var haimaEnableButton: NSButton!

    @IBOutlet weak var IPAFilePath: NSTextField!
    
    @IBOutlet weak var KTPlayListBox: NSPopUpButton!
    @IBAction func versionInfoEnableEditPushed(_ btn: NSButton) {
        if btn.state == NSOnState {
            versionInfoTextView.isEnabled = true
        } else {
            versionInfoTextView.isEnabled = false
        }
    }
    @IBAction func buildNumberEnableEditPushed(_ btn: NSButton) {
        if btn.state == NSOnState {
            buildNumberTextView.isEnabled = true
        } else {
            buildNumberTextView.isEnabled = false
        }
    }
    @IBAction func haimaEnableButtonPushed(_ btn: NSButton) {
        if btn.state == NSOnState {
            games_config = getGamesHaimaConfig()
        } else {
            games_config = getGamesConfig()
        }
        versionInfoTextView.isEnabled = false
        buildNumberTextView.isEnabled = false
        startRepackBtn.isEnabled = false
        packageNameBox.removeAllItems()
        packageNameBox.addItem(withTitle: "不修改")
        let gameList = games_config["games"] as! [NSDictionary]
        for i in gameList {
            packageNameBox.addItem(withTitle: ((i["game_special_infoplist"] as! NSDictionary)["CFBundleDisplayName"] as! String) + ",  " + ((i["game_special_infoplist"] as! NSDictionary)["CFBundleIdentifier"] as! String))
        }
        KTPlayListBox.removeAllItems()
        KTPlayListBox.addItems(withTitles: ["关闭","打开"])
    }

    @IBAction func selectIPAFilePushed(_ sender: AnyObject) {
        let myFiledialog: NSOpenPanel = NSOpenPanel()
        
        myFiledialog.prompt = "选择此IPA文件"
        myFiledialog.worksWhenModal = true
        myFiledialog.allowsMultipleSelection = false
        myFiledialog.canChooseDirectories = false
        myFiledialog.resolvesAliases = true
        //        myFiledialog.title = "windows title"
        //        myFiledialog.message = "message"
        myFiledialog.allowedFileTypes = ["ipa"]
        
        myFiledialog.beginSheetModal(for: self.view.window!){
            (result) -> () in
            if result == NSFileHandlingPanelOKButton {
                self.IPAFilePath.stringValue = myFiledialog.url!.path
            }
        }
    }
    
    @IBAction func startUnpack(_ sender: AnyObject) {
        startWorking()
        logBuffer.length = 0
        let task = Process()
        task.launchPath = Bundle.main.resourcePath! + "/unpackipa.sh"
        task.arguments = [IPAFilePath.stringValue]
        let pipe = Pipe()
        task.standardOutput = pipe
        task.standardError = pipe
        NotificationCenter.default.addObserver(self, selector: #selector(ViewController.gotLog(_:)), name: NSNotification.Name.NSFileHandleDataAvailable, object: pipe.fileHandleForReading)
        pipe.fileHandleForReading.waitForDataInBackgroundAndNotify()
        
        // NSNotificationCenter.defaultCenter().addObserver(self, selector: Selector("taskEnded:"), name: NSTaskDidTerminateNotification, object: task)
        task.terminationHandler = {
            (task:Process) in
            self.endWorking()
            if task.terminationReason == .exit {
                print("task end normal")
                appdir = (try! NSString(contentsOfFile: "/tmp/IPA_EXT_appdir", encoding: String.Encoding.utf8.rawValue)) as String
                if appdir != nil {
                    //appdir = dropLast(appdir)
                    print(appdir + "/Info.plist")
                    oriInfoPlist = NSDictionary(contentsOfFile: appdir + "/Info.plist")
                    if oriInfoPlist != nil {
                        self.versionInfoTextView.stringValue = oriInfoPlist["CFBundleShortVersionString"] as! String
                        self.buildNumberTextView.stringValue = oriInfoPlist["CFBundleVersion"] as! String
                        bundle_id = oriInfoPlist["CFBundleIdentifier"] as! String
                        self.startRepackBtn.isEnabled = true
                    }
                }
            } else {
                print("task end bad")
            }
        }
        
        task.launch()
        
    }
    
    @IBAction func startRepack(_ sender: AnyObject) {
        //如果这里不读取一下versionInfoTextView和buildNumberTextView，startWorking会让正在编辑中的textfield的内容被恢复成编辑前的内容
        print("**** Version: \(versionInfoTextView.stringValue)  Build:\(buildNumberTextView.stringValue)")
        logWindow.string = "";
        startWorking()
        updateNewExeName()
        updateInfoPlist()
        updateProfileName()

        logBuffer.length = 0
        let task = Process()
        task.launchPath = Bundle.main.resourcePath! + "/repackipa.sh"
        task.arguments = [IPAFilePath.stringValue]
        let pipe = Pipe()
        task.standardOutput = pipe
        task.standardError = pipe
        NotificationCenter.default.addObserver(self, selector: #selector(ViewController.gotLog(_:)), name: NSNotification.Name.NSFileHandleDataAvailable, object: pipe.fileHandleForReading)
        pipe.fileHandleForReading.waitForDataInBackgroundAndNotify()
        
        // NSNotificationCenter.defaultCenter().addObserver(self, selector: Selector("taskEnded:"), name: NSTaskDidTerminateNotification, object: task)
        task.terminationHandler = {
            (task:Process) in
            self.endWorking()
            if task.terminationReason == .exit {
                print("task end normal")
                
            } else {
                print("task end bad")
            }
        }
        var env = Dictionary<String, String>()
        env["APP_DIR"] = appdir
        
        
        
        let t1=resNeedDeletePaths.string?.components(separatedBy: ";")
        var result = ""
        for i in t1! {
            if result.isEmpty{
                result += i
            } else {
                result += " " + i
            }
        }
        env["DELETE_DIRS"] = result
        
        let t2=resNormalPaths.string?.components(separatedBy: ";")
        result = ""
        for i in t2! {
            if result.isEmpty{
                result += i
            } else {
                result += " " + i
            }        }
        env["NORMAL_COPY_DIRS"] = result
        
        let t3=resNeedEncPaths.string?.components(separatedBy: ";")
        result = ""
        for i in t3! {
            if result.isEmpty{
                result += i
            } else {
                result += " " + i
            }
        }
        env["ENC_COPY_DIRS"] = result
        
        env["INFO_PLIST_DIR"] = "/tmp/IPA_NEW_INFO_PLIST"
        
        result=""
        for j in active_channels {
            let jj = (j as! NSDictionary);//["channel_id"] as String
            if result.isEmpty{
                result += jj["channel_id"] as! String
            } else {
                result += " " + (jj["channel_id"] as! String)
            }
        }
        env["CHANNEL_IDS"]=result
        
        env["BUNDLE_ID"]=bundle_id
        
        env["RESOURCE_PATH"] = Bundle.main.resourcePath
        //env["NORMAL_COPY_DIRS"] = "/Users/apache/GAME/resource/trgame/曹操传/ios_icons"
        //env["ENC_COPY_DIRS"] = "/Users/apache/GAME/resource/trgame/曹操传/正式发布资源/assets"
        if !profileName.isEmpty {
            env["PROFILE_NAME"] = profileName
        }

        if !newExeName.isEmpty {
            env["NEW_EXENAME"] = newExeName
        }

        env["VERSION_NUM"] = versionInfoTextView.stringValue
        env["BUILD_NUM"] = buildNumberTextView.stringValue

        task.environment = env
        
        task.launch()
        
        
        
    }
    
    func taskEnded(_ notification:Notification){
        
    }
    
    func gotLog(_ notification:Notification){
        let sender = notification.object as! FileHandle
        let logdata:Data = sender.availableData
        if logdata.count == 0 {
            return
        }
        logBuffer.append(logdata)
        let logtext:NSString? = NSString(data: logBuffer as Data, encoding: String.Encoding.utf8.rawValue)
        if logtext != nil {
            logWindow.string! += logtext as! String
            // Scroll to end of outputText field
            let range = NSMakeRange((logWindow.string! as NSString).length, 0);
            logWindow.scrollRangeToVisible(range)
            logBuffer.length = 0
        }
        //println(logtext)
        sender.waitForDataInBackgroundAndNotify()
    }
    
    func startWorking(){
        DispatchQueue.main.async {
            self.workingSpinner.isHidden=false
            self.workingSpinner.startAnimation(nil)
            for i in self.view.subviews{
                (i as? NSControl)?.isEnabled = false
            }
            self.resNeedEncPaths.isEditable = false
            self.resNormalPaths.isEditable = false
            self.resNeedDeletePaths.isEditable = false
        }
        
    }
    
    func endWorking(){
        DispatchQueue.main.async {
            self.workingSpinner.isHidden=true
            self.workingSpinner.stopAnimation(nil)
            for i in self.view.subviews{
                (i as? NSControl)?.isEnabled = true
            }

            if self.versionInfoEnableBtn.state == NSOnState {
                self.versionInfoTextView.isEnabled = true
            } else {
                self.versionInfoTextView.isEnabled = false
            }

            if self.buildNumberEnableBtn.state == NSOnState {
                self.buildNumberTextView.isEnabled = true
            } else {
                self.buildNumberTextView.isEnabled = false
            }

            self.resNeedEncPaths.isEditable = true
            self.resNormalPaths.isEditable = true
            self.resNeedDeletePaths.isEditable = true
        }
    }
    
    func updateProfileName(){
        profileName = ""
        if packageNameBox.indexOfSelectedItem > 0 {
            let modInfo = (games_config["games"] as! [NSDictionary])[packageNameBox.indexOfSelectedItem - 1]
            if modInfo["profile_name"] != nil {
                profileName = modInfo["profile_name"] as! String
            }
        }
    }
    func updateNewExeName() {
        newExeName = ""
        if packageNameBox.indexOfSelectedItem > 0 {
            let modInfo = (games_config["games"] as! [NSDictionary])[packageNameBox.indexOfSelectedItem - 1]
            if modInfo["game_name"] != nil {
                newExeName = modInfo["game_name"] as! String
            }
        }
    }
    
    func updateInfoPlist(){
        let newInfoPlist = NSMutableDictionary(dictionary: oriInfoPlist)
        if packageNameBox.indexOfSelectedItem > 0 {
            let modInfo = (games_config["games"] as! [NSDictionary])[packageNameBox.indexOfSelectedItem - 1]["game_special_infoplist"] as! NSDictionary
            for key in modInfo.allKeys as! [String]{
                newInfoPlist[key] = modInfo[key]
            }
        }
        
        newInfoPlist["CFBundleShortVersionString"] = versionInfoTextView.stringValue
        newInfoPlist["CFBundleVersion"] = buildNumberTextView.stringValue
        if !newExeName.isEmpty {
            newInfoPlist["CFBundleExecutable"] = newExeName
        }
        // newInfoPlist["CFBundleIdentifier"] = bundleIdList.allKeys[packageNameBox.indexOfSelectedItem]
        bundle_id = newInfoPlist["CFBundleIdentifier"] as! String
        
        newInfoPlist.write(toFile: "/tmp/IPA_NEW_INFO_PLIST", atomically: true)
    }
    @IBAction func modifyResPathBtnPushed(_ sender: NSButton) {
        switch (sender.tag){
        case 1:
            let myFiledialog: NSOpenPanel = NSOpenPanel()
            
            myFiledialog.prompt = "选择加密资源目录或文件"
            myFiledialog.worksWhenModal = true
            myFiledialog.allowsMultipleSelection = false
            myFiledialog.canChooseDirectories = true
            myFiledialog.resolvesAliases = true
            
            myFiledialog.beginSheetModal(for: self.view.window!){
                (result) -> () in
                if result == NSFileHandlingPanelOKButton {
                    if self.resNeedEncPaths.string!.isEmpty{
                        self.resNeedEncPaths.string = myFiledialog.url!.path
                    } else {
                        self.resNeedEncPaths.string! += ";" + myFiledialog.url!.path
                    }
                }
            }
        case 2:
            self.resNeedEncPaths.string = ""
        case 3:
            let myFiledialog: NSOpenPanel = NSOpenPanel()
            
            myFiledialog.prompt = "选择普通资源目录或文件"
            myFiledialog.worksWhenModal = true
            myFiledialog.allowsMultipleSelection = false
            myFiledialog.canChooseDirectories = true
            myFiledialog.resolvesAliases = true
            
            myFiledialog.beginSheetModal(for: self.view.window!){
                (result) -> () in
                if result == NSFileHandlingPanelOKButton {
                    if self.resNormalPaths.string!.isEmpty{
                        self.resNormalPaths.string = myFiledialog.url!.path
                    } else {
                        self.resNormalPaths.string! += ";" + myFiledialog.url!.path
                    }
                }
            }
        case 4:
            self.resNormalPaths.string = ""
        default:
            print("Bad button tag")
        }
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        workingSpinner.isHidden = true
        channel_config = getChannelDict()
        channels_tableview.reloadData()
        games_config = getGamesConfig()
        versionInfoTextView.isEnabled = false
        buildNumberTextView.isEnabled = false
        startRepackBtn.isEnabled = false
        packageNameBox.removeAllItems()
        packageNameBox.addItem(withTitle: "不修改")
        //        bundleIdList=getBundleIDs()
        //        for i in bundleIdList {
        //            packageNameBox.addItemWithTitle((i.value as String) + "  " + (i.key as String))
        //        }
        let gameList = games_config["games"] as! [NSDictionary]
        for i in gameList {
            packageNameBox.addItem(withTitle: ((i["game_special_infoplist"] as! NSDictionary)["CFBundleDisplayName"] as! String) + ",  " + ((i["game_special_infoplist"] as! NSDictionary)["CFBundleIdentifier"] as! String))
        }
        KTPlayListBox.removeAllItems()
        KTPlayListBox.addItems(withTitles: ["关闭","打开"])
        
        // Do any additional setup after loading the view.
    }
    
//    override var representedObject: AnyObject? {
//        didSet {
//            // Update the view, if already loaded.
//        }
//    }

    
}


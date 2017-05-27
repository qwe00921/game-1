//
//  main.cpp
//  trgameEncoder
//
//  Created by shenliming on 15/2/10.
//  Copyright (c) 2015年 shenliming. All rights reserved.
//

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "codec.h"

static int partEncBytes = 2048;
static int codekeyVersion = 1;
static std::string codekey = "";
static std::vector<std::string> partEncExtensionNames;
static std::vector<std::string> completeEncExtensionNames;
static std::string workingDirectory = "./";
static std::vector<std::string> processFiles;

bool parseOPTS(int argc, const char * argv[]){
    namespace po = boost::program_options;
    std::vector<std::string> partExtList;
    std::vector<std::string> completeExtList;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "produce help message")
    ("partencbytes", po::value<int>(), "set partial encrypted length")
    ("codekeyversion", po::value<int>(), "set version number of codekey")
    ("codekey", po::value<std::string>(), "set codekey content")
    ("partext", po::value<std::vector<std::string>>(&partExtList)->multitoken(), "set partial encrypted file's extension names, separete by space. eg. --partext bmp e5")
    ("completeext", po::value<std::vector<std::string>>(&completeExtList)->multitoken(), "set complete encrypted file's extension names, separete by space. eg. --completeext json LM")
    ("dir", po::value<std::string>(), "set working directory")

    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return false;
    }
    
    if (vm.count("partencbytes")) {
        std::cout << "partial encrypted length was set to "
        << vm["partencbytes"].as<int>() << ".\n";
        partEncBytes = vm["partencbytes"].as<int>();
    } else {
        
    }
    
    if (vm.count("codekeyversion")) {
        std::cout << "version number of codekey was set to "
        << vm["codekeyversion"].as<int>() << ".\n";
        partEncBytes = vm["codekeyversion"].as<int>();
    } else {
        
    }
    
    if (vm.count("codekey")) {
        std::cout << "codekey content was set to "
        << vm["codekey"].as<std::string>() << ".\n";
        codekey = vm["codekey"].as<std::string>();
    } else {
        
    }
    
    if (vm.count("partext")) {
        if(partExtList.size() < 1) {
            std::cout << "wrong usage of --partext" << std::endl;
            return false;
        }
        std::cout << "partial encrypted file's extension names were set to " << std::endl;
        for( int i = 0; i < partExtList.size(); i++){
            std::cout << i << " " << partExtList[i] << std::endl;
            partEncExtensionNames.push_back("." + partExtList[i]);
        }
        
    } else {
        
    }
    
    if (vm.count("completeext")) {
        if(partExtList.size() < 1) {
            std::cout << "wrong usage of --completeext" << std::endl;
            return false;
        }
        std::cout << "complete encrypted file's extension names were set to " << std::endl;
        for( int i = 0; i < completeExtList.size(); i++){
            std::cout << i << " " << completeExtList[i] << std::endl;
            completeEncExtensionNames.push_back("." + completeExtList[i]);
        }
        
    } else {
        
    }
    
    if (vm.count("dir")) {
        std::cout << "working directory was set to "
        << vm["dir"].as<std::string>() << ".\n";
        workingDirectory = vm["dir"].as<std::string>();
    } else {
        
    }
    
    return true;
}

bool needPartEncrypt(std::string extname){
    for(int i=0 ; i < partEncExtensionNames.size(); i++) {
        if(extname == partEncExtensionNames[i])
            return true;
    }
    return false;
}

bool needCompleteEncrypt(std::string extname){
    for(int i=0 ; i < completeEncExtensionNames.size(); i++) {
        if(extname == completeEncExtensionNames[i])
            return true;
    }
    return false;
}


bool encodeOneFile(const char * filename, bool needComplete) {
    bool retval = false;
    char * buf = 0;
    SecretFile * sec = 0;
    char * retBuf = 0;
    int retLength = 0;
    int writeLength = 0;
    int fileLength = 0;
    int readLength = 0;
    
    std::cout << "Encrypting file: " << std::string(filename) << std::endl;
    FILE *fp = fopen(filename, "rb");
    if(fp == NULL) {
        perror("open file for read error");
        goto out1;
    }
    fseek(fp, 0L, SEEK_END);
    fileLength = ftell(fp);
    if (fileLength == 0){
        retval = true;
        goto out1;
    }
    
//    if (fileLength < partEncBytes)
//        needComplete = true;
    
    buf = new char[fileLength];
    fseek(fp, 0L, SEEK_SET);
    readLength = fread(buf, 1, fileLength, fp);
    if(readLength != fileLength) {
        perror("read file error");
        goto out1;
    }
    fclose(fp);
    fp = 0;
    
    sec = new SecretFile();
    sec->setCodeSecret(codekeyVersion);
    if(!codekey.empty()){
        sec->setCodeSecretKey((char*)codekey.c_str());
    }

    if (needComplete){
        retBuf = sec->encryptDataPartial(buf, fileLength, fileLength, &retLength);
    } else {
        retBuf = sec->encryptDataPartial(buf, fileLength, partEncBytes, &retLength);
    }

    fp = fopen(filename, "wb");
    if(fp == NULL) {
        perror("open file for write error");
        goto out1;
    }
    
    writeLength = fwrite(retBuf, 1, retLength, fp);
    if(writeLength != retLength){
        perror("write data error");
        goto out1;
    }
    retval = true;
    
out1:
    if(fp !=0)
        fclose(fp);
    if(buf != 0)
        free(buf);
    if(sec != 0)
        free(sec);
    if(retBuf !=0)
        free(retBuf);
    
    return retval;
}



int main(int argc, const char * argv[]) {

    if(!parseOPTS(argc, argv)){
        return -1;
    }
    
    std::cout << "Scanning directory " << workingDirectory << std::endl;

    if(boost::filesystem::exists(workingDirectory)) {
        boost::filesystem::recursive_directory_iterator end ;
        for( boost::filesystem::recursive_directory_iterator iter(workingDirectory) ; iter != end ; ++iter ) {
           // std::cout << *iter << std::endl;
            if ( !boost::filesystem::is_directory(*iter)) {
//                std::cout << "enqueue" << std::endl;
                processFiles.push_back(iter->path().string());
            }
        }
    }
    
    for(int i; i<processFiles.size(); i++){
        if (needPartEncrypt(boost::filesystem::path(processFiles[i]).extension().string())) {
            bool ret = encodeOneFile(processFiles[i].c_str(), false);
            if(!ret){
                std::cout<<"Error in process file:" << processFiles[i] << std::endl;
                return -1;
            }
            continue;
        }
        
        if (needCompleteEncrypt(boost::filesystem::path(processFiles[i]).extension().string())) {
            bool ret = encodeOneFile(processFiles[i].c_str(), true);
            if(!ret){
                std::cout<<"Error in process file:" << processFiles[i] << std::endl;
                return -1;
            }
            continue;
        }
        
        std::cout << "Skipping file " << processFiles[i] << std::endl;
        
    }
    std::cout << "work completed!" << std::endl;
    return 0;
}

#!/bin/python
# -*- coding: utf-8 -*-

import shutil
import re
import os
import codecs
import zipfile
from datetime import datetime
import gzip
import json
import random as rd

class Utils(object){
    TMP_FOLDER=None
    LOGGER=None

    def __init__(self,tmp_folder,logger){
        Utils.LOGGER=logger
		Utils.TMP_FOLDER=tmp_folder
    }

    @staticmethod
    def createFolderIfNotExists(path){
        if not os.path.exists(path){
            os.makedirs(path, exist_ok=True)
        }
    }

    @staticmethod
    def deletePath(path){
        if os.path.isdir(path){
            Utils.deleteFolder(path)
        }elif os.path.isfile(path){
            Utils.deleteFile(path)
        }else{
            Utils.LOGGER.fatal('File {} is a special file.'.format(path))
        }
    }

    @staticmethod
    def deleteFile(path){
        if os.path.exists(path){
            os.remove(path)
        }else{
            Utils.LOGGER.warn('The file {} does not exist.'.format(path))
        }
    }

    @staticmethod
    def deleteFolder(path){
        if os.path.exists(path){
            shutil.rmtree(path)
        }else{
            Utils.LOGGER.warn('The folder {} does not exist.'.format(path))
        }
    }

    @staticmethod
    def saveJson(path,data,pretty=True){
        with open(path, 'w') as fp{
            json.dump(data, fp, indent=3 if pretty else None)
        }
    }

    @staticmethod
    def changeStrDateFormat(date,input_format,output_format){
        date=datetime.strptime(date, input_format)
        return date.strftime(output_format)
    }

    @staticmethod
    def loadJson(path){
        with open(path, 'r') as fp {
            data=json.load(fp)
        }
        return data
    }

    @staticmethod
    def getTmpFolder(base_name,random=False){
        if random{
            destination_folder=Utils.TMP_FOLDER+base_name+str(rd.randint(0,65535))+'/'
        }else{
            destination_folder=Utils.TMP_FOLDER+base_name+'/'
        }
        Utils.createFolderIfNotExists(destination_folder)
        return destination_folder
    }

    @staticmethod
    def getTodayDate(date_format='%d/%m/%Y'){
        return datetime.now().strftime(date_format)
    }

    @staticmethod
    def getIndexOfDictList(docs,key,value){
        for i in range(len(docs)){
            if docs[i][key]==value{
                return i
            }
        }
    }

    @staticmethod
    def countCSVColumns(path,delimiter=','){
        max_columns=0
        with codecs.open(path, 'r', 'utf-8',errors='ignore') as file{
            for line in file{
                line = re.sub(r'".*?"','',line)
                count=len(line.split(delimiter))
                if count>max_columns{
                    max_columns=count
                }
            }
        }
        return max_columns
    }

    @staticmethod
    def filenameFromPath(path,get_extension=False){
        if get_extension {
            re_result=re.search(r'.+(\/.+)', path)
            return re_result.group(1) if re_result is not None else path
        }else{
            re_result=re.search(r'.+(\/.+)\..+', path)
            return re_result.group(1) if re_result is not None else path
        }
    } 

    @staticmethod
    def partentFromPath(path){
        re_result=re.search(r'(.+\/).+', path)
        return re_result.group(1) if re_result is not None else path
    } 

    @staticmethod
    def unzip(path,destination_folder=None){
        Utils.LOGGER.info('Unziping file {}...'.format(path))
        if not destination_folder{
            destination_folder=Utils.TMP_FOLDER+Utils.filenameFromPath(path)+'/'
        }else{
            destination_folder=Utils.TMP_FOLDER+destination_folder
        }
        Utils.createFolderIfNotExists(destination_folder)
        with zipfile.ZipFile(path, 'r') as zip_ref{
            zip_ref.extractall(destination_folder)
        }
        Utils.deleteFile(path)
        Utils.LOGGER.info('Unziped file {}...OK'.format(path))
        return destination_folder
    }

    @staticmethod
    def gunzip(path,extension){
        Utils.LOGGER.info('Gunziping file {}...'.format(path))
        destination_folder=Utils.TMP_FOLDER+Utils.filenameFromPath(path)+'/'
        Utils.createFolderIfNotExists(destination_folder)        
        destination_filename=Utils.filenameFromPath(path)+extension
        block_size=65536
        with gzip.open(path, 'rb') as s_file{
            destination_path=destination_folder+destination_filename
            with open(destination_path, 'wb') as d_file{
                while True{
                    block = s_file.read(block_size)
                    if not block{
                        break
                    }else{
                        d_file.write(block)
                    }
                }
            }
        }
        Utils.deleteFile(path)
        Utils.LOGGER.info('Gunziped file {}...OK'.format(path))
        return destination_folder
    }

    @staticmethod
    def openFile(path){
        with codecs.open(path, 'r', 'utf-8', errors='ignore') as file{
            return file.read()
        }
    }

    @staticmethod
    def saveFile(path,content){
        with codecs.open(path, 'w', 'utf-8') as file{
            file.write(content)
        }
    }
}
#!/bin/python
# -*- coding: utf-8 -*-

import shutil
import re
import os
import codecs
import zipfile
from datetime import datetime
import tarfile
import json

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
            re_result=re.search(r'.*\/(.*\..+)', path)
            return re_result.group(1) if re_result is not None else path
        }else{
            re_result=re.search(r'.*\/(.*)\..+', path)
            return re_result.group(1) if re_result is not None else path
        }
    } 

    @staticmethod
    def unzip(path){
        Utils.LOGGER.info('Unziping file {}...'.format(path))
        destination_folder=Utils.TMP_FOLDER+Utils.filenameFromPath(path)+'/'
        Utils.createFolderIfNotExists(destination_folder)
        with zipfile.ZipFile(path, 'r') as zip_ref{
            zip_ref.extractall(destination_folder)
        }
        Utils.deleteFile(path)
        Utils.LOGGER.info('Unziped file {}...OK'.format(path))
        return destination_folder
    }

    @staticmethod
    def ungzip(path){
        destination_folder=Utils.TMP_FOLDER+Utils.filenameFromPath(path)+'/'
        Utils.createFolderIfNotExists(destination_folder)
        with tarfile.open(path, 'r:gz') as gzip_ref{
            gzip_ref.extractall(destination_folder)
        }
        Utils.deleteFile(path)
        return destination_folder
    }
}
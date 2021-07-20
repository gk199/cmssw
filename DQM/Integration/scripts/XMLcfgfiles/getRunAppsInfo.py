#!/usr/bin/env python3
"""This module provides the PID and the log file name of the running DQM 
applications (consumers), thus completing the information generated by 
ExtractAppInfoFromXML.

When used as a script the following options are accepted:
	-f Show all columns
	-h show headers
"""
from __future__ import print_function
import sys
import os.path
import getopt as gop
import ExtractAppInfoFromXML as appinf

# ssh srv-c2d05-18.cms ps -eo pid,cmd | grep 22101
################################################################################
def getAppPID(srv,port):
    try:
        print("Connecting to server: "+srv+" and fetching PID for application running on port: "+port)
        cf=os.popen('ssh '+srv+' ps -eo pid,cmd | grep '+port)
        l=cf.readline();
        if l=="":
            cf.close()
            return "App Not Running" 
        else:
            pidv=l.split()
            cf.close();
            return pidv[0]
    except:
        sys.stderr.write(  "Something went really bad\n" )
    return -1
################################################################################
def getAppLogFileName(srv,pid):
#	try:
        #pid=getAppPID(srv,port)
    if pid=="App Not Running":
        return "No active log file"
    else:
        print("Connecting to server: "+srv+" and fetching LogFile for application with PID: "+pid)
        cf=os.popen('ssh '+srv+' ls -l /tmp | grep '+pid)
        l=cf.readline();
        if l=="":
            cf.close()
            return "App Not Running???" 
        else:
            logfilev=l.split()
            cf.close();
            return logfilev[-1]
#	except Exception,e:
#		sys.stderr.write(  "Something went really bad\n" + e[0])
#        return -1
################################################################################
def getRunningAppsInfo(filename):
    (table,grid)=appinf.getAppInfo(filename,2,1,3,4)
    for apps in grid:
        apps.insert(3,getAppPID(apps[1],apps[2]))
        apps.insert(4,getAppLogFileName(apps[1],apps[3]))
    return grid
################################################################################
#Script operation                                                              #
################################################################################
if __name__ == "__main__":             
    fullinfo=False
    headers=False
    try:
        (args,filename)=gop.getopt(sys.argv[1:],"hf",["help"])
    except getopt.GetoptError:
        sys.stderr.write(  "Sintax Error unrecognised option" )
        sys.stderr.write( __doc__ )
        sys.exit(2)

    for item in args:
        if item[0]=="-h":
            headers=True
        elif item[0]=="-f":
            fullinfo=True
        elif item[0]=="--help":
            sys.stdout.write(__doc__)
            sys.exit(2)
    if len(filename)==0:
        sys.stderr.write(  "\nERROR: xdaq XML config file name not present, please specify\n\n" )
        sys.stdout.write(__doc__)
    elif len(filename) > 1:
        sys.stderr.write(  "\nERROR: Too many file names or other arguments, please specify only 1\n\n" )
        sys.stdout.write(__doc__)
        sys.exit(2)
    elif not os.path.exists(filename[0]):
        sys.stderr.write(  "\nERROR: xdaq XML config file does not exist please verify\n\n" )
        sys.stdout.write(__doc__)
        sys.exit(2)
    grid=getRunningAppsInfo(filename[0])
    if fullinfo:
        if headers:
            grid.insert(0,["Application","Server","Port","PID","LogfileName","App Config File"])
    else:
        if headers:
            i=0;
            for record in grid:
                newrecord=[record[0],record[3],record[4]]
                grid[i]=newrecord
                del record
                i+=1
            grid.insert(0,["Application","PID","LogfileName"])

        else:
            i=0;
            for record in grid:
                newrecord=[record[0],record[3],record[4]]
                grid[i]=newrecord
                del record
                i+=1

    appinf.printGrid(grid)



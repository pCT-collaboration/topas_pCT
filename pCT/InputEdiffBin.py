import getopt
import numpy
from numpy import *
import struct
import sys
import time
import cmath
import re

def usage():
    print "Usage: python", sys.argv[0]
    print "    --help  display this message"
    print "    --angle=<projection angle>"
    print "    --dir=<input and output directory>"
    print "    --histories=<number of histories>"
    print "    --phantom=<name of phantom>"
    print "    --preparer=<name of preparer>"

try:
    opts, args = getopt.getopt(sys.argv[1:], "", ["help", "angle=", "dir=", "histories=", "phantom=", "preparer="])
except getopt.GetoptError as err:
    # print help information and exit:
    print(err) # will print something like "option -a not recognized"
    usage()
    sys.exit(2)

angle = ""
directory = ""
histories = 0
phantom = ""
preparer = ""
for o, a in opts:
    if o == "--help":
        usage()
        sys.exit()
    elif o == "--angle":
        angle = a
    elif o == "--dir":
        directory = a
    elif o == "--histories":
        histories = int(a)
    elif o == "--phantom":
        phantom = a
    elif o == "--preparer":
        preparer = a
    else:
        print o, a
        assert False, "unhandled option"

if args != []:
    print "too many arguments", args
    usage()
    sys.exit(2)

if angle == "":
    print "required parameter 'angle' is missing"
    sys.exit(2)

if directory == "":
    print "required parameter 'directory' is missing"
    sys.exit(2)

if histories == 0:
    print "histories must be > 0"
    usage()
    sys.exit(2)

if phantom == "":
    print "required parameter 'phantom' is missing"
    sys.exit(2)

if preparer == "":
    print "required parameter 'preparer' is missing"
    sys.exit(2)

def EtoWepl2( Ene):
    X= 200-Ene #Proton
    X2= X*X
    X3= X2*X
    X4= X2*X2
    paramter = array ([251.716, -0.308624, -0.00418662, -9.1074e-06,3.06738e-08]) #proton
#    paramter = array ([256.213,-0.016466,-0.000524894,2.26596e-07,-6.85547e-11]) #range from Astar
#    paramter = array ([248.807, -0.278675, -0.00542884, 1.25484e-06,3.93298e-09]) #p SS
#    paramter = array ([250.787, -0.0487516, -0.000368286, 8.3514e-09,4.42021e-11]) #He polished
    Wepl = paramter[0]+paramter[1]*X+paramter[2]*X2+paramter[3]*X3+paramter[4]*X4
#    print Ene, Wepl * 1.043
    return Wepl*1.043

def GetDataPhSp(fileName):
    dtype = 'f,f,f,f,f,f,f,i,b,b,i,i,i'
    idata = fromfile(fileName + ".phsp",dtype=dtype)
    data = zeros((histories,8))
    idata = idata[ where( idata['f12'] == 0 ) ]
    evts = idata['f11']
    data[evts,0] = 1
    data[evts,1] = idata['f11']
    data[evts,2] = idata['f0']*10.0
    data[evts,3] = idata['f1']*10.0
    data[evts,4] = idata['f2']*10.0
    data[evts,5] = idata['f5']
    data[evts,6] = idata['f3']
    data[evts,7] = idata['f4']
    evts = []
    idata = []
    print 'Read file:', fileName + ".phsp"
    return data

time_start = time.clock()
iFileName = directory + '/' + phantom + '/' + angle
oFileName = directory + '/' + phantom + '/projection_' + angle + ".bin"

#Plane 1
p1tOut = GetDataPhSp(iFileName + '/Tracker1t_Out')
p1vOut = GetDataPhSp(iFileName + '/Tracker1v_Out')

#Plane 2
p2tOut = GetDataPhSp(iFileName + '/Tracker2t_Out')
p2vOut = GetDataPhSp(iFileName + '/Tracker2v_Out')

#Plane 3
p3tOut = GetDataPhSp(iFileName + '/Tracker3t_Out')
p3vOut = GetDataPhSp(iFileName + '/Tracker3v_Out')

#Plane 4
p4tOut = GetDataPhSp(iFileName + '/Tracker4t_Out')
p4vOut = GetDataPhSp(iFileName + '/Tracker4v_Out')

Z_C = array([-214.3, -164.3, 164.3, 214.3])

n=0
chunk = histories/10
totalLines = 0
Vw0 = []
Vw1 = []
Vw2 = []
Vw3 = []

Tw0 = []
Tw1 = []
Tw2 = []
Tw3 = []

Uw0 = []
Uw1 = []
Uw2 = []
Uw3 = []

WEPLw = []
for i in xrange(histories):
    if p1tOut[i,0] == 1 and p1vOut[i,0] == 1 and p2tOut[i,0] == 1 and p2vOut[i,0] == 1 and p3tOut[i,0] == 1 and p3vOut[i,0] == 1 and p4tOut[i,0] == 1 and p4vOut[i,0] == 1:
        #Vw0[i]=p1vOut[i,3]
        Vw0.append(p1vOut[i,3])
        Vw1.append(p2vOut[i,3])
        Vw2.append(p3vOut[i,3])
        Vw3.append(p4vOut[i,3])

        Tw0.append(p1tOut[i,2])
        Tw1.append(p2tOut[i,2])
        Tw2.append(p3tOut[i,2])
        Tw3.append(p4tOut[i,2])

        Uw0.append(Z_C[0])
        Uw1.append(Z_C[1])
        Uw2.append(Z_C[2])
        Uw3.append(Z_C[3])

#        print p2tOut[i,5], p3tOut[i,5]
        WEPLw.append(EtoWepl2( p2tOut[i,5]- p3tOut[i,5]))

        totalLines += 1
    if i % chunk == 0:
        print 'Processed chunk ', n , 'of 10.'
        n+=1

Vw0 = numpy.asarray(Vw0)
Vw1 = numpy.asarray(Vw1)
Vw2 = numpy.asarray(Vw2)
Vw3 = numpy.asarray(Vw3)

Tw0 = numpy.asarray(Tw0)
Tw1 = numpy.asarray(Tw1)
Tw2 = numpy.asarray(Tw2)
Tw3 = numpy.asarray(Tw3)

Uw0 = numpy.asarray(Uw0)
Uw1 = numpy.asarray(Uw1)
Uw2 = numpy.asarray(Uw2)
Uw3 = numpy.asarray(Uw3)

WEPLw = numpy.asarray(WEPLw)

magic_number = "PCTD";
version_id = 0
beam_energy = 200. #// MeV;
current_time = time.clock();
projection_angle= float(angle);
phantom_name_size = len(phantom);
print "phantom_name_size"
print phantom_name_size
data_source_size = len(iFileName);
print "data_source_size"
print data_source_size
prepared_by_size = len(preparer);
print "prepared_by_size"
print prepared_by_size
#// Write headers:
ofile = open(oFileName,'wb')

myfmt='s'*len(magic_number)
# print myfmt
bin=struct.pack(myfmt,*magic_number) # magic number identifier
ofile.write(bin)

myfmt='i'
bin=struct.pack(myfmt,version_id) # version
ofile.write(bin)

# // (note that it doesn't include null terminator '\0')
myfmt='i'
bin=struct.pack(myfmt,totalLines) # total number of lines
ofile.write(bin)

myfmt='f'
bin=struct.pack(myfmt,projection_angle) #
ofile.write(bin)

myfmt='f'
bin=struct.pack(myfmt,beam_energy) #
ofile.write(bin)

myfmt='i'
bin=struct.pack(myfmt,current_time) #
ofile.write(bin)

myfmt='i'
bin=struct.pack(myfmt,version_id) #
ofile.write(bin)

myfmt='i'
bin=struct.pack(myfmt,phantom_name_size) #
ofile.write(bin)

myfmt='s'*phantom_name_size
bin=struct.pack(myfmt,*phantom) #
ofile.write(bin)

myfmt='i'
bin=struct.pack(myfmt,data_source_size) #
ofile.write(bin)

myfmt='s'*data_source_size
bin=struct.pack(myfmt,*iFileName) #
ofile.write(bin)

myfmt='i'
bin=struct.pack(myfmt,prepared_by_size) #
ofile.write(bin)

myfmt='s'*prepared_by_size
bin=struct.pack(myfmt,*preparer) #
ofile.write(bin)

myfmt='f'*totalLines
#  You can use 'd' for double and < or > to force endinness

bin=struct.pack(myfmt,*Tw0)
ofile.write(bin)
bin=struct.pack(myfmt,*Tw1)
ofile.write(bin)
bin=struct.pack(myfmt,*Tw2)
ofile.write(bin)
bin=struct.pack(myfmt,*Tw3)
ofile.write(bin)

bin=struct.pack(myfmt,*Vw0)
ofile.write(bin)
bin=struct.pack(myfmt,*Vw1)
ofile.write(bin)
bin=struct.pack(myfmt,*Vw2)
ofile.write(bin)
bin=struct.pack(myfmt,*Vw3)
ofile.write(bin)

bin=struct.pack(myfmt,*Uw0)
ofile.write(bin)
bin=struct.pack(myfmt,*Uw1)
ofile.write(bin)
bin=struct.pack(myfmt,*Uw2)
ofile.write(bin)
bin=struct.pack(myfmt,*Uw3)
ofile.write(bin)

bin=struct.pack(myfmt,*WEPLw)
ofile.write(bin)

ofile.close()

time_end = time.clock() - time_start
print "Total time: ", time_end, " (s)"
print "Total particles accepted: ", totalLines

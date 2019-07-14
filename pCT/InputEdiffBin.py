import numpy
from numpy import *
import struct
import sys
import time
import cmath
# import matplotlib
# matplotlib.use("Agg")  # or whichever backend you wish to use

try:
    N = sys.argv[1]
    ofileName = sys.argv[2]
    Nh = int(sys.argv[3])
    iFileName = sys.argv[4]
#ofileName1 = sys.argv[4]
except:
	print "Use: python", sys.argv[0], " BrickN OutputFileName bigNumber InputFilDir"
	exit(1)


def EtoWepl2( Ene):
    X= 200-Ene #Proton
#    X= Ene #Proton SS
#    X= 800-Ene #Helium
#    X= Ene #He Astar
    X2= X*X
    X3= X2*X
    X4= X2*X2
    paramter = array ([251.716, -0.308624, -0.00418662, -9.1074e-06,3.06738e-08]) #proton
#    paramter = array ([256.213,-0.016466,-0.000524894,2.26596e-07,-6.85547e-11]) #range from Astar
#    paramter = array ([248.807, -0.278675, -0.00542884, 1.25484e-06,3.93298e-09]) #p SS
#    paramter = array ([250.787, -0.0487516, -0.000368286, 8.3514e-09,4.42021e-11]) #He polished
    Wepl = paramter[0]+paramter[1]*X+paramter[2]*X2+paramter[3]*X3+paramter[4]*X4
#    return Wepl
    return Wepl*1.043



def GetData(fileName, NbOfHistories):
    dtype = 'i,f'
    idata  = fromfile(fileName,dtype=dtype)
    data = zeros((NbOfHistories,3))
    evts = idata['f0']
    data[evts,0] = 1	# Hit or not
    data[evts,1] = idata['f0'] # evtID
    data[evts,2] = idata['f1'] # hit info (edep or position)
    evts = []
    idata = []
    print 'Read file:', fileName
    return data

def GetDataPhSp(fileName, NbOfHistories):
    dtype = 'f,f,f,f,f,f,f,i,b,b,i,i'
    idata = fromfile(fileName,dtype=dtype)
    data = zeros((NbOfHistories,8))
    idata = idata[ where( idata['f11'] == 0 ) ]
    evts = idata['f10']
    data[evts,0] = 1
    data[evts,1] = idata['f10']
    data[evts,2] = idata['f0']*10.0
    data[evts,3] = idata['f1']*10.0
    data[evts,4] = idata['f2']*10.0
    data[evts,5] = idata['f5']
    data[evts,6] = idata['f3']
    data[evts,7] = idata['f4']


    evts = []
    idata = []
    print 'Read file:', fileName
    return data
def utf8len(s):
    return len(s.encode('utf-8'))
#def GetTrackLength(fileName, NbOfHistories):
#    dtype = 'i,f'
#    idata = fromfile(fileName,dtype=dtype)
#    data = zeros((NbOfHistories,2))
#    #idata = idata[ where( idata['f2']==0)]
#
#    evts = idata['f0']
#    #data[evts,0] = 1
#    data[evts,0] = idata['f1']
#
#
#    evts = []
#    idata = []
#    print 'ReadFile:', fileName
#    return data


time_start = time.clock()
#Plane 1
# p1tIn  = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker1t_In_LinePair_'+N+'.phsp',Nh)
p1tOut = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker1t_Out_LinePair_'+N+'.phsp',Nh)
# p1vIn  = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker1v_In_LinePair_'+N+'.phsp',Nh)
p1vOut = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker1v_Out_LinePair_'+N+'.phsp',Nh)

#Plane 2
# p2tIn  = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker2t_In_LinePair_'+N+'.phsp',Nh)
p2tOut = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker2t_Out_LinePair_'+N+'.phsp',Nh)
# p2vIn  = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker2v_In_LinePair_'+N+'.phsp',Nh)
p2vOut = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker2v_Out_LinePair_'+N+'.phsp',Nh)

#Plane 3
# p3tIn  = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker3t_In_LinePair_'+N+'.phsp',Nh)
p3tOut = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker3t_Out_LinePair_'+N+'.phsp',Nh)
# p3vIn  = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker3v_In_LinePair_'+N+'.phsp',Nh)
p3vOut = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker3v_Out_LinePair_'+N+'.phsp',Nh)

#Plane 4
# p4tIn  = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker4t_In_LinePair_'+N+'.phsp',Nh)
p4tOut = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker4t_Out_LinePair_'+N+'.phsp',Nh)
# p4vIn  = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker4v_In_LinePair_'+N+'.phsp',Nh)
p4vOut = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker4v_Out_LinePair_'+N+'.phsp',Nh)

#
# e0 = #GetDataPhSp(iFileName+'/Angle'+N+'/Tracker2t_Out_LinePair_'+N+'.phsp',Nh)
# e1 = GetDataPhSp(iFileName+'/Angle'+N+'/Tracker3t_In_LinePair_'+N+'.phsp',Nh)
#
#s1 = GetTrackLength(iFileName+'/Angle'+N+'/Step1Track_RawTuple_float64.bin',Nh)
#s2 = GetTrackLength(iFileName+'/Angle'+N+'/Step2Track_RawTuple_float64.bin',Nh)
#s3 = GetTrackLength(iFileName+'/Angle'+N+'/Step3Track_RawTuple_float64.bin',Nh)
#d1 = GetTrackLength(iFileName+'/Angle'+N+'/OutTrack_RawTuple_float64.bin',Nh)

#print s1, s2, s3, d1


#ofile1 = open(ofileName1, 'w')

Z_C = array([-214.3, -164.3, 164.3, 214.3])
    #for i in xrange(Nh):
    # ofile1.write('%f %f %f %f \n' % (p1tIn[i,5], p2tIn[i,5], p3tIn[i,5], p4tIn[i,5]\
    #                     ))
#ofile1.close()

n=0
chunk = Nh/10
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


for i in xrange(Nh):
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

        WEPLw.append(EtoWepl2( p2tOut[i,5]- p3tOut[i,5]))
#        WEPLw.append(EtoWepl2( p3tOut[i,5]))
            # ofile.write('%f %f %f %f %f %f %f %f %f %f %f %f %f  \n' % (\
			# 	 	p1vOut[i,3], p1tOut[i,2], Z_C[0],\
			# 		p2vOut[i,3], p2tOut[i,2], Z_C[1],\
			# 		p3vOut[i,3], p3tOut[i,2], Z_C[2],\
			# 		p4vOut[i,3], p4tOut[i,2], Z_C[3],\
            #         p2tOut[i,5]- p3tOut[i,5]))
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
PHANTOM_NAME= "Water";
PREPARED_BY= "Pierluigi Piersimoni";
version_id = 0
beam_energy = 200. #// MeV;
current_time = time.clock();
projection_angle= float(N);

phantom_name_size = len(PHANTOM_NAME);

print "phantom_name_size"
print phantom_name_size
data_source_size = len(iFileName);
print "data_source_size"
print data_source_size
prepared_by_size = len(PREPARED_BY);
print "prepared_by_size"
print prepared_by_size
#// Write headers:
ofile = open(ofileName,'wb')
# data_file.write(magic_number, 4);
myfmt='s'*len(magic_number)
# print myfmt
bin=struct.pack(myfmt,*magic_number) # magic number identifier
ofile.write(bin)

# data_file.write(reinterpret_cast<char*>(&version_id), sizeof(int)); 		// format version identifier
myfmt='i'
bin=struct.pack(myfmt,version_id) # magic number identifier
ofile.write(bin)

# // (note that it doesn't include null terminator '\0')
# data_file.write(reinterpret_cast<char*>(&event_counter), sizeof(int)); 	// number of events in file
myfmt='i'
bin=struct.pack(myfmt,totalLines) # magic number identifier
ofile.write(bin)

# data_file.write(reinterpret_cast<char*>(&projection_angle), sizeof(float)); // projection angle
myfmt='f'
bin=struct.pack(myfmt,projection_angle) #
ofile.write(bin)

# data_file.write(reinterpret_cast<char*>(&beam_energy), sizeof(float)); 		// beam energy
myfmt='f'
bin=struct.pack(myfmt,beam_energy) #
ofile.write(bin)

# data_file.write(reinterpret_cast<char*>(&current_time), sizeof(int)); 		// generation date
myfmt='i'
bin=struct.pack(myfmt,current_time) #
ofile.write(bin)

# data_file.write(reinterpret_cast<char*>(&current_time), sizeof(int)); 		// pre-process date
myfmt='i'
bin=struct.pack(myfmt,version_id) #
ofile.write(bin)

# data_file.write(reinterpret_cast<char*>(&phantom_name_size), sizeof(int));
myfmt='i'
bin=struct.pack(myfmt,phantom_name_size) #
ofile.write(bin)

# data_file.write(PHANTOM_NAME, phantom_name_size); 							// phantom name or description (string)
myfmt='s'*phantom_name_size
bin=struct.pack(myfmt,*PHANTOM_NAME) #
ofile.write(bin)

# data_file.write(reinterpret_cast<char*>(&data_source_size), sizeof(int));
myfmt='i'
bin=struct.pack(myfmt,data_source_size) #
ofile.write(bin)

# data_file.write(InputFileName, data_source_size); 							// data source (string)
myfmt='s'*data_source_size
bin=struct.pack(myfmt,*iFileName) #
ofile.write(bin)

# data_file.write(reinterpret_cast<char*>(&prepared_by_size), sizeof(int));
myfmt='i'
bin=struct.pack(myfmt,prepared_by_size) #
ofile.write(bin)

# data_file.write(PREPARED_BY, prepared_by_size);								 // prepared by (string)
myfmt='s'*prepared_by_size
bin=struct.pack(myfmt,*PREPARED_BY) #
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

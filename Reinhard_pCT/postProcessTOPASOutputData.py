import numpy as np 
import struct
import sys
import time

try:
        N = sys.argv[1]
        ofileName = sys.argv[2]
        Nh = int(sys.argv[3])

except:
        print "Use: python", sys.argv[0], " MidName OutputFileName NbOfHistories RandomSeed"
        exit(1)

def GetData(fileName, NbOfHistories):
    dtype = 'i,f'
    idata  = np.fromfile(fileName,dtype=dtype)
    data = np.zeros((NbOfHistories,3))
    evts = idata['f0']
    data[evts,0] = 1    # Hit or not
    data[evts,1] = idata['f0'] # evtID
    data[evts,2] = idata['f1'] # hit info (edep or position)
    evts = []
    idata = []
    print 'Read file:', fileName
    return data

def GetDataPhSp(fileName, NbOfHistories):
        dtype = 'f,f,f,f,f,f,f,i,b,b,i,i'
        idata = np.fromfile(fileName,dtype=dtype)
        data = np.zeros((NbOfHistories,5))
        idata = idata[ np.where( idata['f11'] == 0 ) ]
        evts = idata['f10']
        data[evts,0] = 1
        data[evts,1] = idata['f10']
        data[evts,2] = idata['f0']*10.0
        data[evts,3] = idata['f1']*10.0
        data[evts,4] = idata['f2']*10.0
        evts = []
        idata = []
        print 'Read file:', fileName
        return data

time_start = time.clock()



#Plane 1
p1tIn  = GetDataPhSp('StepCalib/Tracker1t_In_StepPhantom_Brick_'+N+'.phsp',Nh)
p1tOut = GetDataPhSp('StepCalib/Tracker1t_Out_StepPhantom_Brick_'+N+'.phsp',Nh)
p1vIn  = GetDataPhSp('StepCalib/Tracker1v_In_StepPhantom_Brick_'+N+'.phsp',Nh)
p1vOut = GetDataPhSp('StepCalib/Tracker1v_Out_StepPhantom_Brick_'+N+'.phsp',Nh)

#Plane 2
p2tIn  = GetDataPhSp('StepCalib/Tracker2t_In_StepPhantom_Brick_'+N+'.phsp',Nh)
p2tOut = GetDataPhSp('StepCalib/Tracker2t_Out_StepPhantom_Brick_'+N+'.phsp',Nh)
p2vIn  = GetDataPhSp('StepCalib/Tracker2v_In_StepPhantom_Brick_'+N+'.phsp',Nh)
p2vOut = GetDataPhSp('StepCalib/Tracker2v_Out_StepPhantom_Brick_'+N+'.phsp',Nh)

#Plane 3
p3tIn  = GetDataPhSp('StepCalib/Tracker3t_In_StepPhantom_Brick_'+N+'.phsp',Nh)
p3tOut = GetDataPhSp('StepCalib/Tracker3t_Out_StepPhantom_Brick_'+N+'.phsp',Nh)
p3vIn  = GetDataPhSp('StepCalib/Tracker3v_In_StepPhantom_Brick_'+N+'.phsp',Nh)
p3vOut = GetDataPhSp('StepCalib/Tracker3v_Out_StepPhantom_Brick_'+N+'.phsp',Nh)

#Plane 4
p4tIn  = GetDataPhSp('StepCalib/Tracker4t_In_StepPhantom_Brick_'+N+'.phsp',Nh)
p4tOut = GetDataPhSp('StepCalib/Tracker4t_Out_StepPhantom_Brick_'+N+'.phsp',Nh)
p4vIn  = GetDataPhSp('StepCalib/Tracker4v_In_StepPhantom_Brick_'+N+'.phsp',Nh)
p4vOut = GetDataPhSp('StepCalib/Tracker4v_Out_StepPhantom_Brick_'+N+'.phsp',Nh)

#
e0 = GetData('StepCalib/DegraderTrack_RawTuple_float64.bin',Nh)
e1 = GetData('StepCalib/Step1Track_RawTuple_float64.bin',Nh)
e2 = GetData('StepCalib/Step2Track_RawTuple_float64.bin',Nh)
e3 = GetData('StepCalib/Step3Track_RawTuple_float64.bin',Nh)
ee = GetData('StepCalib/EdepScintillator1_StepPhantom_Brick_1_RawTuple_float64.bin',Nh)

ofile = open(ofileName,'w')

Z_C = array([-214.3, -164.3, 164.3, 214.3])

n=0
chunk = Nh/10
totalLines = 0
for i in xrange(Nh):
        if ee[i,0] == 1 and\
           p1tOut[i,0] == 1 and p1vOut[i,0] == 1 and p2tOut[i,0] == 1 and p2vOut[i,0] == 1 and\
           p3tOut[i,0] == 1 and p3vOut[i,0] == 1 and p4tOut[i,0] == 1 and p4vOut[i,0] == 1:
                ofile.write('%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f  \n' % (\
                                        p1vOut[i,3], p1tOut[i,2], Z_C[0],\
                                        p2vOut[i,3], p2tOut[i,2], Z_C[1],\
                                        p3vOut[i,3], p3tOut[i,2], Z_C[2],\
                                        p4vOut[i,3], p4tOut[i,2], Z_C[3],\
                                        e0[i,2],e1[i,2],e2[i,2], e3[i,2]))
                totalLines += 1

        if i % chunk == 0:
                print 'Processed chunk ', n , 'of 10.'
                n+=1
ofile.close()
time_end = time.clock() - time_start
print "Total time: ", time_end, " (s)"
print "Total particles accepted: ", totalLines



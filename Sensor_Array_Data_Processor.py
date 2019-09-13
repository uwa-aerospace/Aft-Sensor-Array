"""
===========================================

      Aft Sensor Array Data Processor 
              by Toby Digney

===========================================
"""

import numpy as np

def process(flightlog):
    
    Vs = 5 #Source voltage
    Ro = 100 #Offset resistor for RTDs
    R = 1000 #Current limiting resistor for RTDs
    TC = 0.3851 #RTD temperature coefficent
    GF = 2.09 #Strain gauge sensitivity (gauge factor)
    
    C=(Vs*Ro)/(Ro+R) 
    
    #Open flightlog file as a structured array
    with open(flightlog, 'r') as file:
        data = np.genfromtxt(file, delimiter=',',dtype=[('Time', float),('RTD1', float),('RTD2', float),('RTD3', float),('RTDR', float),('SG1', float),('SG2', float),('SGR', float),('X', float),('Y', float),('Z', float)])
    
    #Create empty structured arrays for the different stages of the arithmrtic processes
    V = np.empty((len(data)),dtype=[('Time', float),('RTD1', float),('RTD2', float),('RTD3', float),('RTDR', float),('SG1', float),('SG2', float),('SGR', float)])
    dV = np.empty((len(data)),dtype=[('Time', float),('RTD1', float),('RTD2', float),('RTD3', float),('SG1', float),('SG2', float)])
    Rx = np.empty((len(data)),dtype=[('Time', float),('RTD1', float),('RTD2', float),('RTD3', float)])
    values = np.empty((len(data)),dtype=[('Time', float),('RTD1', float),('RTD2', float),('RTD3', float),('SG1', float),('SG2', float),('X', float),('Y', float),('Z', float)])
    
    #Convert 10 bit digital voltage readings into analog voltages for all samples
    V['Time'] = data['Time']
    V['RTD1'] = ((data['RTD1']+0.5)*3.3/1024)
    V['RTD2'] = ((data['RTD2']+0.5)*3.3/1024)
    V['RTD3'] = ((data['RTD3']+0.5)*3.3/1024)
    V['RTDR'] = ((data['RTDR']+0.5)*3.3/1024)
    V['SG1'] = ((data['SG1']+0.5)*3.3/1024)
    V['SG2'] = ((data['SG2']+0.5)*3.3/1024)
    V['SGR'] = ((data['SGR']+0.5)*3.3/1024)
    
    #Calculate the differential voltage accross each wheatstone brigde for each sample
    dV['Time'] = data['Time']
    dV['RTD1'] = V['RTD1']-V['RTDR']
    dV['RTD2'] = V['RTD2']-V['RTDR']
    dV['RTD3'] = V['RTD3']-V['RTDR']
    dV['SG1'] = V['SG1']-V['SGR']
    dV['SG2'] = V['SG2']-V['SGR']
    
    #Calculate the resistance of each RTD for each sample
    Rx['Time'] = data['Time']
    Rx['RTD1'] = (R*(dV['RTD1']+C))/(Vs-dV['RTD1']-C)
    Rx['RTD2'] = (R*(dV['RTD2']+C))/(Vs-dV['RTD2']-C)
    Rx['RTD3'] = (R*(dV['RTD3']+C))/(Vs-dV['RTD3']-C)
    
    #Calculate the values of physical phenomena: Time: (seconds), RTDs: (temperature in celcius), SGs: (strain), XYZ: (acceleration in Gs)
    values['Time'] = data['Time']/1000
    values['RTD1'] = (Rx['RTD1']-100)/TC
    values['RTD2'] = (Rx['RTD2']-100)/TC
    values['RTD3'] = (Rx['RTD3']-100)/TC
    values['SG1'] = (dV['SG1']*4)/(GF*Vs)
    values['SG2'] = (dV['SG2']*4)/(GF*Vs)
    values['X'] = ((data['X']-511.5)*200/512)
    values['Y'] = ((data['Y']-511.5)*200/512)
    values['Z'] = ((data['Z']-511.5)*200/512)
    
    #Save the array of physical values as a csv file
    np.savetxt('processed_flightlog.csv',values,delimiter=',')
    
    return
    
test = 'Test_Data.csv'
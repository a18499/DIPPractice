import matplotlib.pyplot as plt
from math import exp,pow
from matplotlib.mlab import normpdf
import numpy as np
import pylab as p
def contractstretching(M,E,R):
    Out=[]
    for eachr in R:
        if(eachr==0):
            Out.append(0)
            continue
        x = float(M/eachr)
        S= 1/(1+pow(x,E))
        Out.append(S)
    return Out

L=256
M = L/2
print(L)
print(M)


R = np.arange(0,256,0.1)
E  = np.arange(1,40,5)
for eache in E:
    Output = contractstretching(M,eache,R)
    p.plot(R,Output, lw=2)

p.xlim(0,255)
p.ylim(0,1)
p.ylabel("S")
p.xlabel("r")
p.show()


Output = contractstretching(M,50,R)
p.plot(R,Output, lw=2)

p.xlim(0,255)
p.ylim(0,1)
p.ylabel("S")
p.xlabel("r")
p.show()

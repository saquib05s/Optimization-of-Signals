#!/usr/bin/env python3
import sys
import re
import math
from typing import final
#import typing_extensions

# YOUR CODE GOES HERE
inputlist=[] # To store inputs (after splitting)
coorlist=[]  # To store intersecting coordinates
streetsdict={} # To store streets info (Name and coordinate)
mightvd={}      # Final To store vertices
mightvd_index=1 #Vertices index
edges=[]   #To store temporary edges
tempset={}        # Final To store edges
tempset=set(tempset)

def clear():
    global inputlist, coorlist, mightvd, mightvd_index, edges, tempset
    inputlist.clear()
    coorlist.clear()
    edges.clear()
    tempset.clear()
    mightvd.clear()
    mightvd_index=1

def intersect(l1, l2):
    global mightvd, mightvd_index, edges
    # Removing brackets and adding sign(-) if present in a number
    for i in range(len(l1)):
        st=""
        l1[i]=list(l1[i])
        c1, c2=0, 0
        if "(" in l1[i]:
            l1[i].remove("(")
            l1[i].remove(")")
            c1=1

        if c1==1:
            for elem in l1[i]:
                st+=str(elem)

            l1[i]=st.split(",")
    
    for i in range(len(l2)):
        st=""
        l2[i]=list(l2[i])
        if "(" in l2[i]:
            l2[i].remove("(")
            l2[i].remove(")")
            c2=1
            
        if c2==1:
            for elem in l2[i]:
                st+=str(elem)

            l2[i]=st.split(",")
    
    # Converting elements to int    
    for i in range(len(l1)):
        for j in range(len(l1)):
            l1[i][j]=int(l1[i][j])
    for i in range(len(l2)):
        for j in range(len(l2)):
            l2[i][j]=int(l2[i][j])
    
    x1, y1 = l1[0][0], l1[0][1]
    x2, y2 = l1[1][0], l1[1][1]
    x3, y3 = l2[0][0], l2[0][1]
    x4, y4 = l2[1][0], l2[1][1]
    
    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    if xden==0 or yden==0:
        return
    else:
        xcoor = xnum / xden
        ycoor = ynum / yden

        l1_total_dist=math.sqrt(pow(x2-x1,2)+pow(y2-y1,2))          #l1 coordinates total distance
        l2_total_dist=math.sqrt(pow(x4-x3,2)+pow(y4-y3,2))          #l2 coordinates total distance

        l1_coor1_dist=math.sqrt(pow(xcoor-x1,2)+pow(ycoor-y1,2))    #l1 intersect points distance from one point
        l1_coor2_dist=math.sqrt(pow(xcoor-x2,2)+pow(ycoor-y2,2))    #l1 intersect points distance from another point
        l2_coor1_dist=math.sqrt(pow(xcoor-x3,2)+pow(ycoor-y3,2))    #l2 intersect points distance from one point
        l2_coor2_dist=math.sqrt(pow(xcoor-x4,2)+pow(ycoor-y4,2))    #l2 intersect points distance from another point

        if (l1_coor1_dist <= l1_total_dist and l1_coor2_dist <= l1_total_dist and l2_coor1_dist <= l2_total_dist and l2_coor2_dist <= l2_total_dist):
            
            tupcoor = (xcoor, ycoor)
            
            """xcoor = float("{0:.2f}".format(xcoor))
            ycoor = float("{0:.2f}".format(ycoor))
            tupcoor = (xcoor, ycoor)"""
            
            if tupcoor not in coorlist:
                coorlist.append(tupcoor)

            if tupcoor not in mightvd.values():
                mightvd[mightvd_index]=tupcoor
                mightvd_index+=1
            
            l1[0]=tuple(l1[0])
            if l1[0] not in mightvd.values():
                mightvd[mightvd_index]=l1[0]
                mightvd_index+=1
            
            l1[1]=tuple(l1[1])
            if l1[1] not in mightvd.values():
                mightvd[mightvd_index]=l1[1]
                mightvd_index+=1
            
            l2[0]=tuple(l2[0])
            if l2[0] not in mightvd.values():
                mightvd[mightvd_index]=l2[0]
                mightvd_index+=1

            l2[1]=tuple(l2[1])
            if l2[1] not in mightvd.values():
                mightvd[mightvd_index]=l2[1]
                mightvd_index+=1

            # Edges
            tupcoor_index=list(mightvd.keys())[list(mightvd.values()).index(tupcoor)]
            l1_0_index=list(mightvd.keys())[list(mightvd.values()).index(l1[0])]
            l1_1_index=list(mightvd.keys())[list(mightvd.values()).index(l1[1])]
            l2_0_index=list(mightvd.keys())[list(mightvd.values()).index(l2[0])]
            l2_1_index=list(mightvd.keys())[list(mightvd.values()).index(l2[1])]

            tupcoor_l1_0=(tupcoor_index, l1_0_index)
            if tupcoor_l1_0 not in edges and tupcoor_index!=l1_0_index and (l1_0_index, tupcoor_index) not in edges:
                edges.append(tupcoor_l1_0)
            
            tupcoor_l1_1=(tupcoor_index, l1_1_index)
            if tupcoor_l1_1 not in edges and tupcoor_index!=l1_1_index and (l1_1_index, tupcoor_index) not in edges:
                edges.append(tupcoor_l1_1)
            
            tupcoor_l2_0=(tupcoor_index, l2_0_index)
            if tupcoor_l2_0 not in edges and tupcoor_index!=l2_0_index and (l2_0_index, tupcoor_index) not in edges:
                edges.append(tupcoor_l2_0)
            
            tupcoor_l2_1=(tupcoor_index, l2_1_index)
            if tupcoor_l2_1 not in edges and tupcoor_index!=l2_1_index and (l2_1_index, tupcoor_index) not in edges:
                edges.append(tupcoor_l2_1)

            return True
        else:
            return

def computeedges():
    global tempset, edges, mightvd, coorlist
    delset = {}
    delset=set(delset)
    mighted=edges

    while (len(mighted))!=0:
        a, b = mightvd[mighted[0][0]], mightvd[mighted[0][1]]
        x1, y1, x2, y2 = a[0], a[1], b[0], b[1]

        if (mighted[0][0], mighted[0][1]) and (mighted[0][1], mighted[0][0]) not in tempset:
            tempset.add((mighted[0][0], mighted[0][1]))

        for c in coorlist:
            if c!=a and c!=b:
                x, y = c[0], c[1]

                if x2-x1==0:
                    if x==x2:
                        c_on=True
                    else:
                        c_on=False
                else:
                    slope = (y2 - y1) / (x2 - x1)
                    c_on = float("{0:.2f}".format(y - y1)) == float("{0:.2f}".format(slope * (x- x1)))

                c_between = (min(x1, x2) <= x <= max(x1, x2)) and (min(y1, y2) <= y <= max(y1, y2))

                if c_on and c_between:
                    if c in mightvd.values():
                        temp=list(mightvd.keys())[list(mightvd.values()).index(c)]
                        mighted.append((mighted[0][0], temp))
                        mighted.append((mighted[0][1], temp))

                        delset.add(mighted[0])

        mighted.remove(mighted[0])
    
    tempset=tempset-delset 
    return True

def computevetrices():
    global streetsdict
    streets=[]
    for st_name in streetsdict:
        streets.append(st_name)
    for i in range (len(streets)-1):
        for j in range(i+1, len(streets)):
            
            for a in range(len(streetsdict[streets[i]])-1):
                l1=[streetsdict[streets[i]][a],streetsdict[streets[i]][a+1]]

                for b in range(len(streetsdict[streets[j]])-1):
                    l2=[streetsdict[streets[j]][b],streetsdict[streets[j]][b+1]]
                    intersect(l1,l2)
    return

def checkcontd(line):
    input=line.split(" ")
    input[len(input)-1] = input[len(input)-1].replace("\n", "")
    for i in range(2, len(input)-1):
        if (input[i]==input[i+1]):
            print ("Error: Consecutive same coordinates")
            return False
    else:
        return True

def checkinput(line):
    global streetsdict
    reg=r'(add\s|mod\s)("[A-Za-z]+(\s[A-Za-z]+)*")((((\s\((-?0*[1-9]+\d*|0*),(-?0*[1-9]+\d*|0*)\)){2,})$))|(rm\s)("[A-Za-z]+(\s[A-Za-z]+)*"$)|(gg$)'

    prog=re.match(reg, line)
    if prog==None:
        return False
    else:
        if checkcontd(line):
            return True
        else:
            return False
        
            
def add():
    global inputlist, streetsdict
    if inputlist[1].lower() in streetsdict:
        print ("Error: Street has already been added")
        return False
    else:
        l=inputlist[2].strip().split(" ") #type is string
        streetsdict[inputlist[1].lower()]=l
        return True

def mod():
    global inputlist, streetsdict
    if inputlist[1].lower() in streetsdict:
        l=inputlist[2].strip().split(" ")
        streetsdict[inputlist[1].lower()]=l
        return True
    else:
        print ("Error: Street does not exist")
        return False

def rm():
    global inputlist, streetsdict
    if inputlist[1].lower() in streetsdict:
        del streetsdict[inputlist[1].lower()]
        return True
    else:
        print ("Error: Street does not exist")
        return False

def verticesoutput(): 
    global mightvd
    V="V "+str(len(mightvd))
    for m in mightvd.keys():
        temptup = mightvd[m]
        g, h = temptup[0], temptup[1]

        g=float("{0:.2f}".format(g))
        h=float("{0:.2f}".format(h))

        if int(g)==float(g): #5==5.0
            g = int(g)
        else:
            g=float("{0:.2f}".format(g))
        if int(h)==float(h):
            h = int(h)
        else:
            h=float("{0:.2f}".format(h))
        temptup=(g, h)
        mightvd[m] = temptup

    finalvertices=str(mightvd)
    if mightvd=={}:
        finalvertices=finalvertices.replace("{", "{\n")
    else:
        finalvertices=finalvertices.replace("), ", ")\n")
        finalvertices=finalvertices.replace(", ",",")
        finalvertices=finalvertices.replace("{", "{\n")
        finalvertices=finalvertices.replace("}", "\n}")

    #return finalvertices
    return V

def finaledges():
    global tempset
    if len(tempset)==0:
        finaledges="{\n}"
        Estr="{}"
    else:
        finaledges=str(tempset)
        Estr=str(tempset)
        Estr=Estr.replace("), ", ")")
        Estr=Estr.replace(", ",",")
        Estr=Estr.replace(")", ">,")
        Estr=Estr.replace(",}", "}")
        Estr=Estr.replace("(", "<")
    
        finaledges=finaledges.replace("), ", ")\n")
        finaledges=finaledges.replace(", ",",")
        finaledges=finaledges.replace(")", ">,")
        finaledges=finaledges.replace("{", "{\n")
        finaledges=finaledges.replace(",}", "\n}")
        finaledges=finaledges.replace("(", "<")
    E = " ".join(("E",Estr))
    #return finaledges
    return E

def gg():
    global mightvd, tempset
    computevetrices()
    sys.stdout.write(verticesoutput()+"\n")
    computeedges()
    #sys.stdout.write(verticesoutput()+"\n")
    sys.stdout.flush()
    sys.stdout.write(finaledges()+"\n")
    sys.stdout.flush()
    clear()
    return True

def split(line):
    global inputlist
    inputlist=[]
    inputlist=line.split("\"")
    if inputlist[0]=="add ":
        output=add()
        if output==False:
            return False
    elif inputlist[0]=="mod ":
        output=mod()
        if output==False:
           return False
    elif inputlist[0]=="rm ":
        output=rm()
        if output==False:
            return False
    elif inputlist[0]=="gg\n":
        output=gg()
        if output==False:
            return False
    
def main():
    # YOUR MAIN CODE GOES HERE

    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment
    global count
    while True:
        line = sys.stdin.readline()
        if line == "":
            break

        inputcheck=checkinput(line)
        if inputcheck:
            mainoutput=split(line)
            #if mainoutput==False:
            #    continue
        else:
            print ("Error: Invalid Input")
        
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == "__main__":
    main()

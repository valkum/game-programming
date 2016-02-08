import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial import ConvexHull
from matplotlib.path import Path
import json
import random
import math

class Skyscraper(object):
    def __init__(self, positionX, positionY):
        self.model = "SkyScraper.obj"
        self.scale = random.uniform(0.7, 1.4)
        self.position = [positionX, 0.0, positionY]
        self.rotation = [random.uniform(math.radians(-15), math.radians(15)), 0.0, 0.0]

def in_circle(center_x, center_y, list, radius):
    for i in xrange(len(list)):
        dist = math.sqrt((center_x - list[i][0]) ** 2 + (center_y - list[i][1]) ** 2)
        if(dist < radius):
            #print str(list[i][0]) + "\t" + str(list[i][1]) + "\t" + "fuck"
            return True
    return False

width = 256
height = 1024
N = 2*4096

r1 = np.random.rand(N/3)
r2 = np.random.rand(N/3)

x = [(-(width/2) * r1[i]) + (width * r2[i]) for i in xrange(N/3)]
y = [(height*r1[i]/3) for i in xrange(N/3)]

hull_path = Path([(0,0), (-0.5*width, 1*height/3), (0.5*width, 1*height/3)])

list = [(x[i],y[i]) for i in xrange(N/3) if hull_path.contains_point((x[i],y[i]))]
sortedListShortened = [var for var in list if var[1] > 40]

blockList = [(random.uniform(-width/2, width/2), random.uniform(height/3, height)) for i in xrange(2*N/3)]

temp = sortedListShortened + blockList
sortedList = sorted(temp, key=lambda x: x[1])
listLen = len(sortedList)
sortedListFinal = [sortedList[i] for i in xrange(listLen-1) if not (in_circle(sortedList[i][0], sortedList[i][1], sortedList[i+1:listLen], 5))]

colors = np.random.rand(N)
area = 1

plt.scatter(np.asarray([finalX[0] for finalX in temp]), np.asarray([finalY[1] for finalY in temp]), color='r' ,s=area, alpha=0.5)
plt.scatter(np.asarray([finalX[0] for finalX in sortedListFinal]), np.asarray([finalY[1] for finalY in sortedListFinal]), s=area, alpha=0.5)
plt.show()

#scraperList = [Skyscraper(x[i], y[i]) for i in xrange(N)]
scraperList = [Skyscraper(sortedListFinal[i][0], sortedListFinal[i][1]) for i in xrange(len(sortedListFinal))]

def jdefault(o):
    return o.__dict__

#out = json.dumps(scraperList, default=jdefault)
pretty = json.dumps(scraperList, sort_keys=True, indent=4, default=jdefault, separators=(',', ': '))

f = open('fffff-00001.lvl', 'w')
f.write(pretty);
f.close()
print "Created level.lvl"

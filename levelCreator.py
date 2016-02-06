
"""
Simple demo of a scatter plot.
"""
import numpy as np
import matplotlib.pyplot as plt
import json

class Skyscraper(object):
    def __init__(self, positionX, positionY):
        self.model = "SkyScraper.obj"
        self.scale = 0.5
        self.position = [positionX, 0.0, positionY]
        self.rotation = [0.0, 0.0, 0.0]

N = 1024
x = 256*np.random.rand(N)
y = 1024*np.random.rand(N)
colors = np.random.rand(N)
#area = np.pi * (15 * np.random.rand(N))**2  # 0 to 15 point radiuses
area = 1

plt.scatter(x, y, s=area, c=colors, alpha=0.5)
plt.show()

scraperList = [Skyscraper(x[i], y[i]) for i in xrange(1024)]

def jdefault(o):
    return o.__dict__

#out = json.dumps(scraperList, default=jdefault)
pretty = json.dumps(scraperList, sort_keys=True, indent=4, default=jdefault, separators=(',', ': '))

#f = open('level.json', 'w')
#f.write(out + '\n');
#f.close()
print pretty

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
from mininet.node import OVSController
import time
from matplotlib import pyplot as plt
import numpy as np

class CustomTopo(Topo):
    def __init__(self, n_generals):
        Topo.__init__(self)

        s1 = self.addSwitch('s1')

        # criacao dos hosts e links
        for i in range(1, n_generals + 1):
            hostname = "h" + str(i)
            host = self.addHost(hostname)

            self.addLink(host, s1)

topos = {'customtopo': (lambda: CustomTopo())}

def decision(i):
    with open("log/gen" + i + ".out", "rb") as f:
        first = f.readline()      # Read the first line.
        f.seek(-2, 2)             # Jump to the second last byte.
        while f.read(1) != b"\n": # Until EOL is found...
            f.seek(-2, 1)         # ...jump back the read byte plus one more.
        last = f.readline()       # Read last line.
        return last

def launch(n_generals, n_traitors):
    topo = CustomTopo(n_generals)
    net = Mininet(topo = topo, controller = OVSController)
    net.start()

    prog_name = "./bin/byz "

    pids = {}
    for i in range(1, n_generals + 1):
        hostname = "h" + str(i)
        ne = net.get(hostname)

        description = ""
        if i is n_generals:
            description = " (commander general)"
        else:
            description = " (lieutenant general)"

        print ("starting " + hostname + description)

        #Commander has id 0
        general_id = i % n_generals

        prog_args = str(general_id) + " " + str(n_generals) + " " + str(n_traitors)
        prog_other = " > log/gen" + str(general_id) + ".out &"

        command = prog_name + prog_args + prog_other
        ne.cmd(command)

        pids[i-1] = int(ne.cmd('echo $!'))

    for i in range(1, n_generals):
        hostname = "h" + str(i)
        ne = net.get(hostname)
        ne.cmd('wait', pids[i-1])

    net.stop()

    for i in range(n_traitors + 1, n_generals):
        print ("LT " + str(i) + ": " + decision(str(i)))

'''
Uses brute force to find the minimum number of generals
given m traitors and prints the elapsed time.
'''
def find_first_agreement(m):
    start = time.time()

    print ("Finding first agreement for M = " + str(m) + "\n")
    for i in range(m+1, 3*m+2):
        launch(i, m)

    elapsed = time.time() - start
    print ("Agreement reached in " + str(elapsed) + " seconds\n")
    return elapsed

def find_first_agreement2(n_traitors):

    max_generals = (3* n_traitors) + 4
    for n_generals in range(n_traitors, max_generals + 1):
        start = time.time()

        launch(n_generals, n_traitors)

        elapsed = time.time() - start
        print ("Agreement reached in " + str(elapsed) + " seconds\n")
        yield elapsed

'''
Finds the elapsed time for the byzantine generals
problem given n generals and m traitors
'''
def time_for_agreement(m):
    start = time.time()

    n = (3 * m) + 1

    print ("Finding agreement with " + str(n) + " generals and " + str(m) + " traitors.\n")
    launch(n, m)

    elapsed = time.time() - start
    print ("Agreement was reached in " + str(elapsed) + " seconds\n")

    return elapsed

def draw_init(x):
    plt.xticks(x)

def draw_add(x, y, draw_value):
    plt.plot(x, y)

    if draw_value is True:
        for i, j in zip(x, y):
            plt.text(i, j, str(j))

def draw_save(filename):
    plt.ylabel('time (s)')
    plt.xlabel('number of generals')

    #plt.show()

    plt.savefig(filename)
    plt.clf()

def draw(x, y, name):
    plt.xticks(x)
    plt.plot(x, y)
    plt.ylabel('time (s)')
    plt.xlabel('number of traitors')
    for i, j in zip(x, y):
        plt.text(i, j, str(j))

    plt.savefig(name)
    plt.clf()

if __name__ == '__main__':
    # Tell mininet to print useful information
    #setLogLevel('info')

    '''
    Experiment 1
    '''

    '''
    min_traitors = 2
    max_traitors = 4

    x = range(min_traitors, max_traitors + 1)
    y = []

    for i in x:
        elapsed_time = find_first_agreement(i)
        y.append(elapsed_time)

    draw(x, y, "experiment1.png")
    '''
    '''
    Experiment 2
    '''
    '''
    min_traitors = 1
    max_traitors = 4

    x = range(min_traitors, max_traitors + 1)
    y = []

    for i in x:
        elapsed_time = time_for_agreement(i)
        y.append(elapsed_time)

    draw(x, y, "experiment2.png")
    '''
    '''
    Experiment 3
    '''
    min_traitors = 2
    max_traitors = 4

    z = []
    rounds = range(min_traitors, max_traitors + 1)
    for n_traitors in rounds:

        n_generals = (3 * n_traitors) + 4

        x = range(n_traitors, n_generals + 1)
        y = []

        draw_init(x)

        for elapsed_time in find_first_agreement2(n_traitors):
            y.append(elapsed_time)

        z.append(y)

        draw_add(x, y, True)
        draw_save("m" + str(n_traitors) + ".png")



    k = 0
    for i in rounds:
        x = range(i, (3 * i) + 5)
        draw_add(x, z[k], False)
        k+=1

    draw_save("experiment3.png")

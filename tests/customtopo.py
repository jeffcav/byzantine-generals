from mininet.topo import Topo
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
from mininet.node import OVSController
import time
from matplotlib import pyplot as plt

'''
Creates a star topology in mininet.
Even though there are built-in classes for this, I wanted to learn
how to do it by hand =)
'''
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

'''
Reads the log file created by each general and returns its final decision as a string
'''
def decision(i):
    with open("log/gen" + i + ".out", "rb") as f:
        first = f.readline()      # Read the first line.
        f.seek(-2, 2)             # Jump to the second last byte.
        while f.read(1) != b"\n": # Until EOL is found...
            f.seek(-2, 1)         # ...jump back the read byte plus one more.
        last = f.readline()       # Read last line.
        return last

'''
Instanciates a mininet network with given parameters and runs
the byzantine generals process in each container
'''
def launch_generals_network(n_generals, n_traitors):
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
Creates networks varying the number of generals based on a given
ammount of traitors
'''
def create_test_instances(n_traitors):

    max_generals = calculate_max_generals(n_traitors)
    for n_generals in range(n_traitors, max_generals + 1):
        start = time.time()

        print ("Launching " + str(n_generals) + " generals with " + str(n_traitors) + " traitors...")

        launch_generals_network(n_generals, n_traitors)

        elapsed = time.time() - start
        print ("Agreement reached in " + str(elapsed) + " seconds\n")
        yield elapsed

'''
Graph plotting utilities
'''
def draw_init(x):
    plt.xticks(x)

def draw_add(x, y, write_value):
    plt.plot(x, y)

    if write_value is True:
        for i, j in zip(x, y):
            plt.text(i, j, str(j))

def draw_save(filename, show_draw):
    plt.ylabel('time (s)')
    plt.xlabel('number of generals')

    if show_draw is True:
        plt.show()

    plt.savefig(filename)
    plt.clf()

'''
Returns a maximum suggested number of generals for a test.

This function must return at least a value of (3*m)+1, but
as we fixed m={1, 2, 3, 4}, 16 is aways a good value and
being fixed helps us doing comparisons.
'''
def calculate_max_generals(m):
    return 16

if __name__ == '__main__':
    # Tell mininet to print useful information
    #setLogLevel('info')

    min_traitors = 1
    max_traitors = 4

    all_elapsed_times = []
    traitors_range = range(min_traitors, max_traitors + 1)
    for n_traitors in traitors_range:
        n_generals = calculate_max_generals(n_traitors)

        #variables for plotting
        generals_range = range(n_traitors, n_generals + 1)
        elapsed_times = []
        draw_init(generals_range)

        #runnung tests and measuring their execution times
        for elapsed_time in create_test_instances(n_traitors):
            elapsed_times.append(elapsed_time)

        all_elapsed_times.append(elapsed_times)

        #plotting execution times and saving into a file
        draw_add(generals_range, elapsed_times, True)
        draw_save("tests/img/m" + str(n_traitors) + ".png", False)


    #Creating a graph which draws all other tests togheter, helping with comparisons
    n_traitors = 1
    draw_init(range(1, calculate_max_generals(max_traitors)+1))

    for i in traitors_range:
        generals_range = range(i, calculate_max_generals(i)+1)
        draw_add(generals_range, all_elapsed_times[n_traitors-1], False)
        n_traitors += 1

    draw_save("tests/img/experiment.png", True)

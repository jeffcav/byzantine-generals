from mininet.topo import Topo
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
from mininet.node import OVSController
from time import sleep

class CustomTopo(Topo):
    def __init__(self, n_generals):
        Topo.__init__(self)

        s1 = self.addSwitch('s1')

        # criacao dos hosts e links
        for i in range(1, n_generals + 1):
            hostname = "h" + str(i)
            host = self.addHost(hostname)

            self.addLink(host, s1)

            print hostname + " added"

topos = {'customtopo': (lambda: CustomTopo())}

def decision(i):
    with open("tmp/gen" + i + ".out", "rb") as f:
        first = f.readline()      # Read the first line.
        f.seek(-2, 2)             # Jump to the second last byte.
        while f.read(1) != b"\n": # Until EOL is found...
            f.seek(-2, 1)         # ...jump back the read byte plus one more.
        last = f.readline()       # Read last line.
        return last

def run_environment(n_generals, n_traitors):
    topo = CustomTopo(n_generals)
    net = Mininet(topo = topo, controller = OVSController)
    net.start()

    prog_name = "./../out/Debug/byzantine_generals "

    pids = {}
    for i in range(1, n_generals + 1):
        hostname = "h" + str(i)
        ne = net.get(hostname)

        print "starting " + hostname

        #Commander has id 0
        general_id = i % n_generals

        prog_args = str(general_id) + " " + str(n_generals) + " " + str(n_traitors)
        prog_other = " > tmp/gen" + str(general_id) + ".out &"

        command = prog_name + prog_args + prog_other
        ne.cmd(command)

        pids[i-1] = int(ne.cmd('echo $!'))
        sleep(1)

    for i in range(1, n_generals):
        hostname = "h" + str(i)
        ne = net.get(hostname)
        ne.cmd('wait', pids[i-1])

    sleep(2)
    net.stop()

    for i in range(n_traitors + 1, n_generals):
        print "LT " + str(i) + ": " + decision(str(i))

if __name__ == '__main__':
    # Tell mininet to print useful information
    setLogLevel('info')
    run_environment(3, 1)

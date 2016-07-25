from mininet.topo import Topo
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
from mininet.node import OVSController
from time import sleep

class CustomTopo(Topo):
    def __init__(self):
        Topo.__init__(self)

        # criacao dos hosts
        ne1 = self.addHost('ne1')
        ne2 = self.addHost('ne2')
        ne3 = self.addHost('ne3')
        ne4 = self.addHost('ne4')

        # criacao dos switches
        s1 = self.addSwitch('s1')

        # criacao dos links
        self.addLink(ne1, s1)
        self.addLink(ne2, s1)
        self.addLink(ne3, s1)
        self.addLink(ne4, s1)


topos = {'customtopo': (lambda: CustomTopo())}


def simpleTest():
    "Create and test a simple network"
    topo = CustomTopo()
    net = Mininet(topo = topo, controller = OVSController)
    net.start()

    net.pingAll()

    prog = "./../out/Debug/byzantine_generals "

    for i in range(1, 5):
        hostname = "ne" + str(i)
        ne = net.get(hostname)

        general = i % 4
        ne.cmd(prog + str(general) + " > tmp/gen" + str(general) + ".txt &")

    sleep(6)
    net.stop()

if __name__ == '__main__':
    # Tell mininet to print useful information
    setLogLevel('info')
    simpleTest()
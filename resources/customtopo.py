from mininet.topo import Topo
class CustomTopo(Topo):
	def __init__(self):
		Topo.__init__(self)
		
		#criacao dos hosts
		ne1 = self.addHost('ne1')
		ne2 = self.addHost('ne2')
		ne3 = self.addHost('ne3')
		ne4 = self.addHost('ne4')

		#criacao dos switches
		s1 = self.addSwitch('s1')

		#criacao dos links
		self.addLink(ne1, s1)
		self.addLink(ne2, s1)
		self.addLink(ne3, s1)
		self.addLink(ne4, s1)

topos = {'customtopo': (lambda: CustomTopo())}


# PhotonLEDControl
Sketch which exposes multiple functions that control RGB LEDs wired through a Particle Photon

**Anywhere that a "hexcolor" is requested needs to have a 3-part hex value representing a color (e.g. red=ff0000, green=00ff00, blue=0000ff)**

Functions:
ledhiccup(String hexcolor)
ledcycle()
ledsetvalue(String hexcolor)
ledsetcolor(String hexcolor)

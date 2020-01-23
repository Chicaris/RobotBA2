import serial
from struct import unpack, pack


class PrcRadio(object):
    "Objet permettant de communiquer avec la radio BlueTooth du PRC"
    
    def __init__(self, port='COM3'):
        "constructeur de la classe, initialise les champs"
        self.connected = False                  # flag indiquant si on est connecte au PRC ou pas
        self.success = True                     # flag indiquant si la derniere methode appelee a reussi sa tache, utile surtout pour les lectures
        self.serialPort = serial.Serial()       # objet pour le port serie
        self.serialPort.baudrate = 115200       # le debit est de 115200 bits
        self.serialPort.rtscts = True           # on utilise le handshake hardware du port serie
        self.serialPort.timeout = 1             # Les methodes read et write auront un time out de 1s
        self.serialPort.port = port             # Designe le port COM a ouvrir
        
    def connect(self):
        "tente de se connecter au PRC, renvoie le flag de reussite"
        if (self.connected):
            self.success = True                 # si on est deja connecte, on n'a rien a faire et on considere qu'on a reussi
        else:        
            try:
                self.serialPort.open()          # on essaie d'ouvrir le port
                self.connected = True           # en cas de reussite, on met a jour le flag de connexion
                self.success = True             # et celui de reussite
            except:
                self.success = False            # si l'ouverture a echoue, on ne met a jour que le flag de reussite
        return self.success                     # on renvoie le flag de reussite
    
    def disconnect(self):
        "deconnecte le PC du PRC, ne renvoie rien (tjs reussi)"
        self.serialPort.close()                 # on se deconnecte (cette action reussit tjs)
        self.connected = False                  # on met a jour le flag de connexion
        self.success = True                     # et celui de reussite
        
    def getInt(self):
        "Essaye de lire un 'int16' emis par le PRC. Abandonne apres le ""time out"" defini (1s par defaut)"
        try:
            tmp = self.serialPort.read(2)       # lit 2 octets (tmp de type 'Bytes')
            tmp = unpack('h',tmp)               # convertit le 'Bytes' en l'entier correspondant (tmp devient un tuple de int)
            tmp = tmp[0]                        # extrait l'entier du tuple (tmp devient un int)
            self.success = True                 # on met a jour le flag de reussite
        except:
            tmp = 0                             # en cas d'echec, on renvoie la valeur 0
            self.success = False                # on met a jour le flag de reussite
        return tmp

    def getFloat(self):
        "essaye de lire un 'float32' emis par le PRC. Abandonne apres le ""time out"" defini (1s par defaut)"
        try:
            tmp = self.serialPort.read(4)       # lit 4 octets (tmp de type 'Bytes')
            tmp = unpack('f',tmp)               # convertit le 'Bytes' en le flottant correspondant (tmp devient un tuple de float)
            tmp = tmp[0]                        # extrait le flottant du tuple (tmp devient un float)
            self.success = True                 # on met a jour le flag de reussite
        except:
            tmp = 0                             # en cas d'echec, on renvoie la valeur 0
            self.success = False                # on met a jour le flag de reussite
        return tmp

    def getString(self):
        "essaye de lire une chaine de characteres emis par le PRC. Abandonne apres le ""time out"" defini (1s par defaut)"
        try:
            tmp = bytes()
            endOfString=False
            while not endOfString:
                tmp += self.serialPort.read(1)  # lit 1 octets (tmp de type 'Bytes')
                if tmp[-1]==0:                    # jusqu'à la réception du charactère
                    endOfString=True

            self.success = True                 # on met a jour le flag de reussite
        except:
            tmp = 0                             # en cas d'echec, on renvoie la valeur 0
            self.success = False                # on met a jour le flag de reussite
        return tmp
    
    def sendInt(self, data):
        "essaye d'envoyer un 'int16' au PRC. Abandonne apres le ""time out"" defini (1s par defaut)"
        try:
            tmp = int(data)                     # transforme la donnee en entier (au cas ou)
            tmp = pack('h', tmp)                # transforme la donnee en un 'Bytes'
            self.serialPort.write(tmp)          # ecrit la donnee sur le port serie
            self.success = True                 # met a jour le flag de reussite
        except:
            self.success = False                # on met a jour le flag de reussite
        return self.success
    
    def sendFloat(self, data):
        "essaye d'envoyer un 'float' au PRC. Abandonne apres le ""time out"" defini (1s par defaut)"
        try:
            tmp = pack('f', data)               # on transforme la donnee en un 'Bytes'
            self.serialPort.write(tmp)          # on l'ecrit sur le port serie
            self.success = True                 # on met a jour le flag de reussite
        except:
            self.success = False                # on met a jour le flag de reussite
        return self.success

    def isSuccessful(self):
        "renvoie le flag de reussite de la derniere action"
        return self.success
    
    def isConnected(self):
        "renvoie l'etat de la connexion avec le PRC"
        return self.connected

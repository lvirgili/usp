import FAdo
from FAdo.fa import *
from FAdo.fio import *
import sys

automato = FAdo.fio.readFromFile(sys.argv[1])[0]
print automato.reCG()

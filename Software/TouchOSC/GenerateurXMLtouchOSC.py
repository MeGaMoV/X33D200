import sys
  
# Enregistrement du stdout actuel afin que nous puissions rétablir sys.stdout après avoir terminé
# notre redirection
stdout_fileno = sys.stdout

sys.stdout = open('index.xml', 'wb')

LongueurFixture = 50
HauteurFixture = 10
CurrentX = 10
CurrentY = 10
TmpX = 0
TmpY = 0
vertical = False
nbrFixtures = 8

debutXML = '<?xml version="1.0" encoding="UTF-8"?>\n<layout version="7" mode="1" orientation="horizontal">\n<tabpage name="MATRICE" scalef="1.0" scalet="0.0">\n'
finXML = '</tabpage>\n</layout>\n'

sys.stdout.write(bytes(debutXML,'UTF-8'))

cote = '<control type="labelv" color="gray" outline="false" name="/cote/1" x="10" y="0" w="960" h="10" osc_cs="/raw/cote/1"/>\n'
sys.stdout.write(bytes(cote,'UTF-8'))
cote = '<control type="labelv" color="gray" outline="false" name="/cote/2" x="970" y="10" w="10" h="960" osc_cs="/raw/cote/2"/>\n'
sys.stdout.write(bytes(cote,'UTF-8'))
cote = '<control type="labelv" color="gray" outline="false" name="/cote/3" x="10" y="970" w="960" h="10" osc_cs="/raw/cote/3"/>\n'
sys.stdout.write(bytes(cote,'UTF-8'))
cote = '<control type="labelv" color="gray" outline="false" name="/cote/4" x="0" y="10" w="10" h="960" osc_cs="/raw/cote/4"/>\n'
sys.stdout.write(bytes(cote,'UTF-8'))

for y in range(1, nbrFixtures+1):
    for x in range(1, nbrFixtures+1):
        for f in range(1, nbrFixtures+1):
            if f == 1:
                TmpX = CurrentX + HauteurFixture
                TmpY = CurrentY
                vertical = False
                
            if f == 2:
                TmpX = CurrentX + HauteurFixture + LongueurFixture
                TmpY = CurrentY
                vertical = False

            if f == 3:
                TmpX = CurrentX + HauteurFixture + LongueurFixture * 2
                TmpY = CurrentY + HauteurFixture
                vertical = True
                
            if f == 4:
                TmpX = CurrentX + HauteurFixture + LongueurFixture * 2
                TmpY = CurrentY + HauteurFixture + LongueurFixture
                vertical = True

            if f == 5:
                TmpX = CurrentX + HauteurFixture + LongueurFixture
                TmpY = CurrentY + HauteurFixture + LongueurFixture * 2
                vertical = False
                
            if f == 6:
                TmpX = CurrentX + HauteurFixture
                TmpY = CurrentY + LongueurFixture * 2 + HauteurFixture
                vertical = False

            if f == 7:
                TmpX = CurrentX
                TmpY = CurrentY + HauteurFixture + LongueurFixture
                vertical = True
                
            if f == 8:
                TmpX = CurrentX
                TmpY = CurrentY + HauteurFixture
                vertical = True

            if vertical:
                fixture = '<control type="labelv" color="gray" outline="false" name="/fixture/'+str(x)+'/'+str(y)+'/'+str(f)+'" x="'+str(TmpX)+'" y="'+str(TmpY)+'" w="'+str(HauteurFixture)+'" h="'+str(LongueurFixture)+'" osc_cs="/raw/fixture/'+str(x)+'/'+str(y)+'/'+str(f)+'"/>\n'
            else:
                fixture = '<control type="labelv" color="gray" outline="false" name="/fixture/'+str(x)+'/'+str(y)+'/'+str(f)+'" x="'+str(TmpX)+'" y="'+str(TmpY)+'" w="'+str(LongueurFixture)+'" h="'+str(HauteurFixture)+'" osc_cs="/raw/fixture/'+str(x)+'/'+str(y)+'/'+str(f)+'"/>\n'

            sys.stdout.write(bytes(fixture,'UTF-8'))
            
        CurrentX = CurrentX + LongueurFixture * 2 + HauteurFixture * 2
    CurrentX = 10
    CurrentY = CurrentY + LongueurFixture * 2 + HauteurFixture * 2


sys.stdout.write(bytes(finXML,'UTF-8'))

# Fermer le fichier
sys.stdout.close()
# Restaurer sys.stdout
sys.stdout = stdout_fileno

import sys
import base64

# Enregistrement du stdout actuel afin que nous puissions rétablir sys.stdout après avoir terminé
# notre redirection
stdout_fileno = sys.stdout

sys.stdout = open('index.xml', 'wb')

LongueurFixture = 50
HauteurFixture = 10

#Intialisation des variables
CurrentX = 10
CurrentY = 10
TmpX = 0
TmpY = 0
vertical = False
nbrFixtures = 8

debutXML = '<?xml version="1.0" encoding="UTF-8"?>\n<layout version="7" mode="1" orientation="horizontal">\n<tabpage name="MATRICE" scalef="1.0" scalet="0.0">\n'
finXML = '</tabpage>\n</layout>\n'
sys.stdout.write(bytes(debutXML,'UTF-8'))

# Le tour de la Matrice
CurrentX = 20
for x in range(1, nbrFixtures+1):
	tour = '<control type="labelv" color="gray" outline="false" name="/tour/'+str(x)+'/1/1" x="'+str(CurrentX)+'" y="0" w="'+str(LongueurFixture)+'" h="'+str(HauteurFixture)+'" osc_cs="/raw/tour/'+str(x)+'/1/1"/>\n'
	sys.stdout.write(bytes(tour,'UTF-8'))
	tour = '<control type="labelv" color="gray" outline="false" name="/tour/'+str(x)+'/1/2" x="'+str(CurrentX+LongueurFixture)+'" y="0" w="'+str(LongueurFixture)+'" h="'+str(HauteurFixture)+'" osc_cs="/raw/tour/'+str(x)+'/1/2"/>\n'
	sys.stdout.write(bytes(tour,'UTF-8'))
	CurrentX = CurrentX + LongueurFixture * 2 + 20
 
CurrentX = 20
for x in range(1, nbrFixtures+1):
	tour = '<control type="labelv" color="gray" outline="false" name="/tour/'+str(x)+'/8/6" x="'+str(CurrentX)+'" y="970" w="'+str(LongueurFixture)+'" h="'+str(HauteurFixture)+'" osc_cs="/raw/tour/'+str(x)+'/8/6"/>\n'
	sys.stdout.write(bytes(tour,'UTF-8'))
	tour = '<control type="labelv" color="gray" outline="false" name="/tour/'+str(x)+'/8/5" x="'+str(CurrentX+LongueurFixture)+'" y="970" w="'+str(LongueurFixture)+'" h="'+str(HauteurFixture)+'" osc_cs="/raw/tour/'+str(x)+'/8/5"/>\n'
	sys.stdout.write(bytes(tour,'UTF-8'))
	CurrentX = CurrentX + LongueurFixture * 2 + 20

CurrentY = 20
for y in range(1, nbrFixtures+1):
	tour = '<control type="labelv" color="gray" outline="false" name="/tour/1/'+str(y)+'/8" x="0" y="'+str(CurrentY)+'" w="'+str(HauteurFixture)+'" h="'+str(LongueurFixture)+'" osc_cs="/raw/tour/1/'+str(y)+'/8"/>\n'
	sys.stdout.write(bytes(tour,'UTF-8'))
	tour = '<control type="labelv" color="gray" outline="false" name="/tour/1/'+str(y)+'/7" x="0" y="'+str(CurrentY+LongueurFixture)+'" w="'+str(HauteurFixture)+'" h="'+str(LongueurFixture)+'" osc_cs="/raw/tour/1/'+str(y)+'/7"/>\n'
	sys.stdout.write(bytes(tour,'UTF-8'))
	CurrentY = CurrentY + LongueurFixture * 2 + 20
 
CurrentY = 20
for y in range(1, nbrFixtures+1):
	tour = '<control type="labelv" color="gray" outline="false" name="/tour/8/'+str(y)+'/3" x="970" y="'+str(CurrentY)+'" w="'+str(HauteurFixture)+'" h="'+str(LongueurFixture)+'" osc_cs="/raw/tour/8/'+str(y)+'/3"/>\n'
	sys.stdout.write(bytes(tour,'UTF-8'))
	tour = '<control type="labelv" color="gray" outline="false" name="/tour/8/'+str(y)+'/4" x="970" y="'+str(CurrentY+LongueurFixture)+'" w="'+str(HauteurFixture)+'" h="'+str(LongueurFixture)+'" osc_cs="/raw/tour/8/'+str(y)+'/4"/>\n'
	sys.stdout.write(bytes(tour,'UTF-8'))
	CurrentY = CurrentY + LongueurFixture * 2 + 20

# La Matrice
CurrentX = 10
CurrentY = 10

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
        
        
        pos = str(x)+str(y)
        pos_b64 = str(base64.b64encode(pos.encode("utf-8")), "utf-8")
        label = '<control name="label_'+pos+'" x="'+str(TmpX+40)+'" y="'+str(TmpY+30)+'" w="40" h="40" color="gray" type="labelh" text="'+pos_b64+'" size="30" background="false" outline="false" />\n'
        sys.stdout.write(bytes(label,'UTF-8'))    

        CurrentX = CurrentX + LongueurFixture * 2 + HauteurFixture * 2
    CurrentX = 10
    CurrentY = CurrentY + LongueurFixture * 2 + HauteurFixture * 2


sys.stdout.write(bytes(finXML,'UTF-8'))

# Fermer le fichier
sys.stdout.close()
# Restaurer sys.stdout
sys.stdout = stdout_fileno

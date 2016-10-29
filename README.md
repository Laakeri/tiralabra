# tiralabra

Työssä on implementoitu kolme algoritmia mincost-flow-ongelman ratkaisemiseen.
### SAPSPFA
Algoritmi joka laskee mincost-flow:n käyttäen shortest-augmenting-path algoritmia SPFA verkkohakualgoritmilla
### SAPDijkstra
Algoritmi joka laskee mincost-flow:n käyttäen shortest-augmenting-path algoritmia Dijkstran verkkohakualgoritmilla
### ScalingCirculation
Algoritmi joka laskee mincost-circulaation käyttäen kapasiteettiskaalaavaa algoritmia

Ongelmien määritelmät ja algoritmien teoria löytyy [algoritmit.pdf](https://github.com/Laakeri/tiralabra/blob/master/algoritmit.pdf)

## Käyttö
Kääntäminen pitäisi onnistua linuxilla kirjoittamalla make kansiossa src.

Ohjelmaa voi käyttää kirjoittamalla ./main [Algoritmi]. [Algoritmi] kohdalle laitetaan algoritmi jota käytetään esim. ./main SAPDijkstra

Sen jälkeen ohjelma lukee syötteen standardisyötteestä. Ensin syötteessä annetaan kaksi lukua, solmujen määrä ja kaarien määrä. Jos algoritmi on SAPSPFA tai SAPDijkstra annetaan sen jälkeen kolme lukua, lähdesolmu, kohdesolmu ja virtauksen määrä. Solmuilla käytetään ykkösindeksointia, eli solmut on numeroitu 1..n. Jos halutaan maksimivirtauksen, virtauksen määräksi laitetaan joku iso luku. Sen jälkeen annetaan m riviä eli verkon kaaret. Kaaret annetaan muodossa [lähtö kohde kapasiteetti hinta].

Kun syöte on luettu, ohjelma ratkaiseen ongelman ja tulostaa vastauksen. Vastauksessa tulostetaan virtauksen määrä kaarissa samassa järjestyksessä kuin kaaret annettiin syötteessä.
### Esimerkkejä:

	./main SAPSPFA

Syöte:

	4 5
	1 4 1000
	1 2 3 2
	1 3 2 3
	2 3 4 5
	3 4 3 2
	2 4 1 1

Tuloste:

	Flow: 4
	Cost: 22
	2
	0
	2
	0
	1
####
	./main SAPDijkstra

Syöte:

	4 5
	1 4 3
	1 2 3 2
	1 3 2 3
	2 3 4 5
	3 4 3 2
	2 4 1 1

Tuloste:

	Flow: 3
	Cost: 13
	1
	2
	0
	2
	1
####
	./main ScalingCirculation

Syöte:

	10 12
	1 2 3 2 
	1 3 2 3
	2 3 4 5
	2 4 1 1
	3 4 3 2
	4 1 4 -4
	5 6 2 -2
	6 7 2 1
	7 5 3 0
	8 9 5 -10
	9 10 2 15
	10 8 3 -2

Tuloste:

	Cost: -3
	1
	0
	0
	1
	0
	1
	2
	2
	2
	0
	0
	0
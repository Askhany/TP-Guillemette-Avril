# analyse_wireshark3

## 1. Mise en situation

Dans ce TP, l’idée est de regarder comment TCP se comporte « en vrai » à partir d’une capture faite avec Wireshark.  
Je me concentre sur une seule connexion complète, depuis l’ouverture jusqu’à la fermeture, en passant par l’échange de données, sur un flux HTTP en clair vers msftconnecttest.com.

<br>

## 2. Travail réalisé

### 2.1. Partie 1 — Capture d’un trafic TCP significatif

1. Quelle interface réseau a été utilisée pour la capture ?  
   * <span style="color:#1f77b4">Pour la capture, je me suis mis sur l’interface Wi‑Fi de ma machine, celle qui apparaît comme interface active en haut de Wireshark.</span>

2. Quelle application ou quel service a permis de générer le trafic TCP étudié ?  
   * <span style="color:#1f77b4">Le trafic TCP vient d’un accès HTTP lancé automatiquement par Windows vers le site msftconnecttest.com, qui sert au test de connectivité réseau.</span>

3. Quelle est la durée totale de la capture ?  
   * <span style="color:#1f77b4">La durée de la capture correspond au temps qui s’écoule entre le premier et le dernier paquet dans la colonne Time de Wireshark ; pour ce flux précis, ça se joue sur quelques secondes seulement.</span>

4. Combien de paquets ont été capturés au total ?  
   * <span style="color:#1f77b4">Le total des paquets est indiqué dans la barre d’état en bas de Wireshark : j’ai 12088 paquets capturés au total (voir capture 1).</span>

<br>

### 2.2. Partie 2 — Identification d’une session TCP complète

1. Quelle est l’adresse IP du client ?  
   * <span style="color:#1f77b4">L’adresse IP du client est 10.111.16.210, c’est‑à‑dire mon poste connecté en Wi‑Fi sur le réseau local.</span>

2. Quelle est l’adresse IP du serveur ?  
   * <span style="color:#1f77b4">L’adresse IP du serveur est 2.21.244.154, qui correspond à la machine hébergeant le service msftconnecttest.com.</span>

3. Quel est le port source utilisé par le client ?  
   * <span style="color:#1f77b4">Côté client, le port utilisé est un port éphémère, ici 52056, choisi automatiquement par la pile TCP de Windows.</span>

4. Quel est le port de destination utilisé par le serveur ?  
   * <span style="color:#1f77b4">Côté serveur, la destination est le port 80, le port classique pour HTTP en clair.</span>

5. Quel service applicatif semble correspondre à ce port ?  
   * <span style="color:#1f77b4">Avec le port 80 et le décodage Wireshark, on voit clairement qu’il s’agit du service HTTP (Hypertext Transfer Protocol).</span>

<br>

### 2.3. Partie 3 — Analyse de l’établissement de la connexion

1. Quels sont les trois paquets correspondant à l’ouverture de la connexion ?  
   * <span style="color:#1f77b4">L’ouverture se fait avec le three‑way handshake : d’abord un SYN envoyé par le client (10.111.16.210:52056 → 2.21.244.154:80), ensuite un SYN,ACK du serveur, puis un dernier paquet ACK du client qui valide la connexion.</span>

2. Quels drapeaux TCP sont activés dans chacun de ces paquets ?  
   * <span style="color:#1f77b4">On a donc successivement : SYN seul dans le premier paquet, SYN + ACK dans le second, puis ACK seul dans le troisième.</span>

3. Quel est le numéro de séquence initial du client ?  
   * <span style="color:#1f77b4">Le numéro de séquence initial du client apparaît dans le SYN avec une valeur brute, mais Wireshark le montre aussi en version relative comme Sequence number: 0, ce qui est plus simple à suivre.</span>

4. Quel est le numéro d’acquittement retourné par le serveur ?  
   * <span style="color:#1f77b4">Dans le SYN‑ACK, le serveur renvoie un Acknowledgment number égal au numéro de séquence initial du client plus 1, affiché en relatif comme Ack=1.</span>

5. Le handshake observé est-il complet et correct ?  
   * <span style="color:#1f77b4">Oui, le handshake se déroule correctement : on voit bien la suite SYN → SYN,ACK → ACK sans RST ni retransmission au milieu.</span>

<br>

### 2.4. Partie 4 — Étude des numéros de séquence et d’acquittement

1. Comment évoluent les numéros de séquence au cours de l’échange ?  
   * <span style="color:#1f77b4">Les numéros de séquence montent au fur et à mesure que le client envoie des données HTTP ; par exemple, dans la trame 159, il envoie 111 octets avec Seq=1 (relatif), ce qui donne un Next Sequence Number à 112.</span>

2. Comment évoluent les numéros d’acquittement ?  
   * <span style="color:#1f77b4">Les ACK suivent la même logique : quand le serveur confirme ces 111 octets, il renvoie un paquet avec Ack=112, ce qui correspond au dernier octet reçu + 1.</span>

3. La progression des acquittements est-elle cohérente avec les données transmises ?  
   * <span style="color:#1f77b4">Oui, les numéros d’acquittement collent bien avec la taille des segments, on ne voit pas de trous ni de retours en arrière, ce qui montre qu’il n’y a pas de pertes sur ce flux.</span>

4. Relever trois paquets successifs et indiquer, pour chacun, le numéro de séquence et le numéro d’acquittement.  
   * <span style="color:#1f77b4">Sur le flux étudié, on peut par exemple prendre :  
     - Paquet A (client → serveur, trame 159, PSH,ACK, Len=111) : Seq=1, Ack=1.  
     - Paquet B (serveur → client, trame 163, ACK, Len=0) : Seq=1, Ack=112.  
     - Paquet C (serveur → client juste après) : Seq=112, Ack=112.  
     On voit bien la progression logique des compteurs.</span>

<br>

### 2.5. Partie 5 — Analyse des drapeaux TCP

1. Quels drapeaux apparaissent le plus fréquemment dans la session analysée ?  
   * <span style="color:#1f77b4">Le flag qui revient tout le temps, c’est ACK, puisque quasiment tous les segments de données et les accusés de réception le portent.</span>

2. Relever un exemple de paquet contenant le drapeau ACK.  
   * <span style="color:#1f77b4">Un bon exemple est la trame 163, où le serveur envoie un simple ACK vers le client, sans données (Len=0), juste pour confirmer ce qu’il vient de recevoir.</span>

3. Relever un exemple de paquet contenant le drapeau PSH, si présent.  
   * <span style="color:#1f77b4">La trame 159 montre un paquet avec les flags PSH,ACK (0x018) et 111 octets de payload ; c’est là que passe la requête HTTP GET /connecttest.txt HTTP/1.1.</span>

4. Relever un exemple de paquet contenant le drapeau FIN ou RST, si présent.  
   * <span style="color:#1f77b4">En fin de session, on voit un paquet avec les flags FIN,ACK qui lance la fermeture de la connexion, puis un FIN dans l’autre sens et enfin l’ACK final.</span>

5. Quel rôle jouent ces différents drapeaux dans la communication observée ?  
   * <span style="color:#1f77b4">SYN et ACK servent à monter la connexion, PSH demande à pousser les données tout de suite vers l’application, et FIN sert à couper la session proprement ; tout ça permet à TCP de gérer la vie complète de la connexion.</span>

<br>

### 2.6. Partie 6 — Étude de la fenêtre TCP

1. Quelle est la valeur initiale de la fenêtre TCP observée ?  
   * <span style="color:#1f77b4">Dans la trame 159, la fenêtre annoncée vaut 255, avec une Calculated window size de 65280 et un facteur d’échelle de 256.</span>

2. La taille de la fenêtre reste-t-elle constante au cours de la session ?  
   * <span style="color:#1f77b4">Non, la taille de la fenêtre bouge un peu pendant l’échange en fonction du remplissage du tampon de réception ; plusieurs paquets affichent des valeurs différentes.</span>

3. Relever au moins trois valeurs différentes de fenêtre si elles existent.  
   * <span style="color:#1f77b4">On peut par exemple relever : Window=255, puis une valeur autour de 243, puis une autre vers 260 ; ça montre que le récepteur ajuste régulièrement ce qu’il peut encaisser.</span>

4. Quel lien peut-on établir entre la fenêtre TCP et la réception des données ?  
   * <span style="color:#1f77b4">Plus la fenêtre annoncée est grande, plus l’émetteur peut envoyer de données sans attendre de nouveaux ACK, ce qui augmente le débit ; quand la fenêtre se réduit, ça revient à dire « calme un peu le rythme, mon tampon se remplit ».</span>

<br>

### 2.7. Partie 7 — Recherche de retransmissions et d’anomalies

1. Des retransmissions TCP sont-elles présentes dans la capture ?  
   * <span style="color:#1f77b4">Avec le filtre tcp.analysis.retransmission, je ne vois aucune retransmission pour ce flux HTTP, ce qui est bon signe.</span>

2. Si oui, combien ont été détectées ?  
   * <span style="color:#1f77b4">Sur cette session, le compteur reste à 0 : aucun paquet n’est marqué comme retransmis.</span>

3. Des ACK dupliqués sont-ils présents ?  
   * <span style="color:#1f77b4">Le filtre tcp.analysis.duplicate_ack ne remonte rien non plus pour ce flux, donc pas d’ACK dupliqués à signaler.</span>

4. Relever un exemple précis d’anomalie observée dans la session.  
   * <span style="color:#1f77b4">Sur ce cas msftconnecttest, je ne vois pas d’anomalie TCP particulière (pas de RST inattendu, pas de série de retransmissions), la session est propre du début à la fin.</span>

5. Quelle conséquence une telle anomalie peut-elle avoir sur les performances de la communication ?  
   * <span style="color:#1f77b4">En général, dès qu’il y a des pertes (retransmissions, ACK dupliqués), TCP doit renvoyer des segments et réduire son rythme, ce qui rallonge les temps de réponse et fait baisser le débit utile.</span>

<br>

### 2.8. Partie 8 — Analyse graphique du flux TCP

1. Le flux TCP observé semble-t-il régulier ou irrégulier ?  
   * <span style="color:#1f77b4">Sur les graphes de séquence, la courbe monte de façon assez régulière puis se stabilise une fois la réponse reçue, donc le flux est plutôt régulier.</span>

2. Observe-t-on des ruptures, ralentissements ou paliers dans l’évolution du flux ?  
   * <span style="color:#1f77b4">On voit surtout un palier après le téléchargement de connecttest.txt, ce qui est logique, mais pas de cassures brutales qui feraient penser à des pertes ou à une grosse congestion.</span>

3. Le temps aller-retour semble-t-il stable ou variable ?  
   * <span style="color:#1f77b4">Les temps aller‑retour restent globalement stables, les paquets de données et leurs ACK arrivent avec des intervalles réguliers.</span>

4. Quelles conclusions peut-on tirer de l’analyse graphique du flux ?  
   * <span style="color:#1f77b4">Les graphes confirment que l’échange est court, fluide et sans problème de réseau notable : la latence est stable et le débit est suffisant pour ce petit volume de données.</span>

<br>

### 2.9. Partie 9 — Reconstruction du flux applicatif

1. Quel protocole applicatif semble être transporté par cette session TCP ?  
   * <span style="color:#1f77b4">Au‑dessus de TCP, on a bien du HTTP, qu’on voit avec la requête GET /connecttest.txt HTTP/1.1 et l’en‑tête Host: www.msftconnecttest.com.</span>

2. Peut-on identifier une requête applicative ?  
   * <span style="color:#1f77b4">Oui, la trame 159 contient clairement la requête HTTP GET /connecttest.txt avec les en‑têtes User-Agent: Microsoft NCSI et Host: www.msftconnecttest.com.</span>

3. Peut-on identifier une réponse du serveur ?  
   * <span style="color:#1f77b4">La réponse du serveur arrive juste après, avec un code de type HTTP/1.1 200 OK et un petit corps de message utilisé par Windows pour tester la connexion.</span>

4. Le contenu applicatif est-il lisible intégralement, partiellement ou non lisible ?  
   * <span style="color:#1f77b4">Ici le contenu est entièrement lisible, car on est en HTTP sur port 80, donc sans chiffrement : on voit la requête, les en‑têtes et le contenu renvoyé.</span>

5. Quelle conclusion peut-on tirer sur la visibilité des données applicatives dans cette session ?  
   * <span style="color:#1f77b4">Cette capture montre bien que tant qu’on reste en HTTP non chiffré, n’importe qui avec Wireshark peut lire tout le trafic applicatif ; pour cacher ça, il faut passer en HTTPS sur le port 443.</span>

<br>

### 2.10. Partie 10 — Analyse de la fermeture de connexion

1. La fermeture de connexion s’effectue-t-elle avec des drapeaux FIN, RST ou un autre comportement ?  
   * <span style="color:#1f77b4">La fermeture se fait de façon classique, avec des paquets qui portent le flag FIN en plus de ACK, et pas avec des RST brutaux.</span>

2. Combien de paquets participent à la fermeture observée ?  
   * <span style="color:#1f77b4">On retrouve le schéma habituel en quatre paquets : FIN,ACK dans un sens, ACK en retour, puis FIN,ACK dans l’autre sens et dernier ACK pour conclure.</span>

3. Quel hôte initie la fermeture de connexion ?  
   * <span style="color:#1f77b4">L’hôte qui envoie le premier FIN,ACK lance la fermeture ; dans ce cas, c’est en général le serveur HTTP une fois qu’il a fini d’envoyer sa réponse.</span>

4. La fermeture semble-t-elle propre et complète ?  
   * <span style="color:#1f77b4">Oui, la fermeture est propre : tous les FIN et ACK attendus sont là, et il n’y a pas de RST ni de paquet perdu à la fin de la session.</span>

<br>

## 3. Analyse finale

1. Présenter en quelques lignes le scénario général de la communication analysée.  
   * <span style="color:#1f77b4">On est sur un scénario très classique : mon poste 10.111.16.210 ouvre une connexion TCP vers 2.21.244.154:80, envoie une requête GET vers msftconnecttest.com, récupère une petite réponse de test, puis la connexion est refermée proprement.</span>

2. Décrire les principales étapes de la session TCP.  
   * <span style="color:#1f77b4">On retrouve les grandes étapes de TCP : le three‑way handshake pour l’ouverture, l’échange de données HTTP avec des PSH,ACK et les ACK en retour, puis la fermeture en quatre paquets avec les FIN et les derniers accusés de réception.</span>

3. Indiquer si la communication semble saine ou dégradée.  
   * <span style="color:#1f77b4">La communication a l’air saine : pas de retransmissions, pas d’ACK dupliqués, une fenêtre raisonnable et un RTT qui ne bouge pas trop.</span>

4. Justifier cette conclusion à partir d’éléments observables dans Wireshark.  
   * <span style="color:#1f77b4">Je m’appuie sur l’absence de lignes tcp.analysis.retransmission ou duplicate_ack, sur la progression bien régulière des numéros Seq/Ack et sur le fait que handshake et fermeture se passent sans erreur.</span>

<br>

## 4. Filtres Wireshark utiles

- tcp  
- tcp.stream eq 0 (ou le numéro de flux correspondant)  
- tcp.flags.syn == 1  
- tcp.flags.fin == 1  
- tcp.flags.reset == 1  
- tcp.analysis.retransmission  
- tcp.analysis.duplicate_ack  
- tcp.port == 80  
- tcp.port == 443
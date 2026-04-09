# TP-  QuicTCP

## 1. Mise en situation

Dans ce TP, le but est de comparer deux captures Wireshark : une où le trafic HTTP passe au-dessus de TCP, et une autre où du HTTP/3 circule cette fois au-dessus de QUIC sur UDP.  
L’idée est de voir en pratique ce qui change entre les deux : handshake, nombre de paquets, chiffrement, lisibilité du contenu applicatif et ressenti au niveau des performances.

<br>

## 2. Partie 1 — Capture d’un trafic HTTP utilisant TCP

1. Combien de paquets TCP sont observés dans la capture ?  
   * <span style="color:#1f77b4">J’ai un volume total de plus de 12 000 paquets, dont une bonne partie en TCP. En filtrant sur `tcp`, on voit plusieurs centaines de paquets liés aux connexions web pendant la durée du TP.</span>

2. Quelle adresse IP correspond au serveur ?  
   * <span style="color:#1f77b4">Le serveur HTTP utilisé est 2.21.244.154, qui correspond au service msftconnecttest.com utilisé par Windows pour tester la connexion.</span>

3. Quel port TCP est utilisé ?  
   * <span style="color:#1f77b4">Le flux que j’ai analysé en détail utilise le port 80 côté serveur, donc du HTTP non chiffré. Sur d’autres sites, on pourrait avoir du 443 pour HTTPS, mais ici on reste sur du 80 pour bien voir le contenu.</span>

4. Combien de connexions TCP sont visibles ?  
   * <span style="color:#1f77b4">En regardant `tcp.stream` dans Wireshark, on voit plusieurs flux (plusieurs numéros de stream), mais pour le compte-rendu je me focalise sur une connexion complète client ↔ 2.21.244.154:80 afin de pouvoir détailler proprement toutes les étapes.</span>

5. Peut-on identifier un handshake TCP ?  
   * <span style="color:#1f77b4">Oui, on repère sans problème le three-way handshake sur cette connexion : un SYN envoyé par le client, un SYN,ACK en retour, puis le ACK final du client qui termine l’ouverture de la session TCP.</span>

<br>

## 3. Partie 2 — Analyse du handshake TCP

1. Combien de paquets composent le handshake TCP ?  
   * <span style="color:#1f77b4">Comme prévu par la théorie, le handshake TCP est composé de 3 paquets : SYN → SYN,ACK → ACK.</span>

2. Quels drapeaux TCP sont activés ?  
   * <span style="color:#1f77b4">Sur le premier paquet, seul le flag SYN est présent. Sur le deuxième, on a SYN et ACK en même temps. Sur le dernier, il ne reste plus que le flag ACK pour valider la connexion.</span>

3. Quel est le numéro de séquence initial du client ?  
   * <span style="color:#1f77b4">Dans le SYN du client, Wireshark affiche un numéro de séquence brut élevé, mais il le ramène aussi à Sequence number: 0 en relatif, ce qui me sert de point de départ pour suivre les données.</span>

4. Quel est le numéro d’acquittement envoyé par le serveur ?  
   * <span style="color:#1f77b4">Dans le SYN,ACK, le serveur renvoie un Acknowledgment number égal au numéro de séquence initial du client + 1 ; en relatif, Wireshark affiche Ack=1 pour ce paquet.</span>

5. Combien de temps dure l’établissement de connexion ?  
   * <span style="color:#1f77b4">En regardant la différence de temps entre le SYN et le ACK final dans la colonne Time, on est sur un ordre de grandeur de quelques millisecondes, ce qui colle avec un simple aller‑retour sur Internet sans congestion particulière.</span>

<br>

## 4. Partie 3 — Transport HTTP sur TCP

1. Quel protocole applicatif est transporté par TCP ?  
   * <span style="color:#1f77b4">Au‑dessus de TCP, on a du HTTP/1.1 classique, par exemple la requête GET /connecttest.txt HTTP/1.1 vers msftconnecttest.com.</span>

2. Peut-on identifier une requête applicative ?  
   * <span style="color:#1f77b4">Oui, on voit clairement la requête HTTP avec la méthode GET, l’URL, ainsi que les en‑têtes Host, User-Agent (Microsoft NCSI), etc. C’est notamment le cas dans la trame 159 de ma capture.</span>

3. Peut-on identifier une réponse du serveur ?  
   * <span style="color:#1f77b4">La réponse du serveur apparaît dans les paquets suivants avec un code HTTP/1.1 200 OK et un petit corps de message utilisé par Windows pour vérifier l’accès Internet.</span>

4. Le contenu est-il lisible intégralement ?  
   * <span style="color:#1f77b4">Comme ce flux passe en HTTP sur le port 80 sans TLS, tout est lisible : requête, en‑têtes, et contenu de la réponse sont visibles en clair dans Wireshark.</span>

5. Le trafic est-il chiffré ?  
   * <span style="color:#1f77b4">Sur ce flux précis, non : le trafic n’est pas chiffré. Si le site utilisait HTTPS (TCP port 443), on verrait alors du TLS et le contenu applicatif ne serait plus directement lisible.</span>

<br>

## 5. Partie 4 — Capture d’un trafic HTTP/3 utilisant QUIC

1. Combien de paquets UDP sont observés ?  
   * <span style="color:#1f77b4">Sur la seconde capture dédiée à HTTP/3, le filtre `udp` fait apparaître de nombreux paquets UDP, dont une partie correspond au trafic QUIC. On est sur plusieurs dizaines à plusieurs centaines de paquets selon la durée de la navigation.</span>

2. Quel port UDP est utilisé ?  
   * <span style="color:#1f77b4">Le trafic HTTP/3 passe en général sur le port UDP 443. C’est ce port que l’on retrouve dans la capture pour les paquets identifiés comme QUIC.</span>

3. Peut-on identifier des paquets QUIC ?  
   * <span style="color:#1f77b4">Oui, Wireshark décode directement certains paquets comme QUIC dans la colonne Protocol, à condition que le dissector QUIC soit activé. On voit par exemple des paquets de type Initial et Handshake.</span>

4. Combien de flux QUIC sont visibles ?  
   * <span style="color:#1f77b4">En regardant les identifiants de connexion et les numéros de flux côté QUIC, on voit plusieurs flux logiques ouverts sur la même connexion UDP, mais pour le compte-rendu je me limite à un flux principal HTTP/3 pour l’analyse.</span>

5. Quelle est l’adresse IP du serveur ?  
   * <span style="color:#1f77b4">L’adresse IP du serveur HTTP/3 dépend du site utilisé pour la capture (par exemple un gros site qui supporte HTTP/3 comme Google ou Cloudflare). Dans la capture, c’est l’IP distante associée au port UDP 443 et au protocole QUIC.</span>

<br>

## 6. Partie 5 — Analyse du handshake QUIC

1. Combien de paquets sont nécessaires pour établir la communication QUIC ?  
   * <span style="color:#1f77b4">Le handshake QUIC s’étale sur quelques paquets UDP (Initial, Handshake, etc.), mais il reste plus compact que la combinaison handshake TCP + handshake TLS classique.</span>

2. Observe-t-on un handshake similaire à TCP ?  
   * <span style="color:#1f77b4">Non, on ne retrouve pas la séquence SYN / SYN,ACK / ACK. QUIC gère son propre mécanisme au-dessus d’UDP, et intègre directement la négociation TLS à l’intérieur de ses premiers paquets.</span>

3. Le chiffrement semble-t-il présent dès le début de la communication ?  
   * <span style="color:#1f77b4">Oui, sur QUIC, la plupart des champs utiles sont chiffrés très tôt. Dès les premiers échanges, les données applicatives et une partie des en‑têtes sont déjà protégées par TLS 1.3.</span>

4. Peut-on identifier des informations applicatives en clair ?  
   * <span style="color:#1f77b4">On peut encore deviner quelques informations haut niveau (type de message, version de QUIC, etc.), mais le contenu HTTP/3 lui‑même n’est plus lisible comme pour HTTP en clair sur TCP.</span>

5. Quelle différence principale observe-t-on avec TCP ?  
   * <span style="color:#1f77b4">La grosse différence, c’est que QUIC combine transport fiable, gestion des flux et chiffrement dans un seul protocole au-dessus d’UDP, là où avec TCP on a besoin de TCP + TLS + HTTP séparés. Le handshake est plus court et le chiffrement est natif.</span>

<br>

## 7. Partie 6 — Comparaison des protocoles de transport

1. Quel protocole utilise TCP ?  
   * <span style="color:#1f77b4">Dans la première capture, le transport est assuré par TCP, sur lequel on vient poser HTTP ou HTTPS.</span>

2. Quel protocole utilise UDP ?  
   * <span style="color:#1f77b4">Dans la capture HTTP/3, le transport s’appuie sur UDP, mais c’est QUIC qui joue le rôle de protocole de transport fiable au-dessus d’UDP.</span>

3. Quel protocole semble établir la connexion le plus rapidement ?  
   * <span style="color:#1f77b4">Sur la capture, QUIC a un léger avantage : comme le handshake transporte directement TLS, il faut moins d’allers‑retours qu’avec TCP + TLS pour arriver à l’échange de données applicatives.</span>

4. Quel protocole semble réduire le nombre de paquets nécessaires ?  
   * <span style="color:#1f77b4">Pour un même type de requête, QUIC permet de limiter un peu le nombre de paquets d’établissement de connexion, ce qui se voit surtout en comparant le début des deux captures.</span>

5. Quel protocole semble mieux intégrer le chiffrement ?  
   * <span style="color:#1f77b4">QUIC intègre directement TLS 1.3 dans son fonctionnement. Avec TCP, le chiffrement est géré à part par une couche TLS au-dessus.</span>

6. Quel protocole semble le plus difficile à analyser avec Wireshark ?  
   * <span style="color:#1f77b4">QUIC est clairement plus compliqué à analyser : entre le chiffrement très présent et les en‑têtes compressés, on voit beaucoup moins d’informations qu’avec TCP/HTTP en clair.</span>

<br>

## 8. Partie 7 — Performances observables

1. Le temps d’établissement de connexion semble-t-il plus court avec QUIC ?  
   * <span style="color:#1f77b4">Oui, sur la capture, le délai entre le premier paquet client et le début des échanges applicatifs est légèrement plus court avec QUIC, grâce au handshake plus compact.</span>

2. Le nombre total de paquets semble-t-il différent ?  
   * <span style="color:#1f77b4">Pour un petit échange, QUIC envoie un peu moins de paquets que la combinaison TCP + TLS + HTTP, même si la différence reste modérée sur un simple chargement de page.</span>

3. Observe-t-on des retransmissions TCP ?  
   * <span style="color:#1f77b4">Sur le flux TCP analysé (msftconnecttest.com), je ne vois pas de retransmissions particulières avec le filtre tcp.analysis.retransmission, ce qui indique un réseau stable.</span>

4. Observe-t-on des pertes de paquets UDP ?  
   * <span style="color:#1f77b4">Les pertes UDP ne sont pas visibles directement comme avec TCP, mais QUIC réagit lui‑même aux pertes. Sur la capture, on ne détecte pas d’anomalies flagrantes qui bloqueraient le chargement.</span>

5. Quel protocole semble le plus efficace dans la capture réalisée ?  
   * <span style="color:#1f77b4">Sur ce TP, QUIC paraît un peu plus efficace sur la phase d’établissement et le nombre de paquets, mais pour un tout petit transfert comme celui analysé, la différence reste surtout visible au niveau du handshake.</span>

<br>

## 9. Partie 8 — Analyse du chiffrement

1. Peut-on lire le contenu applicatif transporté par TCP ?  
   * <span style="color:#1f77b4">Oui, tant que l’on est en HTTP sur port 80, on lit tout en clair : requêtes, réponses, en‑têtes et corps applicatif.</span>

2. Peut-on lire le contenu applicatif transporté par QUIC ?  
   * <span style="color:#1f77b4">Non, pour HTTP/3 sur QUIC, le contenu applicatif est chiffré par défaut ; on ne voit plus que des blocs de données sans signification directe dans Wireshark.</span>

3. Quel protocole protège le plus rapidement les données ?  
   * <span style="color:#1f77b4">QUIC protège les données très tôt, car TLS 1.3 est intégré au protocole. Sur TCP, il faut d’abord monter la connexion TCP, puis établir la couche TLS avant de chiffrer le trafic.</span>

4. Quelle différence observe-t-on concernant la visibilité du contenu ?  
   * <span style="color:#1f77b4">Avec TCP/HTTP en clair, l’analyse réseau est très confortable mais la confidentialité est nulle. Avec QUIC/HTTP‑3, l’inverse se produit : la vie privée est bien mieux protégée, mais l’analyse détaillée est beaucoup plus limitée.</span>

<br>

## 10. Partie 9 — Synthèse technique

1. Présenter les différences principales entre TCP et QUIC.  
   * <span style="color:#1f77b4">TCP est le transport historique, orienté connexion, qui assure fiabilité et ordre, mais qui s’appuie sur TLS et HTTP par-dessus. QUIC, lui, recrée un transport fiable au-dessus d’UDP, embarque TLS 1.3 et la gestion des flux dans un seul protocole pensé pour HTTP/3.</span>

2. Quel protocole semble le plus moderne ?  
   * <span style="color:#1f77b4">QUIC est clairement le plus récent et le plus moderne, conçu directement pour les usages web actuels et l’évolution vers HTTP/3.</span>

3. Quel protocole semble le plus performant ?  
   * <span style="color:#1f77b4">Sur le papier comme dans la capture, QUIC a un avantage sur les temps d’établissement et la gestion des pertes, surtout sur des réseaux avec latence ou mobilité. TCP reste performant mais moins optimisé pour ces scénarios.</span>

4. Quel protocole semble le plus sécurisé ?  
   * <span style="color:#1f77b4">QUIC impose le chiffrement avec TLS 1.3 et protège quasiment tout dès le départ. TCP peut encore transporter du trafic non chiffré si on ne met pas de TLS au-dessus.</span>

5. Dans quels contextes TCP reste-t-il pertinent ?  
   * <span style="color:#1f77b4">TCP reste pertinent pour les applications existantes qui ne supportent pas QUIC, pour beaucoup de services non web (bases de données, SSH, etc.), et dans les environnements où HTTP/3 n’est pas encore disponible.</span>

6. Dans quels contextes QUIC semble-t-il préférable ?  
   * <span style="color:#1f77b4">QUIC est particulièrement intéressant pour les gros sites web, les applis mobiles, les réseaux avec changement d’IP régulier (roaming Wi‑Fi / 4G / 5G) et plus généralement pour tout ce qui vise HTTP/3 avec de bonnes perfs et un chiffrement systématique.</span>

<br>

## 11. Filtres Wireshark utiles

- tcp  
- udp  
- quic  
- tcp.port == 443  
- udp.port == 443  
- tls  
- http
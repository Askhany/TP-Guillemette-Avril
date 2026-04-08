# TP TCP — Sliding Window
<br>

## 1. Principe général

TCP utilise un mécanisme appelé sliding window afin d'améliorer les performances du transport de données.  
Le récepteur indique au transmetteur la quantité maximale de données qu'il peut accepter sans envoyer d'acquittement supplémentaire. Cette quantité correspond à la taille de la fenêtre TCP.

<br>

## 2. Partie 1 — Observation du sliding window dans Wireshark

Une capture Wireshark a été réalisée lors du téléchargement d'un fichier.

Informations observées :
- taille d'un segment TCP : 1460 octets
- RTT moyen : 40 ms
- taille de la fenêtre TCP : 8760 octets

### Questions
1. Combien de segments TCP peuvent être envoyés sans attendre d'acquittement ?  
   * <span style="color:#1f77b4">8760 ÷ 1460 = 6 segments</span>
2. Combien d'octets peuvent être envoyés sans attendre d'ACK ?  
   * <span style="color:#1f77b4">8760 octets</span>
3. Combien d'acquittements sont nécessaires pour transmettre 43800 octets ?  
   * <span style="color:#1f77b4">43800 ÷ 8760 = 5 acquittements</span>
4. Quelle est la relation entre la taille de la fenêtre et le nombre de segments envoyés ?  
   * <span style="color:#1f77b4">Plus la fenêtre TCP est grande, plus le nombre de segments envoyés avant attente d'un ACK est élevé</span>

<br>

## 3. Partie 2 — Numéros de séquence

Les numéros de séquence observés dans la capture sont les suivants :
- 1000
- 2460
- 3920
- 5380
- 6840

### Questions
1. Quelle est la taille d'un segment TCP dans cet exemple ?  
   * <span style="color:#1f77b4">1460 octets (2460-1000 = 1460)</span>
2. Combien d'octets sont transmis entre le premier et le dernier segment ?  
   * <span style="color:#1f77b4">5840 octets (6840-1000 = 5840)</span>
3. Quel sera le prochain numéro de séquence attendu ?  
   * <span style="color:#1f77b4">8300 (6840 + 1460)</span>
4. Que représente le numéro de séquence dans TCP ?  
   * <span style="color:#1f77b4">Le numéro de séquence représente la position du premier octet de données transporté dans le segment TCP</span>

<br>

## 4. Partie 3 — Calcul du débit théorique

On considère les paramètres suivants :
- taille de fenêtre TCP : 64 KB
- RTT : 50 ms

Formule :  
\[
Débit = \frac{\text{TailleFenetre}}{\text{RTT}}
\]

### Questions
1. Convertir la taille de fenêtre en octets.  
   * <span style="color:#1f77b4">64 KB = 65536 octets (64 × 1024)</span>
2. Convertir le RTT en secondes.  
   * <span style="color:#1f77b4">50 ms = 0,05 s</span>
3. Calculer le débit maximal théorique en octets par seconde.  
   * <span style="color:#1f77b4">1 310 720 octets/s (65536 ÷ 0,05)</span>
4. Convertir le résultat en Ko/s.  
   * <span style="color:#1f77b4">1280 Ko/s (1 310 720 ÷ 1024)</span>
5. Convertir le résultat en Mb/s.  
   * <span style="color:#1f77b4">10,49 Mb/s (1 310 720 × 8 ÷ 1 000 000)</span>

<br>

## 5. Partie 4 — Influence du RTT

On considère une taille de fenêtre fixe de 65535 octets.

### Questions
1. Calculer le débit maximal si le RTT est de 20 ms.  
   * <span style="color:#1f77b4">3 276 750 octets/s (65535 ÷ 0,02)</span>
2. Calculer le débit maximal si le RTT est de 100 ms.  
   * <span style="color:#1f77b4">655 350 octets/s (65535 ÷ 0,1)</span>
3. Calculer le débit maximal si le RTT est de 200 ms.  
   * <span style="color:#1f77b4">327 675 octets/s (65535 ÷ 0,2)</span>
4. Que peut-on conclure concernant l'influence du RTT sur les performances TCP ?  
   * <span style="color:#1f77b4">Plus le RTT augmente, plus le débit diminue proportionnellement</span>

<br>

## 6. Partie 5 — Fenêtre glissante

Une fenêtre TCP permet d'envoyer plusieurs segments avant réception des acquittements.

Données :
- taille de segment : 1000 octets
- taille de fenêtre : 5000 octets

### Questions
1. Combien de segments peuvent être envoyés avant réception d'un ACK ?  
   * <span style="color:#1f77b4">5 segments (5000 ÷ 1000)</span>
2. Quels sont les numéros de séquence envoyés si le premier numéro est 0 ?  
   * <span style="color:#1f77b4">0, 1000, 2000, 3000, 4000</span>
3. Quel numéro d'acquittement sera envoyé après réception complète des données ?  
   * <span style="color:#1f77b4">5000 (dernier octet reçu + 1)</span>
4. Que devient la fenêtre après réception de l'ACK ?  
   * <span style="color:#1f77b4">La fenêtre se décale de 5000 octets et permet d'envoyer 5 nouveaux segments</span>

<br>

## 7. Partie 6 — Analyse d'une capture Wireshark

Extrait de capture :
- Seq = 0
- Seq = 1460
- Seq = 2920
- Seq = 4380
- Ack = 5840
- Window Size = 5840

### Questions
1. Combien de segments ont été envoyés ?  
   * <span style="color:#1f77b4">4 segments</span>
2. Combien d'octets ont été transmis ?  
   * <span style="color:#1f77b4">5840 octets (4380 + 1460)</span>
3. Pourquoi l'ACK indique 5840 ?  
   * <span style="color:#1f77b4">Il confirme la réception de tous les octets jusqu'à la position 5840</span>
4. Quelle est la taille de la fenêtre annoncée par le récepteur ?  
   * <span style="color:#1f77b4">5840 octets</span>
5. Que signifie une diminution de la taille de la fenêtre ?  
   * <span style="color:#1f77b4">Le récepteur a moins d'espace tampon disponible</span>

<br>

## 8. Partie 7 — Débit réel

On observe les éléments suivants dans Wireshark :
- taille de fenêtre : 12000 octets
- RTT : 60 ms

### Questions
1. Calculer le débit maximal théorique.  
   * <span style="color:#1f77b4">200 000 octets/s (12000 ÷ 0,06)</span>
2. Si seulement 8000 octets sont transmis par RTT, quel est le débit réel ?  
   * <span style="color:#1f77b4">133 333 octets/s (8000 ÷ 0,06)</span>
3. Comparer débit réel et débit théorique.  
   * <span style="color:#1f77b4">Le débit réel (133 Ko/s) est inférieur au théorique (200 Ko/s)</span>
4. Proposer une explication possible à la différence observée.  
   * <span style="color:#1f77b4">Pertes de paquets, congestion réseau, ou fenêtre sous-utilisée</span>

<br>

## 9. Partie 8 — Synthèse

### Questions
1. Quel est le rôle principal du sliding window ?  
   * <span style="color:#1f77b4">Permettre l'envoi de plusieurs segments avant d'attendre un ACK</span>
2. Pourquoi TCP n'envoie-t-il pas les données une par une ?  
   * <span style="color:#1f77b4">Pour éviter les temps morts d'attente et maximiser l'utilisation du réseau</span>
3. Quel est l'impact d'une petite fenêtre TCP ?  
   * <span style="color:#1f77b4">Réduit le débit car moins de données peuvent être envoyées en continu</span>
4. Quel est l'impact d'un RTT élevé ?  
   * <span style="color:#1f77b4">Diminue le débit car l'émetteur doit attendre plus longtemps avant de renvoyer</span>
5. Pourquoi le mécanisme de sliding window améliore-t-il les performances ?  
   * <span style="color:#1f77b4">Il remplit le tuyau réseau en continu au lieu d'envoyer segment par segment avec ACK systématique</span>
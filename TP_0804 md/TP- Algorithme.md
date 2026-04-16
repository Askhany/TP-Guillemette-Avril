# TP-Algorithme

## 1. Exercice 1 — Horloges logiques de Lamport

Intérêt de l’algorithme <br>
Les horloges de Lamport permettent d’ordonner les événements dans un système distribué où il n’existe pas d’horloge globale commune. Elles sont essentielles pour analyser la causalité entre événements dans des systèmes distribués, des bases de données distribuées ou des protocoles réseau. 

On considère 3 processus : P1, P2, P3.  

Suite d’événements :  
1. P1 réalise un événement local  
2. P1 envoie un message à P2  
3. P2 reçoit le message  
4. P2 envoie un message à P3  
5. P3 reçoit le message  

Toutes les horloges commencent à 0.  

### Questions  
1. Calculer la valeur de l’horloge logique pour chaque événement en utilisant l’algorithme de Lamport.  
   * <span style="color:#1f77b4">Événement 1 (P1 local) : 1</span>  
   * <span style="color:#1f77b4">Événement 2 (P1 envoie à P2) : 2</span>  
   * <span style="color:#1f77b4">Événement 3 (P2 reçoit) : 3 = max(0,2)+1</span>  
   * <span style="color:#1f77b4">Événement 4 (P2 envoie à P3) : 4</span>  
   * <span style="color:#1f77b4">Événement 5 (P3 reçoit) : 5 = max(0,4)+1</span>  

---

## 2. Exercice 2 — Horloges vectorielles

Intérêt de l’algorithme <br>
Les horloges vectorielles permettent de déterminer précisément la causalité entre événements dans un système distribué. Elles sont utilisées pour savoir si deux événements sont indépendants (concurrents) ou si l’un dépend de l’autre.  

On considère 3 processus : P1, P2, P3.  

Vecteur initial :  
(0, 0, 0)  

Suite d’événements :  
1. P1 réalise un événement local  
2. P1 envoie un message à P2  
3. P2 reçoit le message  
4. P2 réalise un événement local  

### Questions  
1. État du vecteur après l’événement local de P1.  
   * <span style="color:#1f77b4">(1,0,0)</span>  
2. État du vecteur de P1 lors de l’envoi du message à P2.  
   * <span style="color:#1f77b4">(2,0,0)</span>  
3. État du vecteur de P2 après réception du message.  
   * <span style="color:#1f77b4">(2,1,0) = max((0,0,0),(2,0,0)) puis +1 sur P2</span>  
4. État du vecteur de P2 après son événement local.  
   * <span style="color:#1f77b4">(2,2,0)</span>  

---

## 3. Exercice 3 — Algorithme d’élection du leader (Bully)

Intérêt de l’algorithme <br>
Dans un système distribué, il est souvent nécessaire de choisir un coordinateur (leader) pour organiser les tâches, synchroniser les opérations ou prendre des décisions globales. L’algorithme Bully permet d’élire automatiquement un leader en cas de panne.  

On considère 4 processus :  

| Processus | Identifiant |
|----------|-------------|
| P1       | 1           |
| P2       | 2           |
| P3       | 3           |
| P4       | 4           |

Le processus P4 est le leader mais il tombe en panne.  
Le processus P2 détecte la panne.  

### Questions  
1. À quels processus P2 envoie-t-il un message ?  
   * <span style="color:#1f77b4">À P3 et P4 (tous ceux qui ont un identifiant supérieur au sien)</span>  
2. Quel processus devient le nouveau leader ?  
   * <span style="color:#1f77b4">P3, car c’est le plus grand identifiant encore actif</span>  
3. Pourquoi cet algorithme s’appelle-t-il “Bully” ?  
   * <span style="color:#1f77b4">Parce que le processus avec le plus grand identifiant “s’impose” comme leader</span>  

---

## 4. Exercice 4 — Consensus distribué

Intérêt de l’algorithme <br>
Les algorithmes de consensus permettent à plusieurs machines de se mettre d’accord sur une valeur commune malgré les pannes ou les retards réseau. Ils sont essentiels dans les bases de données distribuées, la blockchain et les systèmes tolérants aux pannes.  

Trois serveurs doivent choisir une valeur :  

| Serveur | Valeur proposée |
|---------|-----------------|
| S1      | 10              |
| S2      | 10              |
| S3      | 20              |

La valeur choisie est celle qui obtient la majorité.  

### Questions  
1. Quelle valeur est choisie ?  
   * <span style="color:#1f77b4">10 (2 serveurs sur 3 la proposent)</span>  
2. Que se passe-t-il si S2 tombe en panne ?  
   * <span style="color:#1f77b4">Il reste 10 et 20, il n’y a plus de majorité → pas de consensus possible</span>  
3. Combien de serveurs minimum sont nécessaires pour tolérer une panne ?  
   * <span style="color:#1f77b4">3 serveurs (règle 2f+1 pour f = 1 panne)</span>  

---

## 5. Exercice 5 — Exclusion mutuelle distribuée

Intérêt de l’algorithme <br>
L’exclusion mutuelle garantit qu’une seule machine à la fois accède à une ressource partagée (fichier, base de données, imprimante). Cela évite les incohérences et les conflits dans les systèmes distribués.  

Trois processus souhaitent accéder à une ressource critique.  

| Processus | Timestamp |
|-----------|-----------|
| P1        | 5         |
| P2        | 2         |
| P3        | 8         |

### Questions  
1. Déterminer l’ordre d’accès à la ressource.  
   * <span style="color:#1f77b4">P2 → P1 → P3 (ordre croissant des timestamps : 2, 5, 8)</span>  

---

## 6. Exercice 6 — Réplication et quorum

Intérêt de l’algorithme <br>
La réplication permet de stocker des données sur plusieurs serveurs afin d’améliorer la disponibilité et la tolérance aux pannes. Les quorums permettent de maintenir la cohérence des données lors des opérations de lecture et d’écriture.  

Une donnée est stockée sur 5 serveurs.  

Règles :  
— lecture : nécessite 2 réponses  
— écriture : nécessite 3 confirmations  

### Questions  
1. Peut-on lire si 3 serveurs sont en panne ?  
   * <span style="color:#1f77b4">Oui, il reste 2 serveurs, ce qui suffit pour la lecture</span>  
2. Peut-on écrire si 2 serveurs sont en panne ?  
   * <span style="color:#1f77b4">Oui, il reste 3 serveurs, ce qui suffit pour l’écriture</span>  
3. Pourquoi l’écriture nécessite-t-elle plus de confirmations que la lecture ?  
   * <span style="color:#1f77b4">Pour garantir que les écritures touchent assez de serveurs et restent cohérentes avec les lectures</span>  

---

## 7. Exercice 7 — Détection de panne par heartbeat

Intérêt de l’algorithme <br>
La détection de panne permet d’identifier les machines défaillantes dans un système distribué afin de garantir la continuité du service. Les messages heartbeat sont souvent utilisés pour vérifier qu’un serveur fonctionne toujours.  

Un serveur envoie un message “alive” toutes les 2 secondes.  

Un autre serveur observe les messages suivants :  

| Temps | Message |
|-------|---------|
| 0 s   | alive   |
| 2 s   | alive   |
| 4 s   | alive   |
| 6 s   | rien    |
| 8 s   | rien    |

### Questions  
1. Après combien de temps peut-on suspecter une panne du serveur ?  
   * <span style="color:#1f77b4">À partir de 6 secondes, dès le premier message manquant</span>
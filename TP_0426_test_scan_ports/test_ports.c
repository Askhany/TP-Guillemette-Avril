#include <stdio.h>      // Pour afficher du texte à l'écran avec printf() et gérer les erreurs avec perror()
#include <stdlib.h>     // Pour quitter le programme avec exit() et convertir des chaînes en nombres avec atoi()
#include <string.h>     // Pour manipuler les chaînes de caractères et mettre à zéro des zones mémoire avec memset()
#include <winsock2.h>   // Pour utiliser les sockets réseau sur Windows (TCP/UDP)
#include <ws2tcpip.h>   // Pour des fonctions réseau supplémentaires (comme inet_pton pour convertir IP)

#pragma comment(lib, "ws2_32.lib")  // Indique au compilateur d'inclure la bibliothèque Winsock lors de la compilation

#define TIMEOUT_SEC 0.5   // Durée maximale (en secondes) d'attente pour une connexion à un port

// ==========================================
// Programme : Scanner de ports TCP sur Windows
// Objectif : Tester si les ports 1 à 1024 d'une machine sont ouverts ou fermés
// Cette version : Affiche chaque port, compte les ports ouverts, utilise un timeout rapide
// ==========================================

int main(int argc, char *argv[]) {

    // ==========================================
    // 1. Vérifier si l'utilisateur a fourni une IP
    // ==========================================
    // argc = nombre d'arguments passés au programme (ex: ./scan_ports.exe 127.0.0.1)
    // argv = tableau contenant les arguments
    // argv[0] = nom du programme
    // argv[1] = IP cible
    if (argc != 2) {
        printf("Usage: %s <IP cible>\n", argv[0]);
        printf("Exemple: %s 127.0.0.1\n", argv[0]);
        return 1; // Quitte le programme si pas d'IP donnée
    }

    char *target_ip = argv[1]; // Stocke l'IP cible dans une variable pour faciliter l'utilisation

    // ==========================================
    // 2. Initialiser Winsock (obligatoire sur Windows)
    // ==========================================
    WSADATA wsa; // Structure qui stocke les informations de l'API Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        // Si WSAStartup échoue, on ne peut pas utiliser le réseau
        printf("Erreur : impossible d'initialiser Winsock\n");
        return 1;
    }
    // À ce stade, Winsock est prêt à être utilisé pour créer des sockets

    printf("Scan des ports TCP sur %s...\n", target_ip);

    int open_ports = 0; // Compteur pour savoir combien de ports sont ouverts

    // ==========================================
    // 3. Boucle pour tester chaque port
    // ==========================================
    // On teste les ports standards de 1 à 1024 (HTTP, SSH, FTP, SMTP, etc.)
    for (int port = 1; port <= 1024; port++) {

        // ==========================================
        // 4. Créer une socket TCP
        // ==========================================
        // Une socket = point de communication pour envoyer/recevoir des données
        // AF_INET = on utilise IPv4 (ex: 192.168.1.1)
        // SOCK_STREAM = connexion TCP (fiable, orientée connexion)
        // IPPROTO_TCP = protocole TCP
        SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (sock == INVALID_SOCKET) { 
            printf("Erreur de création de socket sur le port %d\n", port);
            continue; // Passe au port suivant si création de socket échoue
        }

        // ==========================================
        // 5. Préparer l'adresse de la machine cible
        // ==========================================
        struct sockaddr_in server_addr; // Structure qui contient l'adresse IP et le port
        server_addr.sin_family = AF_INET; // On précise IPv4
        server_addr.sin_port = htons(port); // Conversion du numéro de port en format réseau
        // htons = Host TO Network Short -> convertit le port du format machine au format réseau

        // inet_pton = Convertit une IP en texte (ex: "127.0.0.1") en format binaire utilisable
        if (inet_pton(AF_INET, target_ip, &server_addr.sin_addr) <= 0) {
            printf("IP invalide : %s\n", target_ip);
            closesocket(sock); // On ferme la socket pour libérer les ressources
            continue; // Passe au port suivant
        }

        memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));
        // On met à zéro le reste de la structure pour éviter des valeurs indéfinies (bonne pratique)

        // ==========================================
        // 6. Définir un délai maximum (timeout)
        // ==========================================
        // Timeout = combien de temps attendre avant de considérer un port comme fermé
        DWORD timeout = (DWORD)(TIMEOUT_SEC * 1000); // Conversion en millisecondes (0.5 sec = 500 ms)
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)); // Timeout réception
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)); // Timeout envoi

        // ==========================================
        // 7. Essayer de se connecter au port
        // ==========================================
        // connect() tente d'ouvrir une connexion TCP avec le port cible
        // Si connect() retourne 0, la connexion a réussi -> port ouvert
        // Sinon -> port fermé ou filtré par un firewall
        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
            printf("Port %d ouvert\n", port); // Affiche que le port est ouvert
            open_ports++; // On incrémente le compteur
        } else {
            printf("Port %d ferme\n", port); // Affiche que le port est fermé
        }

        // ==========================================
        // 8. Fermer la socket après chaque test
        // ==========================================
        closesocket(sock); // Libère les ressources, très important
    }

    // ==========================================
    // 9. Nettoyer Winsock à la fin du programme
    // ==========================================
    WSACleanup(); // Toujours nettoyer Winsock pour libérer la mémoire utilisée

    // ==========================================
    // 10. Afficher un résumé du scan
    // ==========================================
    printf("\nScan terminé.\n");
    printf("Nombre de ports ouverts : %d sur 1024\n", open_ports);
    printf("Tous les ports non listés sont fermés ou filtrés.\n");

    return 0; // Fin du programme
}
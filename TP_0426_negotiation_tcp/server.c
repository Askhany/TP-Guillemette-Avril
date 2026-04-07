#include <stdio.h>      // Pour printf() et perror() -> afficher du texte et messages d'erreur
#include <string.h>     // Pour memset() -> mettre des zones mémoire à zéro
#include <winsock2.h>   // Pour les sockets sur Windows
#include <ws2tcpip.h>   // Pour des fonctions réseau supplémentaires (ex: inet_pton)

#pragma comment(lib, "ws2_32.lib") // Indique au compilateur de lier la librairie Winsock

#define PORT 12345 // Port sur lequel le serveur va écouter

int main() {

    // ======================================
    // 1. Initialisation de Winsock
    // ======================================
    // Avant d'utiliser les sockets sur Windows, il faut toujours initialiser l'API réseau Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) { // MAKEWORD(2,2) = version 2.2 de Winsock
        printf("Erreur d'initialisation de Winsock\n");
        return 1; // On quitte si impossible d'initialiser
    }

    // ======================================
    // 2. Création de la socket serveur
    // ======================================
    // SOCK_STREAM = TCP (connexion fiable)
    // AF_INET = IPv4
    // IPPROTO_TCP = protocole TCP
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == INVALID_SOCKET) { // Vérifie si la socket a été créée correctement
        printf("Erreur de création de socket\n");
        WSACleanup();
        return 1;
    }

    // ======================================
    // 3. Préparer l'adresse du serveur
    // ======================================
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(PORT); // Convertit le numéro de port pour le réseau
    server_addr.sin_addr.s_addr = INADDR_ANY; // Écoute sur toutes les interfaces réseau
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero)); // Remplit le reste de la structure avec des zéros

    // ======================================
    // 4. Lier la socket à l'adresse
    // ======================================
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Erreur de bind\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ======================================
    // 5. Mettre la socket en mode écoute
    // ======================================
    // Le serveur va attendre des connexions entrantes
    if (listen(sock, 1) < 0) { // 1 = nombre max de clients en file d'attente
        printf("Erreur de listen\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Serveur prêt sur le port %d\n", PORT);

    // ======================================
    // 6. Attente d'un client
    // ======================================
    SOCKET client_sock;
    struct sockaddr_in client_addr;
    int addr_len = sizeof(client_addr);

    client_sock = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
    // accept() bloque jusqu'à ce qu'un client se connecte
    if (client_sock == INVALID_SOCKET) {
        printf("Erreur d'accept\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Client connecté !\n");

    // ======================================
    // 7. Simulation handshake TCP
    // ======================================
    // Le vrai handshake TCP est SYN → SYN-ACK → ACK
    // Ici on simule juste les messages pour l'apprentissage
    char buffer[1024];

    // Attendre SYN du client
    recv(client_sock, buffer, sizeof(buffer), 0);
    printf("Serveur: reçu SYN du client\n");

    // Envoyer SYN-ACK
    send(client_sock, "SYN-ACK", 8, 0);
    printf("Serveur: envoyé SYN-ACK\n");

    // Attendre ACK
    recv(client_sock, buffer, sizeof(buffer), 0);
    printf("Serveur: reçu ACK, connexion établie !\n");

    // ======================================
    // 8. Fermeture des sockets
    // ======================================
    closesocket(client_sock);
    closesocket(sock);

    // ======================================
    // 9. Nettoyage Winsock
    // ======================================
    WSACleanup();

    printf("Serveur terminé.\n");
    return 0;
}
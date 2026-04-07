#include <stdio.h> 
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Lien avec Winsock

#define PORT 12345 // Port sur lequel le serveur écoute

int main() {

    // ======================================
    // 1. Initialisation de Winsock
    // ======================================
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Erreur d'initialisation de Winsock\n");
        return 1;
    }

    // ======================================
    // 2. Création de la socket client
    // ======================================
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("Erreur de création de socket\n");
        WSACleanup();
        return 1;
    }

    // ======================================
    // 3. Préparer l'adresse du serveur
    // ======================================
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // Convertir le port pour le réseau
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); // IP locale
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    // ======================================
    // 4. Connexion au serveur
    // ======================================
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Erreur de connexion\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Connecté au serveur !\n");

    // ======================================
    // 5. Simulation handshake TCP
    // ======================================
    char buffer[1024];

    // Envoyer SYN
    send(sock, "SYN", 4, 0);
    printf("Client: envoyé SYN\n");

    // Attendre SYN-ACK
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Client: reçu %s\n", buffer);

    // Envoyer ACK
    send(sock, "ACK", 4, 0);
    printf("Client: envoyé ACK, connexion établie !\n");

    // ======================================
    // 6. Fermeture de la socket et nettoyage
    // ======================================
    closesocket(sock);
    WSACleanup();

    printf("Client terminé.\n");
    return 0;
}
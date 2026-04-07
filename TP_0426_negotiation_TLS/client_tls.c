#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Lier Winsock automatiquement

#define SERVER_IP "127.0.0.1" // IP du serveur local
#define PORT 4433             // Port du serveur TLS simulé
#define BUFFER_SIZE 1024      // Taille du buffer pour envoyer/recevoir

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // ==========================================
    // Initialiser Winsock
    // ==========================================
    printf("Initialisation de Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Erreur d'initialisation Winsock\n");
        return 1;
    }

    // ==========================================
    // Créer la socket TCP pour le client
    // ==========================================
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Erreur création socket\n");
        WSACleanup();
        return 1;
    }

    // ==========================================
    // Configurer l'adresse du serveur
    // ==========================================
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr); // Convertir l'IP en binaire

    // ==========================================
    // Connexion au serveur
    // ==========================================
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
        printf("Impossible de se connecter au serveur\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Connecté au serveur !\n");

    // ==========================================
    // 1. Envoyer ClientHello
    // ==========================================
    strcpy(buffer, "ClientHello: TLS 1.2, AES256, random5678"); // Simulé
    send(sock, buffer, strlen(buffer), 0);

    // ==========================================
    // 2. Recevoir ServerHello + certificat
    // ==========================================
    memset(buffer, 0, BUFFER_SIZE);
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Reçu du serveur : %s\n", buffer); // ServerHello

    memset(buffer, 0, BUFFER_SIZE);
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Reçu du serveur : %s\n", buffer); // Certificat

    // ==========================================
    // 3. Envoyer ClientKeyExchange (clé pré-master)
    // ==========================================
    strcpy(buffer, "ClientKeyExchange: clé pré-master simulée");
    send(sock, buffer, strlen(buffer), 0);

    // ==========================================
    // 4. Passage en mode chiffré
    // ==========================================
    memset(buffer, 0, BUFFER_SIZE);
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("%s\n", buffer); // ChangeCipherSpec

    // ==========================================
    // 5. Handshake terminé
    // ==========================================
    memset(buffer, 0, BUFFER_SIZE);
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("%s\n", buffer); // Finished

    // ==========================================
    // 6. Échange de messages simulé
    // ==========================================
    while(1) {
        printf("Entrez un message (ou 'exit' pour quitter) : ");
        fgets(buffer, BUFFER_SIZE, stdin);                // Lire l'entrée utilisateur
        buffer[strcspn(buffer, "\n")] = 0;               // Supprimer le retour chariot

        if (strcmp(buffer, "exit") == 0) break;          // Quitter si l'utilisateur tape exit

        send(sock, buffer, strlen(buffer), 0);           // Envoyer le message au serveur

        memset(buffer, 0, BUFFER_SIZE);
        recv(sock, buffer, BUFFER_SIZE, 0);             // Recevoir la réponse du serveur
        printf("Réponse serveur : %s\n", buffer);
    }

    printf("Déconnexion du serveur...\n");
    closesocket(sock); // Fermer la socket client
    WSACleanup();      // Nettoyage final de Winsock

    return 0;
}
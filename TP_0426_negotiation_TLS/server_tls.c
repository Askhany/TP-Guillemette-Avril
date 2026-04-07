#include <stdio.h>      // Pour printf() et perror() -> afficher des messages
#include <stdlib.h>     // Pour exit() et fonctions utilitaires
#include <string.h>     // Pour memset() -> mettre une zone mémoire à zéro
#include <winsock2.h>   // Pour utiliser les sockets sur Windows
#include <ws2tcpip.h>   // Pour inet_pton et fonctions réseau avancées

#pragma comment(lib, "ws2_32.lib") // Permet de lier automatiquement la librairie Winsock

#define PORT 4433          // Port simulé pour TLS (HTTPS par défaut c'est 443)
#define BUFFER_SIZE 1024   // Taille du buffer pour les messages

int main() {
    WSADATA wsa;               // Structure pour initialiser Winsock
    SOCKET server_socket;      // Socket serveur
    SOCKET client_socket;      // Socket pour le client connecté
    struct sockaddr_in server_addr, client_addr; // Structures pour stocker les adresses réseau
    int client_len = sizeof(client_addr);       // Taille de la structure client
    char buffer[BUFFER_SIZE];  // Buffer pour envoyer et recevoir des données

    // ==========================================
    // Initialiser Winsock
    // ==========================================
    printf("Initialisation de Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) { // Version 2.2 de Winsock
        printf("Erreur d'initialisation de Winsock\n");
        return 1; // Quitte si erreur
    }

    // ==========================================
    // Créer la socket TCP du serveur
    // ==========================================
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Erreur création socket\n");
        WSACleanup(); // Nettoyage de Winsock
        return 1;
    }

    // ==========================================
    // Configurer l'adresse du serveur
    // ==========================================
    server_addr.sin_family = AF_INET;         // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accepte toutes les interfaces réseau locales
    server_addr.sin_port = htons(PORT);       // Convertit le port en format réseau (big-endian)

    // ==========================================
    // Lier la socket à l'adresse et au port
    // ==========================================
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Erreur lors du bind\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // ==========================================
    // Mettre le serveur en écoute
    // ==========================================
    listen(server_socket, 1); // 1 = taille de la file d'attente
    printf("Serveur TLS simulé en écoute sur le port %d...\n", PORT);

    // ==========================================
    // Attendre et accepter la connexion d'un client
    // ==========================================
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Erreur lors de l'acceptation de connexion\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("Client connecté !\n");

    // ==========================================
    // 1. Recevoir ClientHello
    // ==========================================
    memset(buffer, 0, BUFFER_SIZE);           // Mettre le buffer à zéro
    recv(client_socket, buffer, BUFFER_SIZE, 0); // Lire les données envoyées par le client
    printf("Reçu du client : %s\n", buffer); // Affiche le ClientHello simulé

    // ==========================================
    // 2. Envoyer ServerHello + certificat
    // ==========================================
    strcpy(buffer, "ServerHello: TLS 1.2, AES256, random1234"); // Message simulé
    send(client_socket, buffer, strlen(buffer), 0);            // Envoyer au client

    strcpy(buffer, "Certificate: CertificatServeurSimulé");    // Certificat simulé
    send(client_socket, buffer, strlen(buffer), 0);

    // ==========================================
    // 3. Recevoir ClientKeyExchange (clé pré-master)
    // ==========================================
    memset(buffer, 0, BUFFER_SIZE);           // Réinitialiser le buffer
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("Clé pré-master reçue : %s\n", buffer); // Affiche la clé pré-master

    // ==========================================
    // 4. Passage en mode chiffré
    // ==========================================
    strcpy(buffer, "ChangeCipherSpec: passage en mode chiffré"); // Simulation
    send(client_socket, buffer, strlen(buffer), 0);

    // ==========================================
    // 5. Handshake terminé
    // ==========================================
    strcpy(buffer, "Finished: handshake TLS terminé côté serveur"); // Indique que TLS est prêt
    send(client_socket, buffer, strlen(buffer), 0);

    // ==========================================
    // 6. Échange de données simulé
    // ==========================================
    while(1) {
        memset(buffer, 0, BUFFER_SIZE);               // Nettoyer le buffer
        int bytes = recv(client_socket, buffer, BUFFER_SIZE, 0); // Lire le message
        if (bytes <= 0) break;                        // Si rien reçu => client déconnecté

        printf("Message chiffré reçu : %s\n", buffer); // Affiche le message simulé

        // Réponse du serveur
        strcpy(buffer, "Réponse serveur chiffrée");
        send(client_socket, buffer, strlen(buffer), 0);
    }

    printf("Client déconnecté, fermeture du serveur...\n");

    closesocket(client_socket); // Fermer la socket client
    closesocket(server_socket); // Fermer la socket serveur
    WSACleanup();               // Nettoyage final de Winsock

    return 0;
}
#include <iostream>
#include <cstdlib>   //pour atoi, exit
#include <cstring>   //pour memset, strlen
#include <unistd.h>  //pour read, write, close
#include <arpa/inet.h> //pour htons, INADDR_ANY, sockaddr_in

#define TAILLE_TAMPON 1024  //la taille maximale des messages échangés

int main(int argc, char *argv[]) {
    if (argc != 2) { //verifie que le programme est appelé avec exactement un argument supplémentaire
        std::cerr << "Usage : " << argv[0] << " <port>\n";
        exit(EXIT_FAILURE);
    }

    int port = std::atoi(argv[1]); //convertir l'argument de la ligne de commande en une chaine de caractere
    int socketServeur, socketsClient;
    char tompon[TAILLE_TAMPON];

    
    socketServeur = socket(PF_INET, SOCK_STREAM, 0);  //crée un socket TCP
    if (socketServeur == -1) {  //si la création échoue, cad = -1, le programme affiche un message d'erreur 
        perror("[Serveur] Erreur : impossible de créer le socket.\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in adresseServeur, adresseClient; //structure représentant une adresse ipv4
    socklen_t taille_client = sizeof(adresseClient);
    std::memset(&adresseServeur, 0, sizeof(adresseServeur)); //initialise la structure adresseServeur avec des zéros
    adresseServeur.sin_family = PF_INET; //definit la famille d'adresses
    adresseServeur.sin_addr.s_addr = INADDR_ANY; //le serveur accepte la co depuis n'importe interface reseau locale
    adresseServeur.sin_port = htons(port); //convertit le num port en format reseau 
    
    if (bind(socketServeur, (struct sockaddr *)&adresseServeur, sizeof(adresseServeur)) == -1) { //si la liaison échoue, affiche une erreur
        perror("[Serveur] Erreur : impossible de lier le socket à l'adresse.\n");
        close(socketServeur);
        exit(EXIT_FAILURE);
    }

    //mettre le socket en mode ecoute pour accepter des connexion
    if (listen(socketServeur, 5) == -1) { //5 est taille de la file d'attente
        perror("[Serveur] Erreur : impossible de mettre le socket en écoute.\n"); //si echoue, affiche message d'erreur
        close(socketServeur);
        exit(EXIT_FAILURE);
    }

    std::cout << "[Serveur] En attente de connexions sur le port " << port << "...\n";

    while (true) {
        //accepte une connexion entrante
        socketsClient = accept(socketServeur, (struct sockaddr *)&adresseClient, &taille_client);
        if (socketsClient == -1) {
            perror("[Serveur] Erreur lors de l'acceptation d'une connexion.\n");
            continue;
        }

        std::cout << "[Serveur] Client connecté.\n";

        //lire les données envoyées par le client
        std::memset(tompon, 0, TAILLE_TAMPON);
        ssize_t recu = read(socketsClient, tompon, TAILLE_TAMPON - 1);
        if (recu > 0) {  //si un message est reçu, il est afficher
            std::cout << "[Serveur] Message reçu : " << tompon << "\n";

            //répondre au client
            const char *message = "Message bien reçu.";
            write(socketsClient, message, std::strlen(message)); //envoie une réponse
        }

        close(socketsClient); //fermer le socket client
        std::cout << "[Serveur] Connexion avec le client terminée.\n";
    }

    close(socketServeur); //fermer le socket serveur
    return 0;
}

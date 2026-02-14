#include <iostream>
#include <cstring>      //pour memset
#include <cstdlib>      //pour atoi
#include <unistd.h>     //pour close
#include <arpa/inet.h>  //pour les fonctions de socket

#define MAX_CHAINE 20  //taille maximale des messages que le serveur pourra gérer

int main(int argc, char *argv[]) { 

    if (argc != 2) { //le programme vérifie que l'utilisateur a passé exactement un argument
        std::cerr << "Usage : " << argv[0] << " <port>" << std::endl;
        return EXIT_FAILURE;
    }

    int port = std::atoi(argv[1]); //port converti en entier

    //crée un socket pour le protocole UDP
    int socket_serveur = socket(PF_INET, SOCK_DGRAM, 0);
    if (socket_serveur == -1) {
        std::cerr << "[Serveur] Erreur : impossible de créer le socket." << std::endl; //si la création échoue il affiche un message d’erreur
        return EXIT_FAILURE; 
    }

    //configuration de l'adresse du serveur
    sockaddr_in adresse_serveur;
    std::memset(&adresse_serveur, 0, sizeof(adresse_serveur));
    adresse_serveur.sin_family = PF_INET;
    adresse_serveur.sin_addr.s_addr = INADDR_ANY;
    adresse_serveur.sin_port = htons(port);

    //liaison du socket à l'adresse locale
    if (bind(socket_serveur, reinterpret_cast<sockaddr*>(&adresse_serveur), sizeof(adresse_serveur)) == -1) {
        std::cerr << "[Serveur] Erreur : impossible de lier le socket à l'adresse." << std::endl;
        close(socket_serveur);
        return EXIT_FAILURE;
    }

    std::cout << "[Serveur] En attente de messages sur le port " << port << "..." << std::endl;

    //configuration pour le client
    sockaddr_in adresse_client;
    socklen_t taille_client = sizeof(adresse_client);

    char chaine_recue[MAX_CHAINE + 1]; //tampon pour la chaîne reçue (espace pour '\0')

    while (true) {
        std::memset(chaine_recue, 0, sizeof(chaine_recue)); //nettoyage du tampon

        // Réception d'un message du client
        ssize_t recu = recvfrom(socket_serveur, chaine_recue, MAX_CHAINE, 0,
                                reinterpret_cast<sockaddr*>(&adresse_client), &taille_client);

        if (recu < 0) {
            std::cerr << "[Serveur] Erreur lors de la réception." << std::endl;
            continue;
        }

        chaine_recue[recu] = '\0'; //ajout du caractère de fin de chaîne
        std::cout << "[Serveur] Message reçu : " << chaine_recue << std::endl;
    }

    close(socket_serveur);
    return 0;
}
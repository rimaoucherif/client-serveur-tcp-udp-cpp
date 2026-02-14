#include <stdexcept>
#include <iostream>
#include <cstring>    //pour memset, strlen
#include <cstdlib>    //pour atoi, exit
#include <unistd.h>   //pour read, write, close
#include <arpa/inet.h> //pour htons, inet_pton

using namespace std;


#define TAILLE_TAMPON 1024  //taille maximum pour les messages échangés

int main(int argc, char *argv[]) {
    if (argc != 3) { 
        cerr << "Usage : " << argv[0] << " <adresse_IP> <port>\n";
        return EXIT_FAILURE;
    }

    const char *adresseIP = argv[1]; //adresse IP
    int port = atoi(argv[2]); //convertit le numéro de port en entier 

    //création du socket TCP
    int socketClient = socket(PF_INET, SOCK_STREAM, 0);
    if (socketClient == -1) {
        perror("[Client] Erreur : impossible de créer le socket.\n");
        return EXIT_FAILURE;
    }

    //configuration de l'adresse du serveur
    struct sockaddr_in adresseServeur; //struct contenant les informations de l'adresse serveur
    memset(&adresseServeur, 0, sizeof(adresseServeur)); //initialise tous les champs de la struct à zéro.
    adresseServeur.sin_family = PF_INET;
    adresseServeur.sin_port = htons(port);

    if (inet_pton(PF_INET, adresseIP, &adresseServeur.sin_addr) <= 0) {   //convertit l'adresse IP texte  en format binaire
        perror("[Client] Erreur : adresse IP invalide.\n");
        close(socketClient);
        return EXIT_FAILURE;
    }

    if (connect(socketClient, reinterpret_cast<struct sockaddr*>(&adresseServeur), sizeof(adresseServeur)) == -1) {  //connexion au serveur
        perror("[Client] Erreur : impossible de se connecter au serveur.\n");
        close(socketClient);
        return EXIT_FAILURE;
    }

    cout << "[Client] Connecté au serveur " << adresseIP << ":" << port << "\n"; //affiche un message confirmant que la connexion au serveur a été faite

    char message[TAILLE_TAMPON];
    cout << "[Client] Entrez un message : ";
    cin.getline(message, TAILLE_TAMPON); //demande à l'utilisateur d'entrer un message à envoyer au serveur

    if (write(socketClient, message, strlen(message)) == -1) { //envoie le message au serveur via le socket
        perror("[Client] Erreur : échec de l'envoi du message.\n"); //si erreur, affiche un message d'erreur
        close(socketClient);
        return EXIT_FAILURE;
    }

    //lecture de la réponse du serveur
    char answer[TAILLE_TAMPON];
    memset(answer, 0, TAILLE_TAMPON); //initialise le tampon de answer à zero
    if (read(socketClient, answer, TAILLE_TAMPON - 1) == -1) {  //lit la answer envoyée par le serveur
        perror("[Client] Erreur : échec de la lecture de la réponse.\n");
        close(socketClient);
        return EXIT_FAILURE;
    }

    cout << "[Client] Réponse du serveur : " << answer << "\n"; //affiche la answer reçue du serveur

    close(socketClient); //fermer la co au serveur
    return 0;
}
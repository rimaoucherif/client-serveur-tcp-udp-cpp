#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>      //pour std::atoi
#include <unistd.h>     //pour close
#include <arpa/inet.h>  //pour les fonctions de socket

constexpr int MAX_CHAINE = 20; //taille maximale d'une chaîne de caractères

int main(int argc, char* argv[]) {
    //vérification des arguments
    if (argc != 3) {
        std::cerr << "Usage : " << argv[0] << " <adresse_IP> <port>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string adresse_ip = argv[1];
    int port = std::atoi(argv[2]); // Conversion du port en entier

    //création du socket UDP
    int socket_client = socket(PF_INET, SOCK_DGRAM, 0);
    if (socket_client == -1) {
        std::cerr << "[Client] Erreur : impossible de créer le socket." << std::endl;
        return EXIT_FAILURE;
    }

    //configuration de l'adresse du serveur
    sockaddr_in adresse_serveur{};
    std::memset(&adresse_serveur, 0, sizeof(adresse_serveur));
    adresse_serveur.sin_family = PF_INET;
    adresse_serveur.sin_port = htons(port);

    if (inet_pton(PF_INET, adresse_ip.c_str(), &adresse_serveur.sin_addr) <= 0) {
        std::cerr << "[Client] Erreur : adresse IP invalide." << std::endl;
        close(socket_client);
        return EXIT_FAILURE;
    }

    std::string chaine_a_envoyer;
    std::cout << "[Client] Entrez une chaîne (max " << MAX_CHAINE << " caractères) : ";
    std::getline(std::cin, chaine_a_envoyer);

    //limiter la chaîne à MAX_CHAINE caractères
    if (chaine_a_envoyer.length() > MAX_CHAINE) {
        chaine_a_envoyer = chaine_a_envoyer.substr(0, MAX_CHAINE);
    }

    //envoi du message au serveur
    ssize_t envoye = sendto(socket_client, chaine_a_envoyer.c_str(), chaine_a_envoyer.size(), 0,
                            reinterpret_cast<sockaddr*>(&adresse_serveur), sizeof(adresse_serveur));
    if (envoye < 0) {
        std::cerr << "[Client] Erreur lors de l'envoi." << std::endl;
        close(socket_client);
        return EXIT_FAILURE;
    }

    std::cout << "[Client] Message envoyé : " << chaine_a_envoyer << std::endl;

    close(socket_client);
    return 0;
}